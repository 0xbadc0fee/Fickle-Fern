---
title: Claude Code Handoff — Seam-2 Lighting Fixture Round-Trip Proof
project: STW ESX.4CM-A — Silent-Sagebrush host-DLL harness
repo: Fickle-Fern (github.com/0xbadc0fee/Fickle-Fern)
author: Silas Curfman (STW Technic, LP)
date: 2026-08-11
status: PROOF-STAGE — build + prove round trip, do NOT canonize yet
build_variant: host (MinGW-w64 x86-64 -> .dll), gated by -DSVG_HARNESS
audience: [claude-code, human]
---

# Claude Code Handoff — Seam-2 Lighting Fixture Round-Trip Proof

## 0. Your job, in one sentence

Build the per-fixture host DLL for the `lighting_control` fixture, confirm its
exports, then run the Python round-trip test and report whether the signal
transits the full Seam-2 path. **Do not delete anything or promote anything to
"reference" until the round trip is green** — this is a proof, and Seam 1 stays
in the tree as fallback until Seam 2 is proven.

## 1. Locked design decisions (context — do not relitigate)

These are settled. Build to them; don't propose alternatives.

- **Seam 2** — the bridge talks to a `gt_svg` mirror struct; a `.def`-driven copy
  step moves values `gt_svg <-> gt_can_devs`. (The retired Seam 1 wrote
  `gt_can_devs` directly through per-signal accessors.)
- **Per-fixture DLL** — one fixture per DLL build. The build selects the fixture
  by pointing `-I` at that fixture's folder; the harness sources never change.
  Motivation: each DLL ships as a standalone test session to a remote tester
  (DLL + SVG + HTML + bridge), needing only Windows + Python + Chromium.
- **Hand-written accessors** — the DLL export surface is hand-written per fixture
  in `fixture_accessors.c`. It is NOT generated from a third `.def` macro
  expansion. (Rejected because a generated export surface is the one thing the
  cffi bridge binds to, and macro-generated function definitions are far harder
  to debug than the plain assignments the copy step generates.)
- **harness_step() proof body is MINIMAL** — for THIS proof it runs only the
  in-scope HAL + the single logic-under-test (`update_lightControl`). Expanding
  it to the full in-scope superloop is a SEPARATE later step; do not do it here.

## 2. Repo verification status — ALREADY DONE

The author manually verified all five repo-dependent assumptions on 2026-08-11.
**All five passed.** You do NOT need to re-verify these; treat them as ground truth:

1. `gt_can_devs.t_buttonPanel.u8_b4_state` and `.u8_b4_lights` — correct member
   paths in `can_device_definition.h`.
2. `init_hwInputs` / `update_hwInputs` / `init_hwOutputs` / `update_hwOutputs` —
   correct names in `hw_inputs.h` / `hw_outputs.h`.
3. `init_lightControl(&gt_can_devs)` and `update_lightControl(void)` — correct
   signatures in `lighting_control.h`.
4. `VAR_ASSIGN(dst, src)` => `dst = src;` — confirmed present in
   `SPN_definitions.h`.
5. BIOS stub set (Group L, ~6 symbols) present in `harness_bios_stubs.c` and
   matches what `hw_inputs.c` / `hw_outputs.c` reference.

If the build nonetheless fails on any of these symbols, STOP and report the exact
error rather than editing member names — a discrepancy would mean the file set
drifted from the repo, and the author needs to know.

## 3. File manifest

Two groups: MECHANISM (fixed across fixtures) and CONTENT (per fixture).

### Harness — MECHANISM (place under `Testing/harness/`)

| File | Role |
|---|---|
| `harness_api.c` / `.h` | Lifecycle (`harness_init`, `harness_step`) + export declarations. Fixture-agnostic except the accessor declaration block in the header. |
| `harness_svg_copy.c` / `.h` | `update_harnessInputs` / `update_harnessOutputs` — the `gt_svg <-> gt_can_devs` copy, X-macro over `fixture.def`. |
| `harness_can_devs.c` / `.h` | Defines `gt_can_devs` (host-build CAN-transport replacement). **Carry over from current tree.** |
| `harness_bios_stubs.c` | Tier-A inert `x_in_*`/`x_out_*` stubs (Group L). **Carry over from current tree.** |

