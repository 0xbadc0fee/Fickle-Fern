"""
signal_map.py  (lighting_control fixture)

The single tag-keyed registry of signals that cross the bridge<->DLL seam. Every
other bridge module keys off these tag strings, and the tags are IDENTICAL to:
  - the friendly-name tag in fixture.def (VAR_ASSIGN arg 1),
  - the exported accessor names (harness_set_<tag> / harness_get_<tag>),
  - the SVG data-signal="<tag>" attributes.
One vocabulary, end to end. Do not introduce a second naming scheme here.

Direction is from the BRIDGE's point of view:
  IN  : SVG -> bridge -> DLL   (bridge WRITES via harness_set_<tag>)   [SVG2CNTRL]
  OUT : DLL -> bridge -> SVG   (bridge READS  via harness_get_<tag>)   [CNTRL2SVG]

Adding a signal = add one row here, one .def row, one accessor pair, one SVG
data-signal. Keep all four in sync by hand (that is the locked project decision).
"""

from __future__ import annotations

from dataclasses import dataclass


IN = "in"    # SVG -> DLL : bridge writes with harness_set_<tag>
OUT = "out"  # DLL -> SVG : bridge reads  with harness_get_<tag>


@dataclass(frozen=True)
class Signal:
    tag: str        # friendly-name key, shared across .def / accessor / SVG
    direction: str  # IN or OUT (bridge's perspective)
    width: int      # byte width of the raw value across the seam (1 = uint8)

    @property
    def setter(self) -> str:
        return f"harness_set_{self.tag}"

    @property
    def getter(self) -> str:
        return f"harness_get_{self.tag}"


# The lighting_control fixture's signal set. Order mirrors fixture.def rows.
SIGNALS: tuple[Signal, ...] = (
    Signal(tag="button04_state", direction=IN, width=1),
    Signal(tag="button04_lights", direction=OUT, width=1),
)

# Fast lookups.
BY_TAG: dict[str, Signal] = {s.tag: s for s in SIGNALS}
INPUTS: tuple[Signal, ...] = tuple(s for s in SIGNALS if s.direction == IN)
OUTPUTS: tuple[Signal, ...] = tuple(s for s in SIGNALS if s.direction == OUT)


class UnknownSignal(ValueError):
    """Raised for a tag not in the registry. ValueError (not KeyError) so its
    str() is the plain message, without KeyError's added quoting."""


def get(tag: str) -> Signal:
    """Look up a signal by tag, or raise a clear error naming the bad tag."""
    try:
        return BY_TAG[tag]
    except KeyError:
        known = ", ".join(sorted(BY_TAG))
        raise UnknownSignal(
            f"unknown signal tag {tag!r}; known tags: {known}"
        ) from None
