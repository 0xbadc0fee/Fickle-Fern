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
`harness_step`/exported-accessor wiring). No source file outside this HAL/IO
cluster (`hw_outputs.c`, `hw_inputs.c`, `output_handler_lib.c`,
`input_handler_lib.c`, `alarm_handler_lib.c` not yet probed) has been checked
for Category-1 breakage — the "zero breakage so far" finding is scoped to
what's actually been probed, not a claim about the whole codebase.
