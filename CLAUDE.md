# Fickle-Fern — Host DLL Build Variant (Silent-Sagebrush harness target)

## What this repo is
An STW ESX.4CM-A C application, normally cross-compiled for Infineon
AURIX/TriCore via HighTec `tricore-gcc` → signed `.hex`. That build now works
from the command line via a Makefile (1:1 `.hex` artifact match with the
original Eclipse `.cproject` build confirmed). **DO NOT alter or regress the
TriCore build.**

## What we are adding
A SECOND, parallel build target: the same application source, compiled by a
HOST compiler (MinGW-w64 GCC, x86-64 Windows) into a `.dll`. Purpose: run the
application's LOGIC on a development PC with no physical controller, so a
Python bridge (cffi, API mode) can drive logic-only tests.

This is a new build VARIANT, NOT a fork. Same source, selected at build time
via a `-DSVG_HARNESS` (host-target) macro. Keep the two build paths cleanly
separated (separate make targets / output dirs); never mix TriCore and host
objects. The two build systems coexist in one repo but must never
cross-contaminate: TriCore Makefile → `.hex` (real, shippable) vs. host target
→ `.dll` (stub-linked, test-only), selected by which shell you're in and which
make target you invoke.

## Hard scope constraints (do not violate)
- **LOGIC ONLY.** In-scope: pure-callee I/O-signal surface (ADC, digital I/O,
  PWM in/out, frequency). OUT-of-scope and NOT to be simulated: RTOS/scheduler,
  CAN/LIN/ETH/J1939/SENT/serial, inter-core (ICC).
- The TriCore `.a` BIOS archives CANNOT be linked host-side (wrong
  architecture). Host link is satisfied by developer-authored STUB `.c` files
  providing matching `x_*` / `osy_*` symbols.
  - In-scope symbols: Tier B/C behavior (plausible/scriptable values).
  - Out-of-scope symbols: Tier A (link-only, return benign constant). They
    exist ONLY to satisfy the linker; they must do nothing. This is how
    logic-only is enforced STRUCTURALLY, not just documented.
- Stub builds are TEST ARTIFACTS ONLY, never shipped. Keep the real-vs-stub
  boundary bright. (STW-Technic is a distributor of the upstream vendor's
  binaries; a stubbed build must never be mistaken for a shippable one.)

