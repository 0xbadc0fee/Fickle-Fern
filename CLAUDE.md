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
