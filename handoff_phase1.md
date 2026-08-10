# Handoff — Phase 1 (Silent-Sagebrush host-DLL harness)

Status as of 2026-08-08. Read `CLAUDE.md` first — it's the authoritative
project brief and now also carries a "Findings to date" section with the
detailed results this doc summarizes. This file is just the fast-start
recap for a new session.

## What this project is

Fickle-Fern is normally a TriCore-only embedded build (STW ESX.4CM-A). We're
adding a second, parallel build variant: same application source, compiled by
MinGW-w64 (host, x86-64 Windows) into a `.dll`, gated by `-DSVG_HARNESS`, so
a future Python bridge can drive the app's LOGIC with no physical controller.
Logic-only, structurally enforced — see CLAUDE.md's hard scope constraints.

## Current code state

- **TriCore build:** untouched, still the working CMake build (`.hex` output).
  Do not regress it.
- **Host build:** **no CMake target or actual `.dll` link exists yet.**
  Everything done so far is manual `gcc.exe` object-file compilation used as
  a discovery probe, not a wired-up build.
- **Files confirmed to compile clean host-side** (zero Category-1
  TriCore-header/intrinsic breakage, real vendor headers untouched):
  `hw_outputs.c`, `hw_inputs.c`, `output_handler_lib.c`, `input_handler_lib.c`
  (all under `appl_core/src/System/IO` and `appl_core/src/HAL`).
- **`ESX_4CM_A/appl_core/src/Testing/harness_bios_stubs.c` — written and
  verified.** Implements all 34 real `x_out_*`/`x_in_*`/`x_msw_*`/`x_uext_*`
  BIOS stub symbols demanded by the four files above (Tier A inert / Tier B
  init+diag / Tier C behavioral, per `harness_bios_stubs_spec.md`). Verified
  via `nm` symbol-closure check: zero unresolved BIOS symbols across all five
  objects; only `strcmp` remains (stdlib, resolves at DLL link).
- **`ESX_4CM_A/appl_core/src/Testing/harness_bios_stubs_spec.md`** — the spec
  this was built from. Has two known inaccuracies (documented in CLAUDE.md's
  findings section): its 31-symbol count should be 34 (2 phantom symbol names
  that don't exist, 4 real `x_msw`/`x_uext` symbols it missed), and its claim
  that Bash-tool + gcc is broken and PowerShell is required is wrong — real
  cause was a PATH issue, fixed identically in both shells (see below). Worth
  correcting the doc itself if it gets reused.
- **Earlier, separate strand:** `harness_svg_buttonPanel.c/h` +
  `svg_out_map_buttonPanel.def` — pre-existing "SVG Harness step 1" work
  (TriCore-side, X-macro signal mapping for a button panel). Not part of the
  BIOS-stub work above; mentioned so it isn't mistaken for it.
- **Not yet probed for Category-1 breakage:** `alarm_handler_lib.c`,
  `nvm_handler_lib.c`, `can_handler_lib.c`, and everything outside the
  input/output HAL cluster. "Zero breakage so far" is scoped only to the four
  files listed above.

## Toolchain note (don't rediscover this)

`gcc` is not on `PATH` in Bash or PowerShell by default. The real binary is
MSYS2's `C:\msys64\mingw64\bin\gcc.exe`. **Invoking it by full path alone is
not enough** — `cc1.exe` depends on DLLs (libgmp/libmpfr/libmpc/libiconv/
libwinpthread/zlib) that live in `mingw64\bin`, so without that directory
actually on `PATH`, any real compile silently exits 1 with zero stdout/stderr
(only `--version`/`-dumpversion` work, since those skip cc1). Fix, either
shell, same result in both:
```
# Bash
export PATH="/c/msys64/mingw64/bin:$PATH"
# PowerShell
$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH
```

## Recommended next steps

1. **Probe remaining HAL/System files** the same way (`alarm_handler_lib.c`,
   `nvm_handler_lib.c`, `can_handler_lib.c`) to extend the Category-1 /
   symbol-inventory map before wiring a real build.
2. **Stand up the actual host CMake target** (MinGW toolchain file, `.dll`
   output, includes `harness_bios_stubs.c` + the four probed HAL files).
   Nothing like this exists yet — everything to date is discovery-only.
3. **Implement `harness_init()` / `harness_step()`** and the first hand-written
   accessor pair against `gt_can_devs` (`T_CANDevices`, declared in
   `can_handler_lib.c`), per CLAUDE.md's Bridge Interface (Approach B) —
   this is the repo's stated "definition of done" for this step and hasn't
   been started.
4. Once the `.dll` links, confirm the export table (`objdump -p`) shows only
   the intended `harness_*` accessors.
5. If `harness_bios_stubs_spec.md` gets reused/extended, fix the two doc
   inaccuracies noted above so they don't propagate further.