### Fixture — CONTENT (place under `Testing/Fixtures/lighting_control/`)

| File | Role |
|---|---|
| `fixture.def` | Signal table — single source of truth. Combined (both directions). |
| `fixture_signals.h` / `.c` | `T_HarnessSignalsButtonPanel` type + the one `gt_svg` instance. |
| `fixture_accessors.c` | Hand-written export bodies (`harness_set_button04_state`, `harness_get_button04_lights`). |
| `test_roundtrip.py` | ctypes round-trip proof. |
| `fixture_lighting_control.svg` / `.html` | Input surface + browser layer. **Carry over from current tree** (not needed for the DLL round trip, but part of the fixture). |

The eight new files (harness ×4, fixture ×4 incl. test) are provided alongside
this handoff. The four "carry over" files already exist in the current `Testing/`
tree — reuse them, don't regenerate.

## 4. One behavioral change from the current tree — REVIEW, don't assume

`harness_svg_copy.c` defines BOTH `SVG2CNTRL` and `CNTRL2SVG` in each copy
function — one to the real `VAR_ASSIGN`, the other to empty:

```c
sint16 update_harnessInputs(void)
{
    sint16 s16_error = C_NO_ERR;
    #define SVG2CNTRL(name, CNTRL_VALUE, SVG_VALUE) VAR_ASSIGN((CNTRL_VALUE), (SVG_VALUE));
    #define CNTRL2SVG(name, CNTRL_VALUE, SVG_VALUE) /* not this direction */
    #include "fixture.def"
    #undef SVG2CNTRL
    #undef CNTRL2SVG
    return s16_error;
}
```

The current-tree `harness_svg_lightControlTest.c` defined only ONE macro per
function and left the other undefined. That worked only because it used SEPARATE
`svg_in_map` / `svg_out_map` `.def` files. This proof uses ONE combined
`fixture.def` (both row types), so both macros must be defined (one empty) or the
preprocessor errors on the other direction's rows. This is intentional and is the
reason the combined `.def` is viable. If you prefer the split-file approach
instead, that's a design change — flag it, don't silently switch.

## 5. Build (PowerShell ONLY)

**Hard environment constraints (known traps, do not deviate):**
- **PowerShell only** for all MinGW invocations. The Bash-tool sandbox silently
  kills `cc1.exe` — a real compile bare-exits 1 with no diagnostic.
- **`-static -static-libgcc` mandatory.** Without them the DLL throws
  `OSError 0x57` on load (missing MinGW runtime DLLs). These flags are what make
  the per-fixture DLL genuinely standalone/shippable — non-negotiable.
- **`-DSVG_HARNESS` mandatory.** Every harness/fixture `.c`/`.h` has an `#error`
  guard that fails the build without it. This gate is what keeps these artifacts
  out of the TriCore `.hex` build.

```powershell
# from repo root; fill in the two <path-to-...> header locations for the TSP + app headers
$FIX = "Testing/Fixtures/lighting_control"

x86_64-w64-mingw32-gcc `
  -shared -o "$FIX/lighting_control.dll" `
  -DSVG_HARNESS `
  -static -static-libgcc `
  -Wl,--exclude-all-symbols `
  -I Testing/harness `
  -I "$FIX" `
  -I <path-to-TSP-headers> `
  -I <path-to-app-headers> `
  Testing/harness/harness_api.c `
  Testing/harness/harness_svg_copy.c `
  Testing/harness/harness_can_devs.c `
  Testing/harness/harness_bios_stubs.c `
  "$FIX/fixture_signals.c" `
  "$FIX/fixture_accessors.c" `
  <in-scope app sources: hw_inputs.c hw_outputs.c lighting_control.c + their in-scope deps>
```

`-I "$FIX"` is the fixture selector: it makes `#include "fixture.def"` and
`#include "fixture_signals.h"` in the harness resolve to THIS fixture. That single
`-I` is the entire per-fixture-DLL mechanism.

## 6. Confirm exports (gate before running Python)

```powershell
x86_64-w64-mingw32-objdump -x "$FIX/lighting_control.dll" | Select-String "harness_"
```

Expect EXACTLY these four and nothing else:
- `harness_init`
- `harness_step`
- `harness_set_button04_state`
- `harness_get_button04_lights`