## Bridge interface (Approach B — accessor functions)
The Python bridge does NOT read raw struct memory. Expose the harness via
EXPORTED accessor functions (`__declspec(dllexport)`), e.g.
`harness_init()`, `harness_step()` (run one logic cycle), and
`harness_set_signal(id, raw)` / `harness_get_signal(id)` that internally
read/write the application's owned device-state struct.
Data crossing the seam is RAW by contract; NO scaling in C (scaling is the
Python bridge's job later, via a matched DBC).
Windows note: symbol export is OPT-IN — nothing is reachable from Python
unless explicitly exported.

### IMPORTANT — accessors are HAND-WRITTEN for now, not generated
This repo does NOT yet contain the project's X-macro `.def` signal tables
(`SVG2CNTRL` / `CNTRL2SVG` / `CNTRL2DP`) or the `VAR_ASSIGN` machinery — that
work arrives separately (Petulant-Puffin). For THIS step, hand-write a small
number of accessor functions directly against the existing signal storage.
Do NOT invent a code-generation scheme, macro layer, or `.def`-style table for
the accessors — a generated mechanism is coming later and yours would collide
with it. Keep the hand-written accessors few, obvious, and easy to
delete/replace.

**Injection target (CONFIRMED present in this repo):**
`gt_can_devs`, a single global of type `T_CANDevices`, declared in
`appl_core/src/HAL/can_handler_lib.c` (with an `extern` in the matching
`can_handler_lib.h`). All modules share this one instance via a
`T_CANDevices*` pointer taken at `init_*()` time — there is no second/shadow
instance in `safety_core` or `boot_core`. This is the application-owned
device-state struct the harness reads/writes. Expose accessor pairs that
read/write fields of THIS global. Do NOT create a shadow or second instance.

**Keep the field surface tiny for now.** For the first proof, expose ONE field
(or a small handful) that application logic visibly reacts to — enough to poke
from Python and watch the logic move. Do NOT build out a hand-maintained
naming scheme / signal map across all of `T_CANDevices`; that map has a future
owner (the `.def`-generated mechanism) and a hand-built one would have to be
torn out.

## Terminology note — two unrelated things called ".def"
If you use a Windows module-definition file to declare DLL exports, that is a
LINKER artifact, entirely unrelated to this project's future X-macro `.def`
signal tables. Prefer `__declspec(dllexport)` in source over a module-def file
to avoid the naming collision, unless there's a concrete reason not to.

## Definition of done (this step)
1. `make` (or a documented target) produces the harness `.dll` via MinGW-w64
   WITHOUT touching the TriCore build.
2. The DLL links: every referenced symbol is satisfied by app code or stubs.
3. At least `harness_init` / `harness_step` and one signal accessor pair are
   exported and callable from outside the DLL.

## Discovery method for the stub symbol work list
- Undefined symbols the app demands: `nm` on the host `.o` files, ` U ` entries
  (a fully-linked binary has no ` U ` left — use the object files).
- Cross-reference with the TriCore archive's ` T ` (defined) symbols.
- Highest-value single artifact: a host link map (`-Wl,-Map=harness.map`) —
  it enumerates every symbol, which object requested it, and what satisfied it,
  with no change to build output.
- Expect failures in this order:
  - **Category 1** — TriCore headers/intrinsics/pragmas MinGW can't parse →
    host shim headers + `#ifdef SVG_HARNESS` guards.
  - **Category 2** — unresolved in-scope symbols → Tier B/C stubs.
  - **Category 3** — unresolved out-of-scope symbols → Tier A link-only stubs.

## Report back (feeds the parent project)
As you go, record what you actually discover — the real Category-1 breakages
hit, the concrete stub symbol work list, which source files compiled clean vs.
needed shims. This discovery is itself a deliverable (it maps the application's
logic/hardware entanglement); surface it rather than only fixing silently.

## Findings to date (milestone: BIOS stub file complete, 2026-08-07)

`Testing/harness_bios_stubs.c` now exists and is verified: it closes the
entire `x_in_*`/`x_out_*`/`x_msw_*`/`x_uext_*` BIOS symbol surface demanded by
the in-scope HAL layer (`hw_outputs.c`, `hw_inputs.c`, `output_handler_lib.c`,
`input_handler_lib.c`). Built per `Testing/harness_bios_stubs_spec.md`
(Option 2 — all symbols stubbed in one pass, not just the lighting subset).

**Category-1 breakage: none, anywhere probed so far.** `hw_outputs.c`,
`hw_inputs.c`, `output_handler_lib.c`, `input_handler_lib.c`, and the full
vendor header chain they pull in (`x_out.h`, `x_out_client.h`, `x_msw.h`,
`x_uext.h`, `x_in.h`, `x_in_client.h`, `x_stdtypes.h`, `stwtypes.h`,
`stwerrors.h`, `gcc_attributes.h`) all compile clean under MinGW-w64 with
`-Wall -Wdouble-promotion`, zero warnings. No TriCore intrinsic/pragma shim
has been needed yet. (Only fix needed anywhere in this cluster was an
include-path gap: `output_handler_lib.h` pulls `osy_com_j1939_dm1.h`, which
lives in `libs/opensyde_j1939_handler/`, not the core HAL include dirs.)

**Real BIOS symbol count is 34, not the 31 first estimated in the stub spec.**
Two symbol names in that spec's inventory don't exist as real functions
(`x_out_set_dither` — only `x_out_cc_set_dither` is real; `x_out_pid_parameters`
— that's the type `T_x_out_pid_parameters`, the real function is
`x_out_set_control_parameters`). Four real symbols were missing from that
inventory because they live in headers the spec didn't list
(`x_msw.h`/`x_uext.h`, not `x_out.h`): `x_msw_set_state` (main-switch enable,
called at `output_handler_lib.c` init), `x_uext_diag`,
`x_uext_set_voltage_setpoint`, `x_uext_get_active_faults` (the `vref_1` UEXT
reference output in `hw_outputs.c`). Net: 31 (spec) − 2 (phantom) + 4
(missing) = 34 (actual, confirmed by full symbol-closure check).

**Out-parameter population was needed in exactly 3 places**, not on every
diagnostic function as a first read of the spec implied: `x_out_get_active_faults`,
`x_in_get_active_faults`, `x_uext_get_active_faults`. Every other diag function
(`x_out_digital_diag`, `x_out_pwm_diag`, `x_out_cc_diag_v2`, `x_in_digital_diag`,
`x_in_voltage_diag`, `x_in_current_diag`, `x_in_frequency_diag`, `x_uext_diag`)
has no out-parameter in its real signature — return-code only. Always check the
real header before assuming a diag function needs out-param writes.

**No stub needed to go deeper than "return constant / touch harness memory."**
Every Tier C accessor bottoms out in one of four small harness-owned arrays
sized off the vendor's own `X_OUT_COUNT`/`X_IN_COUNT`/`X_MSW_COUNT`/
`X_UEXT_COUNT` constants (29/32/3/4), indexed directly by channel since all
`X_OUT_*`/`X_IN_*`/`X_MSW_*`/`X_UEXT_*` IDs are contiguous 0-based. No stub
calls back into the BIOS surface, so the undefined-symbol list never grew
during implementation.

**Confirmed Tier A (inert, out-of-scope) symbols:** `x_out_client_await_allocations`
and its input-side mirror `x_in_client_await_allocations` are genuine
inter-core (ICC) handshakes — call-site comments read "wait for safety core
output/input allocation" (appl_core is a "client" awaiting a channel-ownership
allocation from `safety_core`). Confirmed via `appl_core.map`/`.xml`: the real
TriCore implementation of `x_in_client_await_allocations` compiles to just 4
bytes, reinforcing that a benign-constant stub is a faithful match, not an
oversimplification.

**Toolchain correction — gcc works from both Bash and PowerShell.** An earlier
belief (and a claim repeated in `harness_bios_stubs_spec.md`) that the Bash
tool's sandbox "silently kills cc1.exe" and PowerShell was required is
**incorrect** and was retested twice. The actual cause of the silent
exit-1/zero-output failure: MSYS2's `C:\msys64\mingw64\bin` was not on `PATH`,
so `cc1.exe` (which depends on DLLs in that directory — libgmp/libmpfr/libmpc/
libiconv/libwinpthread/zlib) failed to load before it could print anything.
This reproduces identically in Bash and PowerShell, and is fixed identically
in both by prepending `C:\msys64\mingw64\bin` to `PATH` for the session
(`export PATH="/c/msys64/mingw64/bin:$PATH"` in Bash,
`$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH` in PowerShell). Invoking
gcc by full path alone, without that PATH entry, is not sufficient. Any
future doc/spec should stop citing "Bash sandbox kills cc1" as the reason to
prefer PowerShell — the real requirement is just the PATH fix, and either
shell works once it's applied.

