# Handoff — Phase 1 (Silent-Sagebrush host-DLL harness)

Status as of 2026-08-10. Read `CLAUDE.md` first — it's the authoritative
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
- **`alarm_handler_lib.c`, `nvm_handler_lib.c`, `can_handler_lib.c` — now
  probed** (see CLAUDE.md's 2026-08-10 findings milestone for full detail):
  - `alarm_handler_lib.c` and `nvm_handler_lib.c` compile clean host-side,
    zero Category-1 breakage. Each pulls in one new out-of-scope symbol
    cluster needing Tier A stubs (J1939 lamp/DTC layer for alarm;
    openSYDE DataPool Handler for nvm — scope of the latter still undecided),
    plus `nvm_handler_lib.c` surfaces a new **in-scope** BIOS symbol family
    (`x_nvm_read`/`x_nvm_write`/`x_nvm_get_size`) not yet in
    `harness_bios_stubs.c`.
  - `can_handler_lib.c` hit a **new Category-1 break**: a hard
    `#error` GCC-version gate in `osy_dpa_trg_definitions.h` (via
    `osy_com_engine.h`), unrelated to the TriCore-intrinsic breakage pattern
    seen before. Bypassable with a macro-spoof probe trick only — not a real
    build solution. More importantly: **`gt_can_devs`, the confirmed accessor
    injection target, lives inside this file, which is entangled with the
    full CAN/J1939 comm engine** (not a clean HAL boundary like the I/O
    files). Getting the real global into the host DLL requires both the
    version-gate workaround and a nontrivial CAN/J1939 Tier A stub set — an
    open design question for step 3 below, not yet resolved.
  - Also confirmed: BIOS `x_*` naming alone isn't the scope test —
    `x_can_bus_init`/`x_osf_can_get_param` share the naming convention with
    the in-scope `x_in`/`x_out` surface but must be Tier A inert (CAN is
    out-of-scope), not Tier C.
- **Not yet probed for Category-1 breakage:** everything outside the
  input/output HAL cluster and these three files (e.g. anything under
  `AgvChassis`, `AgvWork`, `AgvDiag`, `AgvHMI`, `AgvHelper`, `System/Ethernet`,
  the wider `opensyde` tree).

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

1. ~~Probe remaining HAL/System files (`alarm_handler_lib.c`,
   `nvm_handler_lib.c`, `can_handler_lib.c`)~~ — **done, 2026-08-10.** See
   findings above / CLAUDE.md's 2026-08-10 milestone.
2. ~~Resolve the `can_handler_lib.c` / `gt_can_devs` open design question~~ —
   **decided, 2026-08-10.** The harness owns the `gt_can_devs` definition in
   a new file (`Testing/harness_can_devs.c`), including
   `can_device_definition.h` directly — never `can_handler_lib.h`, which
   carries the version gate even just for its `extern` declaration.
   `can_handler_lib.c` is excluded from the host build entirely. Full
   rationale (including why this isn't a disallowed shadow instance) is in
   CLAUDE.md's "Decision (2026-08-10)" section. **Not yet implemented** —
   this is a design decision, no code written yet.
3. ~~Write `Testing/harness_can_devs.c`~~ — **done, 2026-08-10.** Both
   `harness_can_devs.c` and `harness_can_devs.h` exist under
   `appl_core/src/Testing`, define/declare `T_CANDevices gt_can_devs;`, and
   never include `can_handler_lib.h` (the constraint that actually mattered
   — avoiding its version-gated include chain). Note: the file ended up
   including both `can_device_definition.h` and its own `harness_can_devs.h`
   (which itself only pulls `can_device_definition.h`), not
   `can_device_definition.h` alone as originally worded here — harmless,
   verified by compiling and by linking a probe consumer against it. Not
   yet wired into any CMake target (that's step 7).
4. ~~Add the new in-scope BIOS stub family~~ — **done, 2026-08-10.**
   `x_nvm_read`/`x_nvm_write`/`x_nvm_get_size` added to
   `harness_bios_stubs.c` as a new "Section D," Tier C, backed by a
   32 KiB harness-owned flat byte array. Verified: compiles clean
   (`-Wall -Wextra -Wdouble-promotion`, zero warnings), and a full
   undefined-vs-defined symbol closure across the whole in-scope host-build
   set (`hw_outputs.c`, `hw_inputs.c`, `output_handler_lib.c`,
   `input_handler_lib.c`, `alarm_handler_lib.c`, `nvm_handler_lib.c`,
   `harness_bios_stubs.c`, `harness_can_devs.c`) shows all three symbols
   fully resolved with no new collateral undefined symbols introduced.
   One correctness detail worth knowing for future stub work: the backing
   array is lazily initialized to `0xFF` (erased-EEPROM convention) on
   first access, not left zero-initialized — `nvm_handler_lib.c`'s
   `fault_nvm_init()` searches for an empty slot via the sentinel
   `u32_spn == 0xFFFFFFFF`, and a zero-initialized array would never match
   it, silently breaking fault persistence on every call. See CLAUDE.md's
   2026-08-10 findings for detail.
5. ~~Decide `alarm_handler_lib.c`'s place in the first host build pass.~~ —
   **decided, 2026-08-10: excluded from the first build.**
   `alarm_handler_lib.c` is NOT part of the first host `.dll` pass.
   **Deliberately deferred, not forgotten** — its J1939/DM1 Tier A stub
   work (`gat_DmDtcs`, `osy_com_j1939_dm1_lock_tx`/`_unlock_tx`,
   `osy_j1939_set_lamps`, all from `osy_app_j1939.c`) remains a known,
   scoped-out task to pick up whenever alarm-handling logic is actually
   needed for a harness scenario. Nothing to implement for this step right
   now.
6. ~~Resolve the open `osy_dph_*` scope question from `nvm_handler_lib.c`~~ —
   **deferred, 2026-08-10.** Not decided yet: whether
   `osy_dph_nvm_read_list`/`_write_list`/`_apply_data_set` are in-scope NVM
   persistence (Tier B/C) or out-of-scope openSYDE DataPool infrastructure
   (Tier A). Deliberately deferred, not forgotten. Direct consequence for
   step 7: since these three symbols remain genuinely unresolved,
   `nvm_handler_lib.c` is EXCLUDED from the first host `.dll`'s source list
   alongside `alarm_handler_lib.c` — it has no other consumer in the
   currently-included file set (only `alarm_handler_lib.c` calls
   `fault_nvm_init`/`fault_nvm_write`, and that file is already deferred),
   so nothing is lost by leaving it out for now. The `x_nvm_*` stubs from
   step 4 remain written, compiled-verified, and ready for whenever this
   file rejoins the build.
7. ~~Stand up the actual host CMake target~~ — **done, 2026-08-10.** New
   files: `ESX_4CM_A/cmake/host-toolchain.cmake` (MinGW toolchain file) and
   `ESX_4CM_A/host_harness/CMakeLists.txt` (a genuinely separate CMake
   project — never `add_subdirectory`'d from the TriCore top-level
   `CMakeLists.txt`). Builds `svg_harness.dll` from exactly the six files
   settled on above: `hw_outputs.c`, `hw_inputs.c`, `output_handler_lib.c`,
   `input_handler_lib.c`, `harness_bios_stubs.c`, `harness_can_devs.c`.
   `alarm_handler_lib.c`/`nvm_handler_lib.c`/`can_handler_lib.c` stay out per
   steps 2/5/6's decisions. Verified by actually running the build, not just
   authoring the files:
   ```
   cmake -S ESX_4CM_A/host_harness -B build-harness -G Ninja -DCMAKE_TOOLCHAIN_FILE=<absolute-path-to>/ESX_4CM_A/cmake/host-toolchain.cmake
   cmake --build build-harness
   ```
   Links clean, zero errors, `svg_harness.dll` produced.
   Confirmed via `git status` that no TriCore-side file
   (`appl_core/CMakeLists.txt`, the top-level `CMakeLists.txt`,
   `tricore-toolchain.cmake`) was touched.

   **Two real findings from actually running this, not just writing it:**
   - `CMAKE_TOOLCHAIN_FILE` given as a relative path failed to resolve
     ("Could not find toolchain file") even with a correct-looking relative
     path from the invocation directory — cmake's resolution base for that
     variable is inconsistent enough across versions/scenarios to not rely
     on. Fix: always pass an absolute path.
   - **MinGW's `ld` auto-exports every public symbol from a `-shared` build
     by default** (unlike MSVC, where export is opt-in) — the first
     successful link produced 114 exported symbols with zero
     `__declspec(dllexport)` anywhere in the source, including raw
     internal state (`gt_can_devs`, `tvo_headlights`, `tvi_wheel_speed`,
     every vendor BIOS stub). This would have directly violated CLAUDE.md's
     explicit "symbol export is OPT-IN" hard constraint had it shipped as
     the real config. Fixed with `-Wl,--exclude-all-symbols` in
     `target_link_options`; reverified the export table is now genuinely
     empty (`objdump -p` shows a zeroed Export Directory entry). Anyone
     reusing this CMake pattern elsewhere should know this flag is load
     -bearing, not decorative.
   - `.gitignore` updated: added `/build-harness/` (only `/build/` was
     previously covered).
8. ~~Implement `harness_init()` / `harness_step()` and the first
   hand-written accessor pair~~ — **done, 2026-08-10.** New files
   `ESX_4CM_A/appl_core/src/Testing/harness_api.c`/`.h`, added to
   `host_harness/CMakeLists.txt`. `harness_init()` zeroes `gt_can_devs`
   then calls `init_hwInputs()`/`init_hwOutputs()`; `harness_step()` calls
   `update_hwInputs()`/`update_hwOutputs()` — the same I/O HAL read/write
   pair `main.c`'s real cyclic loop runs each pass, minus everything still
   out of scope. First accessor pair:
   `harness_set_button1_state`/`harness_get_button1_state`, raw `uint8`
   read/write of `gt_can_devs.t_buttonPanel.u8_b1_state`. Verified with an
   actual ctypes smoke test against the built DLL (not committed to the
   repo, just a throwaway verification script) — init, set, get, and step
   all behave as expected.

   **Real finding, not just a style note: `harness_init()` is NOT safe to
   call twice per DLL load.** First draft claimed otherwise in a comment;
   the smoke test caught it. `init_hwInputs()`/`init_hwOutputs()` register
   each hardware channel via `add_hwInput()`/`add_hwOutput()` against a
   monotonically-growing static counter in `input_handler_lib.c`/
   `output_handler_lib.c` with no reset path anywhere in the application's
   own code — a second call re-registers the same hardware IDs, which the
   app's own duplicate-detection logic correctly rejects (confirmed:
   returns `C_INPUT_INIT_HW_FAIL`, -20). This mirrors the real controller's
   own assumption that init runs exactly once per boot — not a harness
   limitation, and not something to paper over by adding a reset mechanism
   to application logic that wasn't asked for. Fixed by correcting the
   claim (comment + this doc), not the code: `harness_init()` is one-time-
   per-load; a Python test suite needing a clean slate between cases must
   reload the DLL, not call `harness_init()` again against the same load.
9. ~~Confirm the export table shows only the intended `harness_*`
   accessors~~ — **done, 2026-08-10.** `objdump -p` on the rebuilt
   `svg_harness.dll` shows exactly four exports:
   `harness_get_button1_state`, `harness_init`,
   `harness_set_button1_state`, `harness_step` — nothing else, confirming
   `-Wl,--exclude-all-symbols` (step 7) is doing its job with real exports
   now present to test against.
10. If `harness_bios_stubs_spec.md` gets reused/extended, fix the two doc
    inaccuracies noted previously so they don't propagate further. It will
    also need a new section for the `x_nvm_*` symbol family discovered in
    the 2026-08-10 probe.
11. **Deferred, tracked but not urgent:** `throttle_control.c` will need a
    host-side shim for its `#include "can_handler_lib.h"` (same version
    gate) and Tier A stubs for `force_canMessage`/`set_canMessageActive`
    whenever it's pulled into the host build. Not needed for the current
    milestone.
