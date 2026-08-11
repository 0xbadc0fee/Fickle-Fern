"""
ws_server.py  (lighting_control fixture)

The WebSocket transport. Localhost only. Speaks a tiny tag-keyed JSON protocol
to the browser SVG and drives the DLL one step per inbound state message.

WIRE FORMAT (JSON text frames)
    Browser -> bridge (input):
        {"type": "set", "signal": "button04_state", "value": 1}

    Bridge -> browser (output snapshot), sent after every drive and once on
    connect so a fresh page paints correct initial state:
        {"type": "state", "signals": {"button04_lights": 1}}

    Bridge -> browser (error, non-fatal):
        {"type": "error", "message": "..."}

    Values are RAW integers (no scaling — scaling is a later, explicit bridge
    concern). "signals" in a state frame carries every OUT signal's current byte.

STEP MODEL
    One inbound "set" -> one HarnessDLL.drive_once() -> one "state" broadcast.
    step-per-message, matching the round-trip proof and the edge-triggered logic.
    No clock in this file. A future ticked fixture adds a separate timer task
    that calls harness.step_and_read() and broadcasts — this loop is untouched.
"""

from __future__ import annotations

import asyncio
import json

import websockets  # pip install websockets

import signal_map
from dll_iface import HarnessDLL, HarnessError


class BridgeServer:
    def __init__(self, harness: HarnessDLL, host: str = "127.0.0.1", port: int = 8765):
        self._harness = harness
        self._host = host
        self._port = port
        self._clients: set = set()

    # -- outbound helpers ----------------------------------------------------
    @staticmethod
    def _state_frame(snapshot: dict[str, int]) -> str:
        return json.dumps({"type": "state", "signals": snapshot})

    @staticmethod
    def _error_frame(message: str) -> str:
        return json.dumps({"type": "error", "message": message})

    async def _broadcast(self, frame: str) -> None:
        if not self._clients:
            return
        stale = []
        for ws in self._clients:
            try:
                await ws.send(frame)
            except websockets.ConnectionClosed:
                stale.append(ws)
        for ws in stale:
            self._clients.discard(ws)

    # -- inbound handling ----------------------------------------------------
    async def _handle_message(self, ws, raw: str) -> None:
        try:
            msg = json.loads(raw)
        except json.JSONDecodeError:
            await ws.send(self._error_frame("malformed JSON"))
            return

        if msg.get("type") != "set":
            await ws.send(self._error_frame(f"unsupported type: {msg.get('type')!r}"))
            return

        tag = msg.get("signal")
        value = msg.get("value")
        if not isinstance(tag, str) or not isinstance(value, int):
            await ws.send(self._error_frame("set requires string 'signal' and int 'value'"))
            return

        try:
            sig = signal_map.get(tag)
        except signal_map.UnknownSignal as exc:
            await ws.send(self._error_frame(str(exc)))
            return
        if sig.direction != signal_map.IN:
            await ws.send(self._error_frame(f"{tag!r} is an output; cannot set"))
            return

        # THE STEP: set -> step -> get, off the event loop so a slow DLL call
        # never stalls the socket. drive_once is internally locked.
        try:
            snapshot = await asyncio.to_thread(self._harness.drive_once, tag, value)
        except HarnessError as exc:
            await ws.send(self._error_frame(f"harness: {exc}"))
            return

        await self._broadcast(self._state_frame(snapshot))

    async def _serve_client(self, ws) -> None:
        self._clients.add(ws)
        try:
            # Paint initial state so a fresh page is correct before any input.
            snapshot = await asyncio.to_thread(self._harness.read_outputs)
            await ws.send(self._state_frame(snapshot))
            async for raw in ws:
                await self._handle_message(ws, raw)
        except websockets.ConnectionClosed:
            pass
        finally:
            self._clients.discard(ws)

    async def run(self) -> None:
        async with websockets.serve(self._serve_client, self._host, self._port):
            print(f"bridge: ws://{self._host}:{self._port}  ({len(signal_map.SIGNALS)} signals)")
            await asyncio.Future()  # run forever