**Verification method used:** compiled `harness_bios_stubs.c` alone (34
defined, 0 undefined via `nm`), then compiled it together with all four
in-scope HAL files and computed the union of every `U` (undefined) symbol
across all five objects, cross-checked against the union of every `T`
(defined) symbol in the same five objects. Only `strcmp` remained unresolved
— standard MinGW libc, resolved automatically at final DLL link, not a BIOS
concern. Zero unresolved BIOS symbols confirmed.

**Not yet done:** no host CMake/build target or actual `.dll` link exists yet
(this was object-file-level verification only, no `harness_init`/
`harness_step`/exported-accessor wiring). As of the prior milestone, no source
file outside the HAL/IO cluster (`hw_outputs.c`, `hw_inputs.c`,
`output_handler_lib.c`, `input_handler_lib.c`) had been checked for Category-1
breakage — the "zero breakage so far" finding was scoped to what had actually
been probed, not a claim about the whole codebase. (`alarm_handler_lib.c`,
`nvm_handler_lib.c`, `can_handler_lib.c` are now covered — see next milestone.)

## Findings to date (milestone: alarm/nvm/can probe complete, 2026-08-10)

Probed the three files the prior milestone flagged as not-yet-checked, per
the handoff doc's recommended next step. Compiled individually host-side
(MinGW, `-DSVG_HARNESS -Wall -Wdouble-promotion`) using the include-path set
from `.cproject`/`appl_core/CMakeLists.txt`'s `appl_core` target scope.

**`alarm_handler_lib.c` and `nvm_handler_lib.c`: zero Category-1 breakage,
zero warnings, clean compiles.** Their undefined-symbol sets split cleanly by
scope:
- `alarm_handler_lib.c` needs `at_vehicleInputs`, `at_vehicleOutputs`,
  `get_numInputs`, `get_numOutputs` (already satisfied by
  `output_handler_lib.c`/`input_handler_lib.c`) and `fault_nvm_init`,
  `fault_nvm_write` (satisfied by `nvm_handler_lib.c`). It also needs
  `gat_DmDtcs`, `osy_com_j1939_dm1_lock_tx`, `osy_com_j1939_dm1_unlock_tx`,
  `osy_j1939_set_lamps` — all defined in
  `appl_core/src/opensyde/j1939/osy_app_j1939.c`, confirmed out-of-scope
  (J1939 lamp/DTC comm layer). These need Tier A inert stubs.
- `nvm_handler_lib.c` needs `memset` (stdlib, resolves at DLL link) plus two
  distinct families: `x_nvm_read`/`x_nvm_write`/`x_nvm_get_size` (raw BIOS
  symbols, same `x_*` naming convention as the already-stubbed `x_in`/`x_out`
  surface — **new Tier B/C stub work, not yet in `harness_bios_stubs.c`'s
  34**), and `osy_dph_nvm_read_list`/`osy_dph_nvm_write_list`/
  `osy_dph_apply_data_set` (openSYDE DataPool Handler layer — scope not yet
  decided, see below).

**`can_handler_lib.c`: a genuine NEW Category-1 breakage, but a compiler-
version gate, not an intrinsic/pragma parse failure.**
`osy_dpa_trg_definitions.h` (reached transitively via
`osy_com_engine.h → osy_com_configuration.h → osy_dpa_data_pool.h`, all under
`appl_core/libs/opensyde_server`) unconditionally `#error`s unless
`__GNUC__`/`__GNUC_MINOR__`/`__GNUC_PATCHLEVEL__` exactly match HighTec's
4.9.4:
```c
#if ((__GNUC__ != 4) || (__GNUC_MINOR__ != 9) || (__GNUC_PATCHLEVEL__ != 4))
#error This Library shall only be used with HighTec TriCore Development Platform V4.9.2.0...
```
Spoofing the three version macros on the command line
(`-U__GNUC__ -D__GNUC__=4 ...`) as a one-off diagnostic probe let the rest of
the header chain parse clean under MinGW 16+ with zero warnings — so this is
an isolated version pin, not a deeper intrinsic/pragma entanglement. The
macro spoof is a probe trick only; it is NOT proposed as the real build's
solution. Undefined symbols surfaced once past the gate:
`x_can_bus_init`, `x_osf_can_get_param` (raw BIOS CAN symbols),
`osy_com_engine_force_tx_message`, `osy_com_engine_set_message_active`,
`osy_dph_lock_data_pool`, `osy_dph_unlock_data_pool` (openSYDE CAN comm
engine), and `gt_J1939_DataPoolValues`, `gt_comm_j1939_can1_ProtocolConfiguration`
(openSYDE-generated J1939 datapool/protocol globals).

**Scope-classification correction: BIOS `x_*` naming is not itself the
scope test.** `x_in_*`/`x_out_*` were in-scope and got behavioral Tier B/C
stubs. `x_can_bus_init`/`x_osf_can_get_param` are the same BIOS naming
convention but CAN is explicitly out-of-scope per this doc's hard scope
constraints — so they must be Tier A inert, not Tier C, despite looking
structurally identical to the in-scope BIOS surface. Stubbing by pattern-
matching the `x_*` prefix alone would silently violate the logic-only
boundary.

