---
title: Python Bridge v2 — Design Spec (CNTRL ⇄ Bridge ⇄ SVG, 2 RAW signals)
project: Silent-Sagebrush / Fickle-Fern
target_repo: 0xbadc0fee/Fickle-Fern
author: Silas Curfman (STW Technic, LP)
date: 2026-08-10
status: spec-for-implementation
doc_type: engineering-decision-record
audience: [claude-code, human]
supersedes: host_dll_poc (throwaway ctypes feasibility check)
tags: [python-bridge, dll, cffi, svg, harness, raw-signal, gt_can_devs, gt_svg]
---

# Python Bridge v2 — Design Spec

> **TL;DR.** Replace the throwaway `host_dll_poc` ctypes probe with a small,
> production-shaped bridge that carries exactly **two RAW `uint8` signals** across
> the DLL seam: one **CNTRL→SVG** (`u8_b4_lights`) and one **SVG→CNTRL**
> (`u8_b4_state`). The bridge drives `gt_svg` fields through named DLL accessors,
> pumps the harness update functions, and exchanges signal values with a
> browser-rendered SVG over a WebSocket. **No scaling, no DBC, no cantools** —
> `VAR_ASSIGN` in the seam is a plain copy, so both signals stay raw end-to-end.

---

## 1. Why v2 (what the throwaway proved and what it lacked)

The `host_dll_poc` (`hello.dll` with generic `set_value` / `get_value`, ctypes,
static-linked with `-static -static-libgcc`) proved three things:

1. A MinGW-built DLL loads and round-trips a value into native Windows Python.
2. `-static -static-libgcc` resolves the `OSError 0x57` runtime-DLL failure.
3. The transport concept (C value ⇄ Python ⇄ SVG) closes — validated end-to-end
   by **PROTO-0** (counter.exe ↔ WebSocket ↔ browser SVG).

What it deliberately did **not** do, and what v2 must do:

- Bind to the **real seam** (`gt_svg` / `gt_can_devs`), not a toy global.
- Expose **named, signal-specific** accessors instead of one opaque address.
- Pump the harness's own `update_harnessInputs()` / `update_harnessOutputs()`
  so the seam is exercised exactly as on-target.
- Use **cffi (API mode)** as the production binding (ctypes was PoC-only).

---

## 2. The seam this bridge binds to (authoritative — from repo)

From `ESX_4CM_A/appl_core/src/Testing/`:

**`svg_in_map_buttonPanel.def`** (SVG → CNTRL):
```
SVG2CNTRL(button04_state, gt_can_devs.t_buttonPanel.u8_b4_state, gt_svg.u8_svgKeyPad01_b4_state)
```

**`svg_out_map_buttonPanel.def`** (CNTRL → SVG):
```
CNTRL2SVG(button04_lights, gt_can_devs.t_buttonPanel.u8_b4_lights, gt_svg.u8_svgKeypad01_b4_lights)
```

**`SPN_definitions.h`** — the row primitive is a raw copy:
```c
#define VAR_ASSIGN(tar_value, src_value) do { (tar_value) = (src_value); } while (0)
```

**Harness contract** (`harness_svg_buttonPanel.c/.h`):
- `gt_svg` (type `T_HarnessSignalsButtonPanel`) is **harness-owned**.
- `gt_can_devs` (type `T_CANDevices`) is **defined in the harness for host builds**
  (its real definition in `can_handler_lib.c` is excluded — TriCore-only surface).
- `sint16 update_harnessInputs(void)` copies `gt_svg → gt_can_devs` (runs SVG2CNTRL rows).
- `sint16 update_harnessOutputs(void)` copies `gt_can_devs → gt_svg` (runs CNTRL2SVG rows).

> **Note for Claude Code:** the `.def` field spelling is inconsistent in the repo
> — inbound uses `u8_svgKeyPad01_b4_state` (capital P), outbound uses
> `u8_svgKeypad01_b4_lights` (lower p). Also, the two `gt_svg` members named in
> the `.def` files are **not yet declared** in the current
> `T_HarnessSignalsButtonPanel` struct (which presently only has
> `u8_svgButtonPanelB1` / `u8_svgButtonPanelLED1`). Reconcile these names as part
> of the C-side work in §4 before building — the bridge binds to whatever the
> struct finally declares. Do not invent scaling.

