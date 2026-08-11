---
title: Seam 2 — gt_svg Mirror + Per-Fixture DLL (lighting_control Round-Trip Proof)
project: Silent-Sagebrush / Fickle-Fern
target_repo: 0xbadc0fee/Fickle-Fern
author: Silas Curfman (STW Technic, LP)
date: 2026-08-11
status: proven — Seam 2 is the project seam
doc_type: engineering-decision-record
audience: [claude-code, human]
supersedes: Seam 1 (svg_in_map_buttonPanel.def / svg_out_map_buttonPanel.def,
  harness_svg_lightControlTest.c/.h, VERSION1_harness_api.c/.h — retired from
  the working tree 2026-08-11, preserved in git history)
tags: [seam-2, gt_svg, gt_can_devs, host-dll, cffi, harness, per-fixture-dll,
  lighting_control, round-trip-proof]
---

# Seam 2 — gt_svg Mirror + Per-Fixture DLL

> **TL;DR.** The Python bridge no longer writes `gt_can_devs` directly through
> per-signal accessors (Seam 1). It now writes a small harness-owned mirror
> struct, `gt_svg`, and a `.def`-driven copy step
> (`update_harnessInputs`/`update_harnessOutputs`, X-macro over `fixture.def`)
> moves values `gt_svg <-> gt_can_devs` on either side of the logic-under-test.
> Each fixture is a standalone DLL, selected at build time by pointing `-I` at
> one fixture folder — the harness sources never change per fixture. Proven
> 2026-08-11 against `lighting_control`: a real static-linked MinGW DLL, a
> persistent ctypes handle, `harness_init() -> 0`, and a button04 press that
> transits the full path and visibly changes the decoded lights byte
> (`0x01 -> 0x10`). Seam 1 is retired from the working tree as of this proof.

---

## 1. Why Seam 2 (what Seam 1 lacked)

Seam 1 (`harness_set_<tag>`/`harness_get_<tag>` writing `gt_can_devs` fields
directly) proved the DLL-load + accessor-export mechanism, but every new
signal meant a hand-written accessor with no single source of truth for the
signal set, and no place to grow into a real "one fixture = one shippable test
session" model — the accessor surface and the signal table were the same
artifact.

Seam 2 splits that into two layers:
- **`gt_svg`** — the fixture's signal mirror, the only thing the accessors
  (and therefore the Python bridge) ever touch.
- **`fixture.def`** — the single source of truth for the signal set, expanded
  twice (input direction, output direction) by the harness's own copy
  functions via an X-macro. Adding a signal means adding one `.def` row and
  one hand-written accessor pair — never touching the copy mechanism.

## 2. Decisions locked by this proof

| Decision | Statement | Why |
|---|---|---|
| Seam | Bridge talks to `gt_svg`; `.def`-driven copy moves `gt_svg <-> gt_can_devs`. | Decouples the bridge's accessor surface from `gt_can_devs`'s real layout; the `.def` is the one file that has to match the real struct. |
| Per-fixture DLL | One fixture per DLL build, selected via `-I <fixture-folder>`. Harness sources are fixed across fixtures. | Each DLL ships standalone (DLL + SVG + HTML + bridge) to a remote tester needing only Windows + Python + Chromium. |
| Accessors | Hand-written in `fixture_accessors.c`, NOT generated from a third `.def` macro expansion. | The export surface is the contract the cffi bridge binds to; hand-written stays debuggable, a broken copy-paste fails loudly at compile time. |
| Combined `.def` | One `fixture.def` with both `SVG2CNTRL` and `CNTRL2SVG` rows; each copy function `#define`s one macro real, the other empty. | Lets one `.def` file serve both directions instead of two separate map files (the old `svg_in_map`/`svg_out_map` split). |
| `harness_step()` scope | Proof body is MINIMAL — in-scope HAL + the single logic-under-test only. Full in-scope superloop is a separate, later step. | Keeps the proof narrow; superloop expansion is meant to be discovered by link failure, not designed up front. |

## 3. Evidence — round-trip result (2026-08-11)

**Build** (MinGW-w64, `x86_64-w64-mingw32-gcc`, `-DSVG_HARNESS -static
-static-libgcc -Wl,--exclude-all-symbols`, 11 sources: 4 harness mechanism +
2 fixture content + `hw_inputs.c`/`hw_outputs.c`/`input_handler_lib.c`/
`output_handler_lib.c`/`lighting_control.c`): linked clean, exit code 0.

**Export gate** (`objdump -x lighting_control.dll`): exactly four exports —
`harness_init`, `harness_step`, `harness_set_button04_state`,
`harness_get_button04_lights`. Nothing else leaked; `--exclude-all-symbols`
holds under a real fixture build, not just the earlier empty-fixture case.

**Round-trip test** (`test_roundtrip.py`, native Windows Python 3.12,
persistent ctypes handle):

```
harness_init() -> 0
released: lights = 0x01
pressed:  lights = 0x10
PASS: signal transited bridge -> gt_svg -> gt_can_devs -> logic -> gt_svg -> bridge.
```

`update_lightControl`'s state machine is edge-triggered (toggles
`u8_light_mode` on a 0->1 transition of the button state). The test's
release-then-press sequence lands on that rising edge by construction, which
is why the LED byte changes on the very first press — `0x01` (mode OFF,
`RED_ON`) to `0x10` (mode HEAD_TAIL, `GREEN_ON`), read directly from the
source rather than assumed.

## 4. What changed in the working tree (2026-08-11)

- **Retired** (removed from working tree, preserved in git history):
  `VERSION1_harness_api.c/.h`, `harness_svg_lightControlTest.c/.h`,
  `svg_in_map_buttonPanel.def`, `svg_out_map_buttonPanel.def`.
- **Promoted to reference**: `Testing/harness/` (`harness_api.c/.h`,
  `harness_svg_copy.c/.h`, `harness_can_devs.c/.h`, `harness_bios_stubs.c`)
  and `Testing/Fixtures/lighting_control/` (`fixture.def`,
  `fixture_signals.c/.h`, `fixture_accessors.c`, `test_roundtrip.py`,
  `fixture_lighting_control.svg/.html`). PROOF-STAGE banners dropped;
  `T_HarnessSignalsButtonPanel` renamed to `T_HarnessSignals` (module-agnostic,
  doesn't imply "button panel" for a future fixture).
- **Not retired**: `harness_svg_buttonPanel.c/.h` — an older, independent
  button-panel bring-up artifact not covered by this proof's retire list. It
  defines its own separate `T_HarnessSignalsButtonPanel`/`gt_svg`/`gt_can_devs`
  and shares no header with the promoted files, so the rename above does not
  touch it. Left as-is; a candidate for cleanup, not addressed here.
- **Not done here (separate step, per the handoff)**: expanding
  `harness_step()` to the full in-scope superloop. To be discovered by link
  failure when the next fixture or scenario needs more than
  `update_lightControl`.

## 5. Copy-me template

`Testing/Fixtures/lighting_control/` is the reference fixture folder. To add a
new fixture: copy the folder, rewrite `fixture.def`'s rows against the new
module's `gt_can_devs` members, rewrite `fixture_signals.h`'s struct members
to match, write one accessor pair per `.def` row in `fixture_accessors.c`, and
point the build's `-I` at the new folder. `Testing/harness/` does not change.