If more symbols leak, `-Wl,--exclude-all-symbols` isn't taking effect. If fewer,
a source file didn't compile in. Either way, stop and report before the Python step.

### Optional but recommended — see the macro expansion

```powershell
x86_64-w64-mingw32-gcc -E -P -DSVG_HARNESS -I Testing/harness -I "$FIX" -I <headers> `
  Testing/harness/harness_svg_copy.c | Select-String "u8_b4|u8_svgKeypad" -Context 2
```

You should SEE the `.def` rows as literal assignments with no macros present.
This confirms the compile-time expansion is doing what's intended.

## 7. Run the round-trip proof

Use **Windows Python** (`AppData/.../Python312`), NOT the MinGW shell. `.dll` must
be beside `test_roundtrip.py` (the build writes it into `$FIX`).

```powershell
cd $FIX
python test_roundtrip.py
```

The test:
1. `harness_init()` -> expect `0` (`C_NO_ERR`).
2. `harness_set_button04_state(0)` -> `harness_step()` -> read baseline lights.
3. `harness_set_button04_state(1)` -> `harness_step()` -> read lights again.
4. Asserts the lights byte CHANGED in response to the press.

**GREEN** = the full path works on a real static DLL:
`bridge -> gt_svg -> gt_can_devs -> update_lightControl -> gt_can_devs -> gt_svg -> bridge`.
This is the one genuinely unproven element — persistent-handle DLL + Seam-2
transfer together.

### Assertion caveat (expected, not a failure)

The test's pass condition is deliberately LOOSE: it proves the signal transited
and the logic changed it, NOT a specific expected value. It does not encode
whether button 4 is momentary or toggle, or which color bits fire, because that
depends on `lighting_control.c`'s state machine.

- If it returns **PASS** — the round trip is proven. Good.
- If it returns exit code **2 / "lights unchanged"** — do NOT assume failure yet.
  Read `lighting_control.c`'s state machine. If the logic is momentary
  (press-and-release within one step produces no persistent output), the probe
  needs adjusting (e.g. hold pressed across two steps, or read the output while
  still pressed). Report what the logic actually does and propose the corrected
  probe rather than editing the C.
- Any nonzero return from `harness_init` or a load error — report the exact error.

## 8. On PASS — the promote sequence (do these ONLY after green)

Do not start these until the round trip passes. When it does:

1. **Memorialize Seam 2** as the project seam — write an EDR (YAML frontmatter,
   TL;DR blockquote, decision table, the round-trip result as evidence).
2. **Retire Seam 1** — remove from the working tree (they remain in Git history):
   - `VERSION1_harness_api.c` / `.h`
   - `harness_svg_lightControlTest.c` / `.h` (BOTH copies: root + fixture folder)
   - `svg_in_map_buttonPanel.def` / `svg_out_map_buttonPanel.def`
   - the Seam-1 `harness_api.c` / `.h` currently in the tree (replaced by the
     Seam-2 versions in this handoff)
3. **Promote to reference** — resolve the `T_HarnessSignalsButtonPanel` ->
   `T_HarnessSignals` rename (so the type name doesn't imply "button panel" in a
   future elevator fixture), drop the PROOF-STAGE banners, mark it the copy-me
   template.
4. **Then (separate step)** — expand `harness_step()` to the full in-scope
   superloop body. Discover the divergence from `main.c` by LINK FAILURE:
   attempt the build with all in-scope `update_*()` calls, let undefined symbols
   (`force_canMessage`, `set_canMessage`, etc.) name themselves, and add Tier-A
   inert stubs to satisfy the linker. Stub to LINK, never to emulate — the moment
   a stub simulates bus/scheduler behavior, it has crossed into the deferred
   SIL/HIL scope.

## 9. Guardrails (apply throughout)

- **Logic-only, by construction.** Don't add CAN/LIN/ETH/RTOS/scheduler behavior
  to make anything "work." Those are deferred to bench/HIL by design.
- **No scaling in C.** Values cross the seam RAW; scaling is the Python bridge's
  job, later.
- **Stubs are test-only, never shipped.** Keep the real-vs-stub boundary bright.
- **Don't expand scope to fix a red test.** If the proof fails, report and propose
  — the likely fixes are a probe adjustment or a genuine build finding, not a
  design change.