**Open design question surfaced, not resolved: `gt_can_devs` (the confirmed
accessor injection target) is defined inside a file that cannot compile
host-side without both the GCC-version-gate workaround above AND a full Tier
A stub set for the CAN/J1939 comm engine it's entangled with.** Unlike the
clean I/O HAL files, `can_handler_lib.c` is not a pure HAL boundary —
`init_canInterfaces`/`update_canInputs`/`update_canOutputs`/
`force_canMessage`/`set_canMessageActive` are CAN/J1939 engine logic wrapped
around the same global the Bridge Interface accessors need to read/write.
Since a shadow/second instance of `T_CANDevices` is explicitly disallowed,
there is no way to get the struct's real definition into the DLL without
pulling this file into the host build — meaning step 3 of the "Recommended
next steps" (accessor wiring against `gt_can_devs`) inherits both this
Category-1 workaround and a nontrivial Tier A stub set as prerequisites, not
just the BIOS stubs already done. Decision on how to handle this is pending.

**Still not yet probed:** no other files outside the HAL/IO cluster and these
three have been checked for Category-1 breakage.

## Findings to date (milestone: `gt_can_devs` separability discovery, 2026-08-10)

Follow-up to the open design question above. Goal: confirm whether
re-homing just the `gt_can_devs` *definition* into a harness-owned file
(instead of compiling `can_handler_lib.c`) leaves any other unresolved
reference into that translation unit. Discovery only — no build or harness
code written.

**`CAN_PROVIDES`** (every external-linkage symbol `can_handler_lib.c`
defines, confirmed via `nm` on the compiled `.o` and cross-checked against
`can_handler_lib.h`'s prototypes — exact match, 7 symbols): `gt_can_devs`,
`init_canInterfaces`, `update_canInputs`, `update_canOutputs`,
`force_canMessage`, `set_canMessageActive`, `can_get_availability_state`.
(`maq_CanAvailable` is `static` — internal only, not in this set.)

**`CAN_NEEDED` (intersection with the undefined-symbol union of the full
in-scope host-build set — `hw_outputs.c`, `hw_inputs.c`,
`output_handler_lib.c`, `input_handler_lib.c`, `alarm_handler_lib.c`,
`nvm_handler_lib.c`, `harness_bios_stubs.c`): EMPTY.** None of those seven
files reference any `CAN_PROVIDES` symbol, not even `gt_can_devs` itself —
this cluster doesn't touch the CAN device struct at all yet.

**But that's an artifact of how narrow the probed set still is, not proof
of clean separability on its own** — a repo-wide grep for each
`CAN_PROVIDES` symbol found real consumers outside the probed cluster:
- `gt_can_devs` — read/written by 17 `init_*Control(&gt_can_devs, ...)`
  calls in `main.c` (elevator, header, hitch, auger, light, cChains,
  frontSweeps, rotaryTrap, stickB, stickRemover, propulsion, powerAssist,
  suctionFan, engineStarter, throttle, coolingFan, misc — all under
  `AgvChassis`/`AgvWork`, none probed yet), and by the existing
  "SVG Harness step 1" strand (`harness_svg_buttonPanel.c` +
  `svg_in_map_buttonPanel.def`/`svg_out_map_buttonPanel.def`). Also written
  by `can_in_map.def`/`can_out_map.def` — but those are `#include`d only
  from inside `can_handler_lib.c`'s `update_canInputs`/`update_canOutputs`,
  so they don't independently pull anything into a harness build.
- `init_canInterfaces`, `update_canInputs`, `update_canOutputs` — called
  only from `main.c`'s setup/loop. Pure CAN transport entry points, not
  logic; a harness `harness_init()`/`harness_step()` should not call these
  at all (nothing to stub — just don't invoke them).
- `can_get_availability_state` — called only from `opensyde/osy_srv.c`
  (openSYDE server diagnostics), fully out-of-scope call site.
- `force_canMessage`, `set_canMessageActive` — called from
  `AgvChassis/throttle_control.c`, a genuine **application-logic** file.
  **This is the one real hidden dependency beyond `gt_can_devs`.** Both
  functions exist purely to force/gate a J1939 message's transmission (they
  bottom out in `osy_com_engine_force_tx_message`/`_set_message_active`) —
  inherently a transport side-effect, not logic. If `throttle_control.c`
  (or a similar Agv module) ever joins the host build, these two symbols
  become genuinely required, but satisfying them is a **trivial Tier A
  no-op stub, not a scope violation** — "force this CAN message" correctly
  doing nothing is exactly what logic-only mode should do.

**Reverse direction (does `gt_can_devs`'s type or init pull in anything
`can_handler_lib.c`-specific?): no.** `T_CANDevices` is defined in
`can_device_definition.h` (`appl_core/src/System/CAN`), not in
`can_handler_lib.c` — composed of `T_JoystickJS6000`, `T_8ButtonPanel`,
`T_8772_Display` (HMI headers) and `T_Engine` (`can_engine.h`). Confirmed
this header compiles clean host-side standalone, zero warnings, **with no
dependency on the version-gated openSYDE chain** (see below). `gt_can_devs`
itself has no initializer in `can_handler_lib.c`, so it's plain
zero-initialized BSS (confirmed via `appl_core.map`: `.bss.gt_can_devs`) —
`init_canInterfaces` never touches it, so there's no "must call init before
logic reads it" ordering dependency to replicate.

**New Category-1-adjacent finding: the GCC-version `#error` gate from the
prior milestone is reachable through `can_handler_lib.h` too, not just the
`.c` file.** `#include "can_handler_lib.h"` alone (no `.c` body) hits the
identical `osy_dpa_trg_definitions.h` `#error`, via
`can_handler_lib.h → j1939_data_pool.h → osy_dpa_data_pool.h →
osy_dpa_trg_definitions.h`. So "harness includes `can_handler_lib.h` just to
match the `extern`" does NOT dodge the gate — confirmed by direct probe.
The clean bypass: include `can_device_definition.h` directly (defines the
type, zero entanglement with the openSYDE version gate, confirmed above)
and write the harness's own `extern T_CANDevices gt_can_devs;` against
that, never including `can_handler_lib.h` in the host build. **This exact
pattern already exists as precedent**: `harness_svg_buttonPanel.h` (the
earlier, separate SVG-harness strand) independently redeclares
`extern T_CANDevices gt_can_devs;` rather than including
`can_handler_lib.h` — apparently for this same reason, discovered
independently.

**Verdict:** `gt_can_devs` is cleanly separable for the host build *as
currently scoped* (7-file HAL/IO cluster) — zero additional symbols needed,
and the definition can be re-homed by including `can_device_definition.h`
directly rather than `can_handler_lib.h`. This is NOT yet proof the harness
is done with CAN entanglement, though: the moment application-logic modules
that actually consume `gt_can_devs` (`main.c`'s `init_*Control` calls, or
`throttle_control.c`) join the host build, `force_canMessage` and
`set_canMessageActive` become newly required — trivial Tier A stubs, not a
redesign. Proposed minimal Tier A stub surface for that future point (not
needed yet): `force_canMessage` and `set_canMessageActive` as inert
no-op/void stubs; `init_canInterfaces`/`update_canInputs`/
`update_canOutputs`/`can_get_availability_state` should simply not be
called from any harness code path rather than stubbed.

## Decision (2026-08-10): harness owns the `gt_can_devs` definition

**Committed.** The host build will define `T_CANDevices gt_can_devs;` in a
new harness-owned file (`Testing/harness_can_devs.c`, parallel to
`harness_bios_stubs.c`), including `can_device_definition.h` directly (never
`can_handler_lib.h`, per the version-gate finding above). `can_handler_lib.c`
is excluded from the host build entirely.

**Why this doesn't violate the "no shadow/second instance" hard constraint:**
that rule guards against a second copy of `gt_can_devs` coexisting with the
real one *inside one linked binary* (e.g. safety_core/boot_core disagreeing
with appl_core). The host `.dll` and the TriCore `.hex` are never linked
together — `can_handler_lib.c` is wholly absent from the host variant, so
the harness definition is the ONLY `gt_can_devs` in that build target, not a
second one alongside the real one. Different build target, same "exactly
one shared instance" rule, satisfied.

**Known, bounded, deferred consequence (not solved by this decision, not
blocking it either):** `throttle_control.c` is the one application-logic
file that both meaningfully consumes `gt_can_devs` and calls
`force_canMessage`/`set_canMessageActive`; it `#include`s `can_handler_lib.h`
directly and will hit the identical `osy_dpa_trg_definitions.h` version gate
the moment it's added to the host build. (Repo-wide, only `main.c`,
`opensyde/osy_srv.c` — out of scope — and `throttle_control.c` include
`can_handler_lib.h` at all; a small, already-anticipated Category-1 shim
item for whenever that file joins the build, not a new open question.)

