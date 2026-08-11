"""
dll_iface.py  (lighting_control fixture)

Thin wrapper around the host DLL. This is the proven core of test_roundtrip.py
(persistent ctypes handle, harness_init once, set -> step -> get) lifted behind a
small object so the WebSocket layer never touches ctypes directly.

ctypes is used here because it is what the Seam-2 round-trip proof used
(2026-08-11). Production target is cffi API-mode; when that swap happens it
happens HERE and nowhere else — ws_server.py and bridge.py bind to this class,
not to ctypes.

THE STEP SEAM
    drive_once(tag, value) does exactly one  set -> step -> get  cycle and returns
    the full OUTPUT snapshot. Today its only caller is the WebSocket message
    handler (step-per-message). When a ticked fixture (e.g. a free-running
    counter) is added later, a timer task calls step_and_read() on an interval —
    an ADDITIONAL trigger reusing the same primitives, not a rewrite. Keeping the
    step behind this one seam is what makes that a purely additive change.

    Steps happen when this is called, never on a clock inside here — the
    logic-only / no-real-time guarantee stays intact by construction.
"""

from __future__ import annotations

import ctypes
import os
import threading

import signal_map


class HarnessError(RuntimeError):
    """Raised when the DLL reports a non-zero rc or a binding is missing."""


class HarnessDLL:
    """Persistent-handle wrapper over one fixture DLL."""

    def __init__(self, dll_path: str):
        if not os.path.exists(dll_path):
            raise HarnessError(f"DLL not found: {dll_path}")
        self._path = dll_path
        # Single lock: the DLL holds process-wide mutable state (gt_svg /
        # gt_can_devs). set -> step -> get must be atomic w.r.t. other callers,
        # so every cycle takes this lock. Single-threaded today, but the lock
        # makes the future tick task safe to add without a redesign.
        self._lock = threading.Lock()
        self._dll = ctypes.CDLL(dll_path)
        self._bind()
        self._inited = False

    # -- binding -------------------------------------------------------------
    def _bind(self) -> None:
        """Declare signatures for the four exports, per fixture_accessors.c."""
        d = self._dll
        try:
            d.harness_init.restype = ctypes.c_int16
            d.harness_init.argtypes = []
            d.harness_step.restype = ctypes.c_int16
            d.harness_step.argtypes = []
        except AttributeError as exc:
            raise HarnessError(f"DLL missing core export: {exc}") from None

        # Per-signal accessors, bound by the tag-keyed names from signal_map.
        for sig in signal_map.INPUTS:
            fn = getattr(d, sig.setter, None)
            if fn is None:
                raise HarnessError(f"DLL missing setter export: {sig.setter}")
            fn.argtypes = [self._ctype(sig.width)]
            fn.restype = None
        for sig in signal_map.OUTPUTS:
            fn = getattr(d, sig.getter, None)
            if fn is None:
                raise HarnessError(f"DLL missing getter export: {sig.getter}")
            fn.argtypes = []
            fn.restype = self._ctype(sig.width)

    @staticmethod
    def _ctype(width: int):
        return {1: ctypes.c_uint8, 2: ctypes.c_uint16, 4: ctypes.c_uint32}[width]

    # -- lifecycle -----------------------------------------------------------
    def init(self) -> None:
        """Call harness_init exactly once; raise if it does not return 0."""
        with self._lock:
            if self._inited:
                return
            rc = self._dll.harness_init()
            if rc != 0:
                raise HarnessError(f"harness_init returned {rc}, expected 0")
            self._inited = True

    # -- primitives (the tick task will reuse these) -------------------------
    def _set(self, tag: str, value: int) -> None:
        sig = signal_map.get(tag)
        if sig.direction != signal_map.IN:
            raise HarnessError(f"{tag!r} is not an input; cannot set it")
        getattr(self._dll, sig.setter)(value & ((1 << (8 * sig.width)) - 1))

    def _get(self, tag: str) -> int:
        sig = signal_map.get(tag)
        if sig.direction != signal_map.OUT:
            raise HarnessError(f"{tag!r} is not an output; cannot get it")
        return int(getattr(self._dll, sig.getter)())

    def _step(self) -> None:
        rc = self._dll.harness_step()
        if rc != 0:
            raise HarnessError(f"harness_step returned {rc}, expected 0")

    def read_outputs(self) -> dict[str, int]:
        """Snapshot every OUT signal's current raw value (no step)."""
        return {s.tag: self._get(s.tag) for s in signal_map.OUTPUTS}

    def step_and_read(self) -> dict[str, int]:
        """One step, then snapshot outputs. The tick task's entry point."""
        with self._lock:
            if not self._inited:
                raise HarnessError("step before init()")
            self._step()
            return {s.tag: self._get(s.tag) for s in signal_map.OUTPUTS}

    # -- the message-driven seam ---------------------------------------------
    def drive_once(self, tag: str, value: int) -> dict[str, int]:
        """
        Apply one input change, step once, return the full output snapshot.
        This is set -> step -> get, atomic under the lock. The WebSocket handler
        calls exactly this per inbound state message.
        """
        with self._lock:
            if not self._inited:
                raise HarnessError("drive_once before init()")
            self._set(tag, value)
            self._step()
            return {s.tag: self._get(s.tag) for s in signal_map.OUTPUTS}
