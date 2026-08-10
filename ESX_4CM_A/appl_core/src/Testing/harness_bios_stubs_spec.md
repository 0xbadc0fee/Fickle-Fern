---
title: Host-DLL BIOS Stub File — Build Spec (x_in / x_out substitution)
project: STW ESX.4CM-A — Silent-Sagebrush host-DLL harness variant
repo: Fickle-Fern (github.com/0xbadc0fee/Fickle-Fern)
author: Silas Curfman (STW Technic, LP)
date: 2026-08-07
status: ready-for-implementation
doc_type: build-spec
audience: [llm-agent, human]
build_variant: host (MinGW-w64 x86-64 Windows -> .dll), gated by -DSVG_HARNESS
toolchain: MSYS2 MinGW-w64, x86_64-w64-mingw32-gcc.exe (GCC 16.1.0)
consumes: on_host_test_harness_stubbing_scope.md, CLAUDE.md
tags: [stubbing, bios, x_in, x_out, tier-a, tier-b, tier-c, host-dll, logic-only]
---

# Host-DLL BIOS Stub File — Build Spec (x_in / x_out substitution)

## TL;DR

> Create ONE C file (suggested `Testing/harness_bios_stubs.c`) that provides
> developer-authored stub bodies for the `x_in_*` / `x_out_*` BIOS symbols the
> in-scope I/O HAL references. The real TriCore `.a` archives cannot link
> host-side; these stubs satisfy the linker instead. The file `#include`s the
> REAL vendor headers (`x_out.h`, `x_in.h`) so the compiler checks each stub
> body against the official signature. 31 symbols total across the whole HAL;
> the **lighting PoC critical path needs only ~6** (Group L below). Choose
> Option 1 (lighting subset first) or Option 2 (all 31 now).

## Hard constraints (do not violate)

- **Host build only.** This file compiles into the MinGW `.dll` variant ONLY,
  never the TriCore `.hex` build. Keep it out of the TriCore source list.
- **PowerShell only** for all MinGW invocations (compile, `nm`, `objdump`,
  link). The Bash-tool sandbox silently kills `cc1.exe` — a real compile
  bare-exits 1 with no diagnostic. This is a known, retested trap.
- **Logic-only, structurally.** Out-of-scope symbols (Tier A) must be visibly
  inert — present to satisfy the linker, doing nothing. This is how logic-only
  is enforced by architecture, not just documented.
- **No scaling in C.** Values cross the seam raw. Any scaling is the Python
  bridge's job later.
- **Stubs are test artifacts only, never shipped.** STW-Technic distributes the
  upstream vendor's binaries; a stub build must never be mistaken for shippable.

## File setup

- **Name:** `Testing/harness_bios_stubs.c` (any name works — the linker matches
  symbol names, not file names — but this name documents intent).
- **First lines:** `#include "x_out.h"` and `#include "x_in.h"` (the REAL vendor
  headers). These parsed clean host-side in prior probing — zero Category-1
  breakage across the x_in/x_out chain — so NO host shim header is required.
  Including them lets the compiler enforce signature match; do NOT hand-type
  signatures, pull each exact prototype from the included header.
- If any include unexpectedly fails to parse under MinGW, that is a NEW
  Category-1 finding — record the exact error (pragma? intrinsic?
  `__attribute__`?) as an A5.3 entanglement data point, then shim only the
  offending declarations behind `#ifdef SVG_HARNESS`.

## Return-contract legend

- **Success constant:** `C_NO_ERR` (from `stwerrors.h`).
- **Out-parameter rule:** any function that returns status/values through a
  pointer argument MUST write a benign value to that pointer (e.g. "no fault" /
  zero), so caller logic reads a defined value, not uninitialized memory.
- **Harness storage:** Tier-C accessors read/write a harness-owned array/struct
  keyed by channel — NOT hardware, NOT the vendor archive. Define this storage
  in the stub file (or a shared harness header). It must bottom out in
  harness-owned memory so a stub never calls back into the vendor surface (which
  would create new undefined symbols and grow the work list).

---

## Symbol inventory (31) — grouped by tier

Symbols were extracted from actual call sites in the in-scope files
(`hw_outputs.c`, `hw_inputs.c`, `output_handler_lib.c`, `input_handler_lib.c`).
All output-side symbols funnel through `output_handler_lib.c`; input-side
through `input_handler_lib.c`.

### Section A — Tier A, INERT (out-of-scope: multicore allocation)

Accept args, ignore, `return C_NO_ERR;`. Comment each: *link-only, multicore
channel allocation not simulated host-side; harness is single-process.*

| Symbol | Notes |
|--------|-------|
| `x_out_client_await_allocations` | multicore handshake — inert |
| `x_in_client_await_allocations`  | multicore handshake — inert |

### Section B — Tier A/B, INIT + DIAGNOSTIC (in-scope surface, nothing to model)

Host-side there is no peripheral to configure and no fault to report.

