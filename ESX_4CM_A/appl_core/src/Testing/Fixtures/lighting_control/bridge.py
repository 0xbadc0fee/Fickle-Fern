"""
bridge.py  (lighting_control fixture) — entry point.

Wires the proven DLL core to the WebSocket transport:
    load DLL -> harness_init() once -> serve WebSocket (step-per-message).

Run under native Windows Python (AppData/.../Python312), NOT the MinGW shell.
The DLL must be the static-linked build (-static -static-libgcc) so it loads
without external runtime DLLs.

    python bridge.py [path\\to\\lighting_control.dll]

Defaults to lighting_control.dll beside this file.
"""

from __future__ import annotations

import asyncio
import os
import sys

from dll_iface import HarnessDLL, HarnessError
from ws_server import BridgeServer

DEFAULT_DLL = "lighting_control.dll"


def main(argv: list[str]) -> int:
    here = os.path.dirname(os.path.abspath(__file__))
    dll_path = argv[1] if len(argv) > 1 else os.path.join(here, DEFAULT_DLL)

    try:
        harness = HarnessDLL(dll_path)
        harness.init()
    except HarnessError as exc:
        print(f"FAIL: {exc}")
        return 1

    print(f"bridge: loaded {os.path.basename(dll_path)}, harness_init() -> 0")
    server = BridgeServer(harness)
    try:
        asyncio.run(server.run())
    except KeyboardInterrupt:
        print("\nbridge: shutdown")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