Both signals are `uint8`, RAW. That is the entire signal surface for v2.

---

## 3. Architecture

```
  Browser (Chromium, localhost)                 Native Windows Python 3.12
  ┌────────────────────────────┐   WebSocket    ┌─────────────────────────────┐
  │  buttonPanel.svg           │  JSON frames   │  svg_bridge/                │
  │  data-signal="button04_..."│ <────────────> │   ws_server.py  (asyncio)   │
  │  pointerdown / pointerup   │                │   dll_iface.py  (cffi API)  │
  │  <rect id> LED fill        │                │   signal_map.py (2 rows)    │
  └────────────────────────────┘                │   bridge.py     (loop)      │
                                                 └──────────────┬──────────────┘
                                                                │ cffi (C ABI)
                                                 ┌──────────────▼──────────────┐
                                                 │  harness_svg.dll (MinGW)    │
                                                 │   gt_svg  ⇄  gt_can_devs    │
                                                 │   update_harnessInputs()    │
                                                 │   update_harnessOutputs()   │
                                                 │   + exported accessors      │
                                                 └─────────────────────────────┘
```

One process owns the DLL. The WebSocket server and the DLL-pump run in the same
asyncio loop; no cross-process shared memory.

---

## 4. C-side work (harness DLL surface — Approach B accessors)

Add an exported accessor surface over `gt_svg` so the bridge never pokes raw
addresses. Keep these in a host-only translation unit (e.g. `harness_dll_api.c`),
compiled **only** into the DLL build.

```c
/* harness_dll_api.h — host/DLL-only export surface */
#ifdef _WIN32
#  define HARNESS_API __declspec(dllexport)
#else
#  define HARNESS_API
#endif

HARNESS_API void  harness_init(void);          /* zero gt_svg, define gt_can_devs */
HARNESS_API void  harness_step(void);          /* in→cntrl, (app logic), cntrl→out */

/* RAW uint8 signal accessors — one pair per signal, named by .def tag */
HARNESS_API void  harness_set_button04_state(unsigned char v);  /* SVG→CNTRL in  */
HARNESS_API unsigned char harness_get_button04_lights(void);    /* CNTRL→SVG out */
```

`harness_step()` sequence (matches on-target seam order):
1. `update_harnessInputs();`   /* gt_svg → gt_can_devs (SVG2CNTRL) */
2. *(call the application logic under test that consumes `u8_b4_state` and
   produces `u8_b4_lights`)*
3. `update_harnessOutputs();`  /* gt_can_devs → gt_svg (CNTRL2SVG) */

`harness_set_button04_state()` writes `gt_svg.<inbound member>`.
`harness_get_button04_lights()` reads `gt_svg.<outbound member>`.

> Keep the accessor names **1:1 with the `.def` friendly-name tags**
> (`button04_state`, `button04_lights`). That tag is the single source of truth
> shared by C, Python, and the SVG `data-signal` attribute.

**Build (MinGW via PowerShell — never Bash; Bash sandbox kills `cc1.exe`):**
```
x86_64-w64-mingw32-gcc -shared -static -static-libgcc -O0 -g `
  -o harness_svg.dll `
  harness_dll_api.c harness_svg_buttonPanel.c <app-logic-units> `
  -I<include dirs>
```
Exclude `can_handler_lib.c`. Confirm exports with `dumpbin /exports harness_svg.dll`.

---

## 5. Python bridge — module contract

Native Windows Python 3.12 from `AppData/.../Python312`, run from PowerShell.
Dependencies: `cffi`, `websockets`. cffi in **API mode** (`ffibuilder.set_source`
+ compiled shim, or `ffi.dlopen` with `cdef` for a pure-binding start; prefer the
compiled API-mode shim for production).

**`signal_map.py`** — the only place signal identity lives:
```python
# tag -> (direction, py getter/setter name on dll_iface)
SIGNALS = {
    "button04_state":  {"dir": "in",  "type": "u8"},   # SVG -> CNTRL
    "button04_lights": {"dir": "out", "type": "u8"},   # CNTRL -> SVG
}
```

**`dll_iface.py`** — thin cffi wrapper:
```python
class Harness:
    def init(self): ...                      # harness_init()
    def step(self): ...                       # harness_step()
    def set_in(self, tag, value: int): ...    # dispatch to harness_set_<tag>()
    def get_out(self, tag) -> int: ...        # dispatch to harness_get_<tag>()