## Findings to date (milestone: `x_nvm_*` stub family added, 2026-08-10)

Implemented handoff step 4: `x_nvm_read`, `x_nvm_write`, `x_nvm_get_size`
added to `harness_bios_stubs.c` as a new "Section D" (Tier C), matching the
existing file's structure and channel-array pattern from the original
34-symbol closure.

**Design choice worth recording: the backing store must emulate an erased
EEPROM (`0xFF` per byte), not a zeroed one.** `nvm_handler_lib.c`'s
`fault_nvm_init()` finds an unused slot by scanning for the header sentinel
`u32_spn == 0xFFFFFFFF` — the standard "never written" value for real
EEPROM/flash. A naively zero-initialized harness array would never satisfy
that check: every call would fall through the loop to
`NVM_EEPROM_OVERFLOW` without ever finding (or creating) a slot, silently
breaking `alarm_handler_lib.c`'s fault-persistence path the first time it
ran. Fixed with a lazy one-time `memset(..., 0xFF, ...)` on first
`x_nvm_read`/`x_nvm_write` call (`EnsureHarnessNvmErased()`), rather than a
GNU designated-range initializer, to stay in portable C. This is the kind
of Tier C nuance the spec's "no stub needed to go deeper than return
constant / touch harness memory" claim doesn't cover by itself — sizing
*and seeding* harness-owned storage correctly can matter as much as wiring
it up. Backing store sized at a self-contained 32 KiB constant (comfortably
above the real code's actual high-water mark, `NVM_FAULTS_START_ADDR +
NVM_FAULTS_SIZE` = 31000), not derived from including the app-layer
`nvm_handler_lib.h`, keeping this file's dependencies limited to vendor
BIOS headers as before.

**Verification:** compiled clean under MinGW (`-Wall -Wextra
-Wdouble-promotion`, zero warnings). Full closure check across the entire
in-scope host-build set (four I/O HAL files + `alarm_handler_lib.c` +
`nvm_handler_lib.c` + `harness_bios_stubs.c` + `harness_can_devs.c`,
undefined-symbol union minus defined-symbol union) confirms all three new
symbols fully resolved, with no new collateral undefined symbols
introduced. Remaining unresolved symbols are exactly the already-tracked
ones: stdlib (`memcpy`/`memset`/`strcmp`, resolve at DLL link) and the two
still-open scope questions (J1939/DM1 cluster from `alarm_handler_lib.c`;
`osy_dph_*` DataPool Handler cluster from `nvm_handler_lib.c`) — see
handoff steps 5 and 6.

## Decision (2026-08-10): `alarm_handler_lib.c` excluded from the first host build

**Committed.** `alarm_handler_lib.c` is not part of the first host `.dll`
pass. This is a deliberate deferral, not an oversight — the J1939/DM1 Tier
A stub work it would require (`gat_DmDtcs`, `osy_com_j1939_dm1_lock_tx`/
`_unlock_tx`, `osy_j1939_set_lamps`, all defined in
`appl_core/src/opensyde/j1939/osy_app_j1939.c`) is fully scoped from the
2026-08-10 probe above and can be picked up whenever alarm-handling logic
is actually needed for a harness scenario. No stub code exists for these
four symbols yet, and none is needed until this decision is revisited.

## Decision (2026-08-10): `nvm_handler_lib.c` also excluded from the first host build

**Committed, alongside the `alarm_handler_lib.c` decision above.** The
`osy_dph_*` DataPool Handler scope question (Tier A vs. Tier B/C — see the
`nvm_handler_lib.c` probe milestone) is deliberately deferred, not
resolved. Since `osy_dph_nvm_read_list`/`_write_list`/`_apply_data_set`
remain genuinely unresolved without that decision, and `nvm_handler_lib.c`
has no other consumer in the currently-included file set (only
`alarm_handler_lib.c` calls `fault_nvm_init`/`fault_nvm_write`, and that
file is already deferred above), `nvm_handler_lib.c` is excluded from the
first host `.dll`'s source list too. The `x_nvm_*` stubs already written
and verified (previous milestone) are unaffected and ready for whenever
this file rejoins the build.

## Findings to date (milestone: host CMake target stood up, `svg_harness.dll` builds, 2026-08-10)

Implemented handoff step 7. New files:
`ESX_4CM_A/cmake/host-toolchain.cmake` (MinGW-w64 toolchain file, parallel
to the existing `tricore-toolchain.cmake` but for a native, non-cross
build) and `ESX_4CM_A/host_harness/CMakeLists.txt` — a genuinely separate
CMake project (its own `project()` call), never `add_subdirectory`'d from
the TriCore-toolchain-bound top-level `ESX_4CM_A/CMakeLists.txt`. Builds
`svg_harness.dll` (`add_library(... SHARED ...)`, `PREFIX ""`) from exactly
the six files the prior decisions settled on: `hw_outputs.c`,
`hw_inputs.c`, `output_handler_lib.c`, `input_handler_lib.c`,
`harness_bios_stubs.c`, `harness_can_devs.c`. Include-path list mirrors
`appl_core/CMakeLists.txt`'s own list verbatim (just re-based with `../`),
kept identical rather than hand-pruned so a file can move between the
TriCore and host targets without either side's include list needing
separate upkeep.

**This was verified by actually running `cmake -S`/`cmake --build`, not
just authoring the files** — surfaced two real findings neither of which
would have shown up from inspection alone:

1. **A relative `CMAKE_TOOLCHAIN_FILE` path failed to resolve** even from
   what looked like the correct invocation directory
   (`Could not find toolchain file: ESX_4CM_A/cmake/host-toolchain.cmake`).
   cmake's resolution base for that variable isn't reliable enough across
   versions/contexts to leave relative — always pass an absolute path.
   Documented directly in both the toolchain file's usage comment and the
   handoff doc so this doesn't cost a second debugging cycle later.

2. **MinGW's `ld` auto-exports every public symbol from a `-shared` build
   by default — the opposite of MSVC's opt-in default.** The first
   successful link produced a DLL with **114 exported symbols** despite
   zero `__declspec(dllexport)` annotations anywhere in the source:
   every vendor BIOS stub, every HAL function, and — critically — raw
   internal state like `gt_can_devs`, individual device globals
   (`tvo_headlights`, `tvi_wheel_speed`, etc.), all directly readable/
   writable from Python via ctypes with no accessor layer involved. This
   would have silently violated CLAUDE.md's explicit "symbol export is
   OPT-IN — nothing is reachable from Python unless explicitly exported"
   hard constraint, and defeated the entire point of the Approach-B
   accessor-function boundary, had it gone unnoticed. **Fixed** by adding
   `-Wl,--exclude-all-symbols` to `target_link_options`; reverified via
   `objdump -p` that the export table is now genuinely empty (zeroed
   Export Directory entry, no `Export Tables` section at all). This flag
   is load-bearing for the whole project's scope-enforcement model, not a
   stylistic choice — worth remembering if this CMake pattern is ever
   copied elsewhere.

**Verification:** `cmake --build build-harness` completes with zero
errors, links `svg_harness.dll`. `git status` confirms no TriCore-side
file (`appl_core/CMakeLists.txt`, top-level `CMakeLists.txt`,
`tricore-toolchain.cmake`) was touched — the two build paths remain
cleanly separated per the hard scope constraints. `.gitignore` updated
with `/build-harness/` (previously only `/build/` was covered).

**Not yet done:** the DLL currently exports nothing (by design, until step
8 adds `__declspec(dllexport)` to `harness_init`/`harness_step`/the first
accessor pair) — this is expected, not a defect, at this stage.

## Findings to date (milestone: `harness_init`/`harness_step`/first accessor pair, 2026-08-10)

Implemented handoff step 8 — this repo's originally-stated "definition of
done" for the whole SVG_HARNESS effort. New files
`ESX_4CM_A/appl_core/src/Testing/harness_api.c`/`.h`, added to
`host_harness/CMakeLists.txt`'s source list. These are now the DLL's entire
public surface (enforced structurally by `-Wl,--exclude-all-symbols` from
the previous milestone, not just by convention):

- `harness_init()` — zeroes `gt_can_devs` via `memset`, then calls
  `init_hwInputs()`/`init_hwOutputs()`.
- `harness_step()` — calls `update_hwInputs()`/`update_hwOutputs()`, the
  same I/O HAL read/write pair `main.c`'s real cyclic loop runs each pass
  (minus CAN, fault handling, and the AgvChassis/AgvWork controls, all
  still out of scope). This is genuine application logic executing, not a
  placeholder — it exercises the full HAL-to-BIOS-stub pipeline built over
  the previous milestones.
- `harness_set_button1_state`/`harness_get_button1_state` — the first
  hand-written accessor pair, per the Bridge Interface (Approach B). Raw
  `uint8` read/write of `gt_can_devs.t_buttonPanel.u8_b1_state` (0 = not
  pressed, 1 = pressed, 3 = fault, per `hmi_8button_panel.h`). Named
  concretely rather than as a generic `harness_set_signal(id, raw)`
  dispatcher — CLAUDE.md's Bridge Interface section explicitly warns
  against inventing a lookup-table/macro mechanism this early, and a
  generic id-dispatch function would start to be exactly that.

**Verified with an actual ctypes smoke test loading the built DLL** (not
committed to the repo — a throwaway scratch script), not just a successful
compile: `harness_init()` → get returns 0 → `set(1)`/`set(3)` → get reflects
each value immediately → `harness_step()` runs without error.

**Real finding, not a style note: `harness_init()` is NOT safe to call
twice against the same DLL load.** The first implementation's comment
claimed it was ("safe to call again to reset state between test runs") —
the smoke test caught this being false: a second `harness_init()` call
returned `C_INPUT_INIT_HW_FAIL` (-20), not success. Root cause:
`init_hwInputs()`/`init_hwOutputs()` register each hardware channel via
`add_hwInput()`/`add_hwOutput()` (in `input_handler_lib.c`/
`output_handler_lib.c`) against a monotonically-growing static counter
(`u8_numInputs`/`u8_numOutputs`) with no reset path anywhere in the
application's own code. A second call re-registers the same hardware IDs,
which `add_hwInput()`'s own duplicate-ID check correctly rejects. This
mirrors the real controller's own assumption — init runs exactly once per
boot — and is not a harness-introduced limitation. Resolved by correcting
the false claim (comment + this doc) rather than by adding an unrequested
reset mechanism to application logic: `harness_init()` is one-time-per-
load; a test suite needing a clean slate between cases must reload the DLL
process/handle, not call `harness_init()` a second time. This is exactly
the kind of thing a smoke test earns its keep by catching — the code
compiled clean and "looked" correct by inspection.

**Verification of the export boundary itself:** `objdump -p` on the
rebuilt `svg_harness.dll` shows exactly four exports —
`harness_get_button1_state`, `harness_init`, `harness_set_button1_state`,
`harness_step` — confirming the previous milestone's
`-Wl,--exclude-all-symbols` fix holds under real exports, not just under
the empty case. Nothing else leaked.

**This closes the repo's originally-stated three-part "Definition of done
(this step)"** from earlier in this document: (1) the `.dll` builds via
MinGW-w64 without touching the TriCore build, (2) it links with every
symbol satisfied, (3) `harness_init`/`harness_step`/an accessor pair are
exported and callable from outside the DLL.

## Findings to date (milestone: Seam 2 round-trip proven + promoted to reference, 2026-08-11)

Executed `reqs/HANDOFF_seam2_roundtrip.md` end to end. **Seam 2** (bridge
writes a harness-owned `gt_svg` mirror; a `.def`-driven X-macro copy step
moves values `gt_svg <-> gt_can_devs` on either side of the logic-under-test)
is now the project seam, proven against the `lighting_control` fixture and
promoted to reference. Seam 1 (per-signal accessors writing `gt_can_devs`
directly) is retired from the working tree — preserved in git history only.

**Repo layout changed to match the handoff's per-fixture-DLL manifest:**
harness MECHANISM (fixed across fixtures) now lives in
`Testing/harness/` — `harness_api.c/.h`, `harness_svg_copy.c/.h`,
`harness_can_devs.c/.h`, `harness_bios_stubs.c`. Fixture CONTENT (per
fixture) lives in `Testing/Fixtures/<fixture-name>/` — `fixture.def`,
`fixture_signals.c/.h`, `fixture_accessors.c`, `test_roundtrip.py`, the
fixture's `.svg`/`.html`. A fixture is selected at build time purely by
pointing `-I` at its folder so `#include "fixture.def"` /
`#include "fixture_signals.h"` in the harness resolve to that fixture —
the harness sources never change per fixture. `T_HarnessSignalsButtonPanel`
was renamed to `T_HarnessSignals` in the fixture files (module-agnostic,
so it doesn't imply "button panel" in a future elevator/other fixture).
`host_harness/CMakeLists.txt`'s three moved-file paths were updated to match,
but that CMake target still builds only the older, broader HAL-cluster
`svg_harness.dll` (from the 2026-08-10 milestone) — it is NOT what builds a
per-fixture DLL and was not otherwise touched. The per-fixture DLL is built
with a direct `gcc` invocation (below), not CMake.

**Working build command** (resolves the handoff's `<path-to-...>`
placeholders — TSP/app headers are the same include-path list already in
`host_harness/CMakeLists.txt`, just re-based relative to `appl_core/src`;
in-scope app sources beyond `hw_inputs.c`/`hw_outputs.c`/`lighting_control.c`
turned out to be exactly `input_handler_lib.c`/`output_handler_lib.c`, their
already-known HAL deps from the 2026-08-07 milestone). Run from
`ESX_4CM_A/appl_core/src` in PowerShell with
`C:\msys64\mingw64\bin` prepended to `PATH`:

```powershell
x86_64-w64-mingw32-gcc -shared -o Testing\Fixtures\lighting_control\lighting_control.dll `
  -DSVG_HARNESS -static -static-libgcc -Wl,--exclude-all-symbols `
  -ITesting\harness -ITesting\Fixtures\lighting_control -I..\libs\bios -I. -IAgvChassis `
  -I..\libs\opensyde_j1939_handler -I..\libs\lwip\include -I..\libs\opensyde_server `
  -Iopensyde -Iopensyde\j1939 -Iopensyde\generated `
  -IHAL -IHAL\STW_4CM_HAL -ISystem -ISystem\IO -ISystem\CAN -ISystem\CAN\Devices `
  -ISystem\Ethernet -ISystem\NVM -IAgvWork -IAgvDiag -IAgvHelper -ITesting `
  Testing\harness\harness_api.c Testing\harness\harness_svg_copy.c `
  Testing\harness\harness_can_devs.c Testing\harness\harness_bios_stubs.c `
  Testing\Fixtures\lighting_control\fixture_signals.c Testing\Fixtures\lighting_control\fixture_accessors.c `
  System\IO\hw_inputs.c System\IO\hw_outputs.c HAL\input_handler_lib.c HAL\output_handler_lib.c `
  AgvChassis\lighting_control.c
```

**Result: exit 0, exactly the 4 expected exports** (`harness_init`,
`harness_step`, `harness_set_button04_state`, `harness_get_button04_lights`,
confirmed via `objdump -x` — `--exclude-all-symbols` still holds under a real
fixture build). **ctypes round-trip test PASS**: `harness_init()` → 0,
released `lights = 0x01`, pressed `lights = 0x10`. This is not a loose
"something changed" result — `update_lightControl`'s state machine is
edge-triggered (toggles `u8_light_mode` on a 0→1 transition of the button
state), confirmed by reading the source, and the test's release-then-press
sequence lands exactly on that rising edge by construction.

Full recipe (with exact repro steps: build → confirm `lighting_control.dll`
lands beside `bridge.py` → `python bridge.py` → open `index.html` → press →
confirm `00000001` ↔ `00010000`) is recorded in `reqs/KNOWN_GOOD.md`, pinned
to tag `lighting_control-host_side_test-PASS`
(commit `2feb58fdf6bd76ad7a1c30331c3ebf61ce4ac65a`). Design rationale and
decision table: `reqs/EDR_seam2_lighting_control.md`.

**Open finding, not yet resolved — `harness_svg_buttonPanel.c/.h` is a live
symbol-collision risk, not dead code.** This file predates even Seam 1's
`harness_svg_lightControlTest.c` (it's the original "SVG Harness step 1"
button-panel bring-up artifact, `Created on: Aug 7, 2026`). It was not on
the handoff's Seam-1 retire list and the promote sequence deliberately left
it untouched. Separately (outside a Claude Code session — found already
modified in the working tree), it was edited to `#include
"Fixtures/lighting_control/fixture.def"` directly (hardcoded to this one
fixture, not selected via `-I` the way `harness_svg_copy.c` is — so unlike
the promoted mechanism, this file is NOT fixture-agnostic) and its
`gt_svg` member names were updated to match
(`u8_svgKeypad01_b04_state`/`_lights`). **It defines `sint16
update_harnessInputs(void)` / `update_harnessOutputs(void)` and a global
`gt_svg` (of type `T_HarnessSignalsButtonPanel`) — the exact same symbol
names `harness_svg_copy.c`/`fixture_signals.c` define for Seam 2.**
Compiling both into the same target would fail at link time with duplicate
symbols. The build command above and `reqs/KNOWN_GOOD.md`'s recipe do NOT
include `harness_svg_buttonPanel.c` — only `harness_api.c` +
`harness_svg_copy.c` (Seam 2's own mechanism) were compiled into the DLL
used for the passing bridge demo, so there is no actual conflict in the
known-good build. Why the file was edited at all, if it's not part of that
build, is unconfirmed — possible leftover exploration from before the
known-good recipe was settled on. Before ever building both `harness_api.c`
and `harness_svg_buttonPanel.c` into the same target, or before deleting
either file, confirm with the author which one (if either) is still meant
to be live — do not assume.
