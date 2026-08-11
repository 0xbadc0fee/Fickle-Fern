# KNOWN_GOOD — Seam-2 `lighting_control` Demo

**Tag:** `lighting_control-host_side_test-PASS`
**Commit:** `2feb58fdf6bd76ad7a1c30331c3ebf61ce4ac65a`

This is the confirmed-working recipe for the Seam-2 `lighting_control`
demo: a host-built DLL driving real application logic (`update_lightControl`),
no physical ECU attached, with a browser SVG as the operator panel.

## Recipe

```powershell
# 0. Check out the known-good state
git checkout lighting_control-host_side_test-PASS

# 1. Build (MinGW-w64, PowerShell, PATH must include the MSYS2 mingw64 bin dir)
$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH
cd ESX_4CM_A\appl_core\src

$HARNESS = "Testing\harness"
$FIX     = "Testing\Fixtures\lighting_control"
$includes = @(
  "-I$HARNESS","-I$FIX","-I..\libs\bios","-I.","-IAgvChassis",
  "-I..\libs\opensyde_j1939_handler","-I..\libs\lwip\include","-I..\libs\opensyde_server",
  "-Iopensyde","-Iopensyde\j1939","-Iopensyde\generated",
  "-IHAL","-IHAL\STW_4CM_HAL","-ISystem","-ISystem\IO","-ISystem\CAN","-ISystem\CAN\Devices",
  "-ISystem\Ethernet","-ISystem\NVM","-IAgvWork","-IAgvDiag","-IAgvHelper","-ITesting"
)
$sources = @(
  "$HARNESS\harness_api.c","$HARNESS\harness_svg_copy.c","$HARNESS\harness_can_devs.c","$HARNESS\harness_bios_stubs.c",
  "$FIX\fixture_signals.c","$FIX\fixture_accessors.c",
  "System\IO\hw_inputs.c","System\IO\hw_outputs.c","HAL\input_handler_lib.c","HAL\output_handler_lib.c","AgvChassis\lighting_control.c"
)
x86_64-w64-mingw32-gcc -shared -o "$FIX\lighting_control.dll" -DSVG_HARNESS `
  -static -static-libgcc -Wl,--exclude-all-symbols $includes $sources

# 2. Output is already named/placed correctly by -o above:
#      ESX_4CM_A/appl_core/src/Testing/Fixtures/lighting_control/lighting_control.dll
#    (bridge.py defaults to this exact filename beside itself — don't rename it.)

# 3. Bridge folder
cd Testing\Fixtures\lighting_control
pip install websockets   # first time only

# 4. Run the bridge (native Windows Python, NOT the MinGW shell)
python bridge.py
# -> "bridge: loaded lighting_control.dll, harness_init() -> 0"
# -> "bridge: ws://127.0.0.1:8765  (2 signals)"

# 5. Open index.html (same folder) in a browser — connects to ws://127.0.0.1:8765
```

## Confirm

Press the button04 keypad element in the page. The `button04_lights` readout
flips:

```
released: 00000001
pressed:  00010000
```

That's the full path proven live: **browser SVG → WebSocket → `gt_svg` →
`gt_can_devs` → `update_lightControl` → `gt_can_devs` → `gt_svg` →
WebSocket → browser SVG** — real application logic, no ECU.

## If it doesn't reproduce

- `lighting_control.dll` not beside `bridge.py`, or built without
  `-static -static-libgcc` (loads with `OSError 0x57`).
- `python` resolves to the wrong interpreter — must be native Windows
  Python (`AppData\...\Python312`), not an MSYS2/MinGW one.
- Build run from a shell missing `C:\msys64\mingw64\bin` on `PATH` —
  `cc1.exe` exits 1 with no diagnostic.

See `reqs/EDR_seam2_lighting_control.md` for the design rationale and the
first (ctypes-only) round-trip proof this demo builds on.