```

**`bridge.py`** — asyncio loop, one tick:
1. Drain inbound WS frames → `set_in("button04_state", v)`.
2. `step()`.
3. `get_out("button04_lights")` → if changed, push outbound WS frame.
4. Sleep to a fixed poll interval (see §7).

**Wire format (JSON, both directions):**
```json
{ "signal": "button04_state",  "value": 1 }   // SVG -> bridge (inbound)
{ "signal": "button04_lights", "value": 1 }   // bridge -> SVG (outbound)
```
`value` is the RAW integer. The bridge does no math — `VAR_ASSIGN` is a copy, so
0..255 passes through unaltered in both directions.

---

## 6. Inbound edge behavior (pointerdown / pointerup)

`u8_b4_state` is a level, not an event. Rising/falling edges are produced **in the
SVG DOM**, not synthesized in Python:

- `pointerdown` on the button element → emit `{"signal":"button04_state","value":1}`
- `pointerup` (and `pointercancel`/`pointerleave` while pressed) → emit `value:0`

The bridge treats inbound as **last-value-wins level state**: it stores the most
recent value and applies it on every `set_in` before `step()`. No debounce, no
edge detection, no timers in the bridge — those are on-target concerns explicitly
deferred to hardware. If the application logic needs an edge, it derives it from
the level transition itself, exactly as it would from a real HMI.

---

## 7. Scope guardrails (architectural, not documentary)

The bridge must remain **incapable** of masquerading as a real-time target:

- **No timing fidelity.** Poll interval is fixed and cosmetic (suggest 20–50 ms
  for a responsive feel). The bridge must not expose or accept a "cycle time,"
  "task rate," or scheduler knob. `step()` runs when pumped; it models no clock.
- **No bus.** No CAN/J1939/DBC/cantools. The seam is `gt_svg`, downstream of
  transport. `can_handler_lib.c` stays excluded.
- **Level-only I/O.** Raw `uint8` copy in each direction. No factor/offset,
  no encode/decode.
- **Single seam.** The bridge only ever touches `gt_svg` via the exported
  accessors. It never reaches into `gt_can_devs` directly — that is the harness's
  job via the `.def` rows.

These constraints are what keep the tool honest as *logic-only* validation and
prevent it from being mistaken for SIL/HIL. (Avoid "SIL"/"HIL"/"vECU" language in
any user-facing naming here — "harness," "headless," "fixture," "virtual.")

---

## 8. Definition of done

1. `harness_svg.dll` builds via MinGW/PowerShell, `-static -static-libgcc`,
   with the four exports from §4 visible in `dumpbin /exports`.
2. cffi API-mode binding loads the DLL from native Windows Python.
3. `pointerdown` on the SVG button drives `button04_state=1` → `harness_step()`
   → `button04_lights` reflects the application's lighting logic → the SVG LED
   element updates fill; `pointerup` returns both to 0.
4. Round-trip observed live in Chromium at localhost with both signals.
5. No scaling code, no DBC, no cantools, no timing/scheduler surface present.

---

## 9. Suggested layout

```
ESX_4CM_A/appl_core/src/Testing/
  harness_svg_buttonPanel.c/.h      # existing (reconcile struct names, §2 note)
  harness_dll_api.c/.h              # NEW — export surface (§4)
  svg_in_map_buttonPanel.def        # existing
  svg_out_map_buttonPanel.def       # existing
  bridge/                           # NEW — Python v2
    signal_map.py
    dll_iface.py
    ws_server.py
    bridge.py
    build_dll.ps1                   # MinGW build script (PowerShell)
    README.md
  svg/
    buttonPanel.svg                 # authored per §handoff (see companion checklist)
```
