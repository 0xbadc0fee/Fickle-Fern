"""
test_roundtrip.py  (lighting_control fixture — PROOF STAGE)

Minimal Seam-2 round-trip proof for the host DLL. ctypes is used here for the
proof (production target is cffi API-mode); the point is only to prove the
persistent-handle DLL + Seam-2 transfer work together on a real static build.

Run under Windows Python (AppData/.../Python312), NOT the MinGW shell.

    python test_roundtrip.py

Expects lighting_control.dll in the same folder (built per PROOF_NOTES.md).
"""

import ctypes
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
DLL_PATH = os.path.join(HERE, "lighting_control.dll")


def main() -> int:
    if not os.path.exists(DLL_PATH):
        print(f"FAIL: DLL not found at {DLL_PATH} — build it first (PROOF_NOTES.md).")
        return 1

    # Persistent handle — the key delta from PROTO-0's process model. The DLL
    # stays loaded across all calls, so gt_svg / gt_can_devs / registration state
    # persist between harness_step() calls (exactly what the bridge needs).
    dll = ctypes.CDLL(DLL_PATH)

    # Declare signatures (RAW uint8 across the seam).
    dll.harness_init.restype = ctypes.c_int16
    dll.harness_step.restype = ctypes.c_int16
    dll.harness_set_button04_state.argtypes = [ctypes.c_uint8]
    dll.harness_set_button04_state.restype = None
    dll.harness_get_button04_lights.restype = ctypes.c_uint8

    # 1. init exactly once
    rc = dll.harness_init()
    print(f"harness_init() -> {rc}")
    if rc != 0:
        print("FAIL: harness_init did not return C_NO_ERR (0).")
        return 1

    # baseline: button not pressed, step, read lights
    dll.harness_set_button04_state(0)
    dll.harness_step()
    lights_released = dll.harness_get_button04_lights()
    print(f"released: lights = 0x{lights_released:02X}")

    # 2-4. press the button, step, read the decoded lights byte back
    dll.harness_set_button04_state(1)
    dll.harness_step()
    lights_pressed = dll.harness_get_button04_lights()
    print(f"pressed:  lights = 0x{lights_pressed:02X}")

    # 5. assert the logic responded to the press.
    #    NOTE: the exact expected value depends on lighting_control's state
    #    machine (toggle vs momentary, which color bits). For the PROOF the
    #    load-bearing claim is only that the value TRANSITED the full path and
    #    the logic CHANGED it in response to input. Tighten this assertion once
    #    the lighting logic's expected output is confirmed against the source.
    if lights_pressed == lights_released:
        print("WARN: lights unchanged between released/pressed.")
        print("      Either the logic is momentary and needs a different probe,")
        print("      or the signal did not transit. Inspect before declaring pass.")
        return 2

    print("PASS: signal transited bridge -> gt_svg -> gt_can_devs -> logic -> gt_svg -> bridge.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