**Init — return `C_NO_ERR`, do nothing:**

| Symbol |
|--------|
| `x_out_digital_init` |
| `x_out_pwm_init` |
| `x_out_cc_init` |
| `x_in_digital_init` |
| `x_in_voltage_init` |
| `x_in_current_init` |
| `x_in_frequency_init` |

**Diagnostic — return `C_NO_ERR` AND write "no fault" to any out-parameter:**

| Symbol | Out-param contract |
|--------|--------------------|
| `x_out_digital_diag` | write no-fault to status out-param |
| `x_out_pwm_diag` | write no-fault |
| `x_out_cc_diag_v2` | write no-fault |
| `x_in_digital_diag` | write no-fault |
| `x_in_voltage_diag` | write no-fault |
| `x_in_current_diag` | write no-fault |
| `x_in_frequency_diag` | write no-fault |
| `x_out_get_active_faults` | write "zero active faults" |
| `x_in_get_active_faults` | write "zero active faults" |

> The diag/get_active_faults out-params are the ONE place "return success" is
> insufficient — you MUST populate what they hand back, or the app's
> fault-gating logic reads garbage. Check the real header for the exact
> out-parameter shape before writing these.

### Section C — Tier C, BEHAVIORAL (values that must actually move)

Read/write harness-owned storage keyed by channel.

**Output setters — store the value where the harness/bridge can observe it:**

| Symbol |
|--------|
| `x_out_set_digital` |
| `x_out_set_duty_cycle` |
| `x_out_set_frequency` |
| `x_out_set_current_setpoint` |
| `x_out_set_circuit` |
| `x_out_set_dither` |
| `x_out_set_current_filter` |
| `x_out_set_control_parameters` |
| `x_out_pid_parameters` |
| `x_out_cc_set_dither` |

**Input getters — return a value the harness can script:**

| Symbol |
|--------|
| `x_in_get_digital_debounced` |
| `x_in_get_voltage_raw` |
| `x_in_get_current_raw` |
| `x_in_get_frequency` |

---

## Group L — the LIGHTING PoC critical subset (~6 symbols)

Lighting drives only DIGITAL outputs (HEADLIGHTS / WORKLIGHTS / TAILLIGHTS are
all `OT_DIGITAL`) and reads its button via the CAN device-mirror
(`gt_can_devs.t_buttonPanel`), NOT via the analog input path. So lighting's
actual path through this BIOS surface is:

| Symbol | Tier | Role in lighting path |
|--------|------|-----------------------|
| `x_out_digital_init` | B (init) | called at output-handler init |
| `x_out_set_digital` | C (setter) | carries light on/off value — THE observable one |
| `x_out_digital_diag` | B (diag) | fault check lighting gates on |
| `x_out_get_active_faults` | B (diag) | fault status lighting reads |
| `x_out_client_await_allocations` | A (inert) | called at startup regardless |
| `x_in_client_await_allocations` | A (inert) | called at startup regardless |

If the first host build includes ONLY lighting + I/O HAL + harness, the linker
will only demand these ~6; the other 25 appear as undefined ONLY when the
modules that call them are compiled in.

---

## Two options — choose one

- **Option 1 — lighting subset first (Group L, ~6 symbols).** Fastest path to a
  linking, running lighting demo. Backfill the rest when other modules are added
  to the host build. Recommended if a working demo soon is the priority.
- **Option 2 — all 31 now.** Stub the full surface. Marginal cost per symbol is
  small (each is a few lines of a known pattern), and the stub file is then
  "done" — any Agv module can join the host build later with no return trip.
  Recommended for a finished surface.

---

## Verification loop (run in PowerShell after writing stubs)

1. Compile the stub file to an object; compile the in-scope build set.
2. Link the `.dll`.
3. `nm` the objects (or read the link map, `-Wl,-Map=harness.map`) and check for
   remaining ` U ` (undefined) entries.
   - **Zero unresolved** = symbol set closed; done.
   - **A new ` U ` appeared** = a stub went too deep and called back into the
     vendor surface. Make that stub shallower (bottom out in harness storage or
     a constant).
4. Confirm the `.dll` export table (`objdump -p harness.dll`) shows the intended
   `harness_*` accessors and nothing leaked.

## Definition of done (this file)

1. Stub file compiles clean under MinGW-w64 via PowerShell, real vendor headers
   included, zero signature-mismatch warnings.
2. The chosen symbol set (Group L or all 31) links with zero unresolved
   BIOS symbols.
3. Tier-A symbols are visibly inert with explanatory comments.
4. Tier-C accessors read/write harness-owned storage (not hardware, not vendor).

## Report back (feeds parent project A5.3)

Record: any Category-1 header breakage actually hit (expected: none), the final
closed symbol list, which stubs needed out-param population, and any symbol that
forced a stub deeper than "return constant / touch harness memory." This
discovery is itself a deliverable — it maps the application's logic/hardware
entanglement.
