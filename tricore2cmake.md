# tricore2cmake: Converting HighTec TriCore Eclipse CDT Builds to CMake + Ninja/Make

## Purpose and scope

This document records findings from converting a three-core (`boot_core`, `appl_core`,
`safety_core`) Infineon AURIX TriCore embedded project from a HighTec-toolchain-based
Eclipse CDT / LogiCAD IDE build to a command-line CMake + Ninja/Make build. The goal was
byte-for-byte (or functionally provable) equivalent `.hex` output, plus per-target
`compile_commands.json` for LSP tooling (clangd/ccls in Neovim/Emacs).

These findings generalize to any Eclipse CDT "Managed Build" project using a HighTec
(or similar GCC-derivative) cross toolchain. Read this before attempting a similar
conversion; it documents both the mechanical recipe and the non-obvious pitfalls that
cost the most time.

---

## 1. Where the ground truth lives

An Eclipse CDT Managed Build project stores its *entire* compiler/linker invocation
recipe in `.cproject` (XML), one `<cconfiguration>` block per build configuration
(e.g. `appl_core_release`, `boot_core_release`). Do not guess flags from README files
or IDE screenshots — read `.cproject` directly. Key elements per configuration:

- `artifactName` / `artifactExtension` — output binary name/extension (e.g. `.elf`).
- `postbuildStep` — a raw shell command string (quoted for `cmd /C`) run after linking.
  This is where `objcopy`, signing tools, and result-folder copies live. **Always
  extract this verbatim** — it is the authoritative recipe for turning the linked
  ELF into the shippable artifact (hex, signed hex, etc.).
- Per compiler tool (`Cross GCC Compiler`):
  - `command` attribute on the `<tool>` element — full path to the actual compiler
    executable (e.g. `C:\HIGHTEC\toolchains\tricore\v4.9.2.0\bin\tricore-gcc`). This
    is the toolchain version/location ground truth.
  - `Other flags` option (`gnu.c.compiler.option.misc.other`) — usually contains the
    bulk of real flags (`-mcpu=`, `-std=`, warnings, section-splitting flags) as one
    string. Note this can overlap/duplicate flags implied by dropdown options
    (e.g. an explicit `-Os` in "Other flags" alongside an "Optimization Level"
    dropdown set to `size`) — duplicated flags are harmless, GCC's last-wins semantics
    apply cleanly.
  - `Other optimization flags` — a *separate* option field, easy to miss; may contain
    per-configuration deltas (e.g. a safety-core config using `-fno-if-conversion`
    that the application core does not).
  - Include paths (`Include paths (-I)`) — listed in IDE order, frequently containing
    duplicates. Order rarely matters for correctness (unique header names across
    dirs) but preserve it when in doubt.
- Per linker tool (`Cross GCC Linker`):
  - `Linker flags` — contains `-T <script>`, `--gc-sections`, `--mem-holes`,
    `--extmap`, `--cref`, `-Map=`, `-XML-Map=`. The `-Map`/`-XML-Map` paths are
    target-specific output locations, not shared config.
  - `Libraries (-l)` — **order matters** for static linking (GNU `ld` resolves
    archive symbols left-to-right; a library must appear after the objects/libraries
    that depend on it). Reproduce this list's order exactly in `target_link_libraries`.
  - `Library search path (-L)` — supplementary; needed so relative `-T script.ld`
    references inside library-wrapper CMake files can resolve via linker script
    search-path fallback.
- **No macro/preprocessor `-D` option present** does not mean "check elsewhere" —
  confirm by grepping `.cproject` for `preprocessor`/`def.symbols`. If absent, the
  build truly has zero custom `-D` flags; don't invent any.

### `sourceEntries` exclusion semantics — the biggest trap

CDT source entries look like:

```xml
<entry excluding="HAL|Devices|safety_core|boot_core|appl_core_release|...|opensyde|doc"
       flags="VALUE_WORKSPACE_PATH|RESOLVED" kind="sourcePath" name=""/>
```

**Do not assume `excluding="HAL|Devices|opensyde|doc"` recursively excludes every
folder named `HAL`, `Devices`, or `opensyde` anywhere in the tree.** Bare-name
patterns in this field only match a path **exactly one level below the entry's own
root** (here, the whole project root, since `name=""` and `resourcePath=""`). If no
such top-level folder exists, the pattern matches nothing and is a no-op — commonly a
stale leftover from a renamed/restructured project.

**How this manifested concretely:** `appl_core`'s source entry excluded `HAL`,
`Devices`, and `opensyde` — yet `appl_core/src/HAL/*.c` and
`appl_core/src/opensyde/**/*.c` were *not* excluded and were, in fact, essential
compiled sources (confirmed by the linker `.map` file). The exclusion patterns that
actually had effect were the ones matching real top-level project children:
`boot_core`, `safety_core`, `debug`, and the various `*_release`/`*_debug` build-output
folders — i.e., "exclude my sibling cores and build-output directories from my own
source root," nothing more.

**Verification method (trust this over pattern-reading):** grep the shipped `.map`
file for `\.o\b` tokens and cross-check against `find <core>/src -name '*.c'`. The
`.map` file is the linker's own record of every object file actually pulled into the
final binary — it cannot lie about what was compiled. If in doubt about which files
belong in a target, this is the authoritative source, not exclusion-pattern
interpretation.

```sh
grep -oE '[A-Za-z0-9_.]+\.o\b' core.map | sort -u
find core/src -name '*.c' | xargs -n1 basename | sed 's/\.c$/.o/' | sort -u
# diff the two lists
```

If a `.c` file's `.o` counterpart is absent from the map, it is dead/unused code not
part of this build config, regardless of what the exclusion filter seems to imply.

---

## 2. Toolchain file gotchas (CMake cross-compiling on a Windows host)

```cmake
set(CMAKE_SYSTEM_NAME Generic)          # bare-metal, no OS
set(CMAKE_SYSTEM_PROCESSOR tricore)
```

- **`CMAKE_EXECUTABLE_SUFFIX` is not yet defined when the toolchain file runs.** Do
  not write `"${TOOLCHAIN_BIN}/tricore-gcc${CMAKE_EXECUTABLE_SUFFIX}"` inside the
  toolchain file expecting `.exe` — the variable is empty at this point, producing a
  compiler path CMake then reports as "not a full path to an existing compiler tool"
  (even though `The C compiler identification is GNU x.y.z` succeeds first, which is
  a misleading partial-success signal). Fix: branch on `CMAKE_HOST_WIN32` and hardcode
  `.exe` yourself.
- **`WIN32` (and `UNIX`/`APPLE`) reflect the *target* system, not the host**, once
  `CMAKE_SYSTEM_NAME` is set to something other than the host's own OS name. With
  `CMAKE_SYSTEM_NAME Generic`, `WIN32` is **false** even when actually running on
  Windows. Any host-OS branching anywhere in the CMake project (e.g. choosing between
  a `windows/` and `linux/` variant of a bundled signing tool) must use
  **`CMAKE_HOST_WIN32`**, never `WIN32`, once cross-compiling. This is a silent,
  easy-to-miss failure mode: the build configures and even links fine, then a
  post-build custom command references a nonexistent path and fails with a generic
  "not recognized as an internal or external command."
- **`-nocrt0` / custom-linker-script targets fail CMake's compiler sanity check.**
  CMake's default `CMAKE_C_COMPILER_WORKS` check tries to link a full trivial
  executable; a bare-metal target with no default startup code / entry point cannot
  satisfy that. Set:
  ```cmake
  set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
  ```
  before/instead of relying on the default executable try-compile. This is
  standard practice for any embedded/bare-metal CMake toolchain file and should be
  applied by default whenever `-nocrt0` (or `--specs=nosys.specs`,
  a custom `-T`, etc.) appears in the original linker flags.

---

## 3. Wrapping prebuilt (precompiled) static libraries

Cross-toolchain embedded projects frequently link against **prebuilt `.a` archives**
supplied without source (vendor BIOS/HAL layer, protocol stacks, etc.) — verify this
by checking whether `.c` files exist alongside the `.a`; if not, the library is a
pure link-time dependency, not a `add_subdirectory`-buildable target.

Pattern used successfully here — one `CMakeLists.txt` per library directory, using
`IMPORTED GLOBAL` so the target is visible from every directory that `add_subdirectory`s
it, without needing `ALIAS` plumbing:

```cmake
add_library(some_lib STATIC IMPORTED GLOBAL)
set_target_properties(some_lib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/libsome_lib.a")
target_include_directories(some_lib INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_directories(some_lib INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
```

Notes:
- Relative paths (or `.`) passed to `target_include_directories`/
  `target_link_directories` inside a library's own `CMakeLists.txt` resolve relative
  to **that file's own directory** (`CMAKE_CURRENT_SOURCE_DIR` at the point the
  command executes), not the consuming target's directory — this is what makes the
  pattern portable regardless of who calls `add_subdirectory` on it.
- `target_link_directories(... INTERFACE ...)` on the library is what allows a
  consumer's `-T relative_script.ld` (passed via `target_link_options`) to resolve:
  GNU `ld` falls back to `-L` search paths when resolving a bare (non-absolute)
  linker-script filename passed to `-T`. In practice, prefer passing an **absolute**
  `-T` path built from `CMAKE_CURRENT_SOURCE_DIR` in the consuming executable target
  instead of relying on this fallback — it's more robust across `ld` versions and
  easier to read/debug.
- If some libraries in a set already ship IDE-authored `CMakeLists.txt` wrappers
  (a sign the vendor anticipated this exact conversion) and others don't, write the
  missing ones matching the existing house style exactly — consistency matters more
  than personal preference here, and it signals the codebase already expected a
  CMake path to exist eventually.
- **Link only the libraries the original `-l` list actually named.** A library
  directory may contain extra `.a` files not referenced by the original build (e.g.
  an unused variant/subset library sitting next to the one that's actually linked).
  Linking it anyway changes the output; don't.

---

## 4. Reproducing the post-build artifact pipeline

The IDE's `postbuildStep` is a shell one-liner chaining multiple tools. Reproduce it
as a `POST_BUILD` custom command on the executable target, factored into a reusable
function if multiple targets share the same pipeline:

```cmake
function(add_hex_output target)
    set(unsigned_hex "${CMAKE_CURRENT_BINARY_DIR}/${target}_unsigned.hex")
    set(signed_hex   "${CMAKE_CURRENT_BINARY_DIR}/${target}.hex")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${target}>" "${unsigned_hex}"
        COMMAND ${signing_tool_path} -i "${unsigned_hex}" -o "${signed_hex}"
        BYPRODUCTS "${unsigned_hex}" "${signed_hex}"
        VERBATIM)
endfunction()
```

- Discover a bundled signing/post-processing tool's CLI contract by invoking it with
  no args or `--help` (many such vendor tools print full usage on invalid invocation
  even without a proper help flag). Don't guess flags from the postbuild string alone
  without confirming — the original recipe may only exercise a subset of the tool's
  options (e.g. no `-a`/`-k` in this case), and confirming saves a debugging cycle.
- **Do not point build output paths at the same location as any checked-in "known
  good" reference artifact you intend to diff against.** Write to the CMake build
  tree; leave the repo's shipped reference file untouched. This sounds obvious but is
  easy to get backwards when directly transcribing an IDE recipe that *did* overwrite
  the shipped copy as its normal behavior.

---

## 5. Validating equivalence: what "identical" can and cannot mean

Byte-for-byte identical output across two different build systems (IDE internal
builder vs. hand-written CMake) is a **best-effort goal, not a guaranteed
achievable one**, for two independent reasons discovered here:

1. **Signing/timestamp tools may embed wall-clock time.** A flash-signature/build tool
   can stamp the current date/time into a reserved metadata block as part of its
   normal signing behavior (observed here: a human-readable build-date string plus a
   dependent CRC, sitting inside the address range the tool itself reported as its
   "signature block"). This is **expected, inherent, non-deterministic-by-design**
   output — not a defect in the CMake recipe. Confirm by checking whether the diffing
   bytes fall inside the address range the signing tool printed as its own signature
   block location.

2. **Object/section link order affects addresses, not content, when
   `-ffunction-sections -fdata-sections` + `--gc-sections` are in play.** GNU `ld`
   places directly-specified (non-archive-member) object files' sections in the order
   they appear on the linker command line. If the IDE's internal builder enumerated
   source files in some order this CMake recipe cannot recover (Eclipse resource-tree
   traversal order is not guaranteed to be alphabetical, and is not recoverable from
   the checked-out repo state), every function may land at a different address than
   the original — producing a large volume of `.hex` line differences that are purely
   **relocation noise**, not code differences. A target with very few source files
   (here: `boot_core`, one file; `safety_core`, four files) has little room for this
   effect and may match near-perfectly; a target with dozens of files (here:
   `appl_core`, 44 files) is far more likely to diverge this way.

### How to actually validate equivalence under these constraints

Do **not** stop at a failing textual `diff` of two `.hex` files — it will look like a
total mismatch even when the underlying code is identical. Use a layered approach,
cheapest/most-conclusive checks first:

1. **Line-count / scope of a normalized diff.** Strip line-ending differences
   (`diff --strip-trailing-cr`) before concluding anything. A small diff (single
   digit to low tens of lines) localized to one address region is a strong signal of
   "timestamp/signature only" — go inspect that region against the signing tool's
   own reported block address.
2. **Object/symbol set comparison via the linker `.map` file.** Extract all `.o`
   tokens from both the shipped and freshly-built `.map` and diff the sets. Identical
   sets prove the same source files were compiled and linked, independent of address
   layout.
3. **Call-graph structural comparison via a `-Wl,-XML-Map=` (or equivalent
   toolchain-specific) cross-reference map, if the toolchain produces one.** This
   format records *semantic* relationships (which object calls which function) rather
   than addresses, and is far more diff-friendly across a link-order change:
   - Normalize away build-environment-specific noise first: absolute repo path
     prefixes, path separator style (`\` vs `/`), and build-system-specific object
     naming conventions (e.g. CMake's `<target>.dir/path/to/file.c.obj` vs an IDE's
     `path/to/file.o`).
   - Extract just the ordered sequence of `called_function="..."` attribute values
     (or equivalent) from both maps and diff *that* reduced sequence. This collapses
     away all path/address noise and directly answers "did the same call graph get
     discovered in the same order," which is the strongest available proxy for "same
     compiled code" without a disassembler.
   - A handful of extra/missing entries (e.g. two extra soft-float comparison
     library calls, `__ltdf2`/`__ledf2`, present only in one build) is normal
     floating-point codegen variance and does not invalidate the equivalence finding
     — note it, don't chase it further unless it recurs at a scale suggesting a real
     flag mismatch.
4. **Report findings honestly and specifically, per target.** Different targets in
   the same project can land at different points on the "byte-identical modulo
   timestamp" ↔ "same content, different layout" spectrum (as happened here: two of
   three cores were effectively byte-identical, one was not). Don't average this into
   a single vague "close enough" — state which targets matched at which level of
   rigor and why the gap exists for the ones that didn't.

**When to stop chasing byte-for-byte parity:** once object/symbol sets and call-graph
structure are confirmed identical and the only remaining delta is provably an
IDE-internal, unrecoverable file-enumeration order (not a flag, include-path, or
source-list error on the CMake side), further effort has sharply diminishing returns
relative to the goal of *establishing that a command-line build is viable*. Document
the finding and move on rather than reverse-engineering an undocumented IDE
implementation detail.

---

## 6. Generating per-target `compile_commands.json` for LSP tooling

`set(CMAKE_EXPORT_COMPILE_COMMANDS ON)` produces one **merged** `compile_commands.json`
in the build directory covering every target. For a multi-core/multi-target embedded
project where each target has its own include-path set (and an editor may be opened
rooted at any one core's subdirectory), this is often not what you want for LSP
discovery: clangd/ccls locate `compile_commands.json` by walking **upward** from the
open file, and will pick the nearest one — which should ideally only contain (or at
least correctly cover) that core's files.

Approach: post-process the merged database with a small `cmake -P` script using
`string(JSON ...)` (CMake ≥ 3.19), filtering entries by testing whether each entry's
`file` field (normalize `\` to `/` first) contains `/<core_name>/`, and writing a
filtered copy to `<core>/compile_commands.json`:

```cmake
file(READ "${COMPDB_INPUT}" _json)
string(JSON _count LENGTH "${_json}")
math(EXPR _last "${_count} - 1")
foreach(_i RANGE ${_last})
    string(JSON _entry GET "${_json}" ${_i})
    string(JSON _file GET "${_entry}" file)
    string(REPLACE "\\" "/" _file_norm "${_file}")
    string(FIND "${_file_norm}" "/${_core}/" _pos)
    if(NOT _pos EQUAL -1)
        string(JSON _entries SET "${_entries}" ${_entries_count} "${_entry}")
        math(EXPR _entries_count "${_entries_count} + 1")
    endif()
endforeach()
file(WRITE "${_out}" "${_entries}")
```

Wire it up as an `add_custom_target(... ALL ...)` at the end of the top-level
`CMakeLists.txt`, after all `add_subdirectory()` calls. No explicit `DEPENDS` on the
compiled targets is required: `compile_commands.json` is written by CMake itself
during the **configure/generate** phase, which fully completes before Ninja/Make
begins executing any build step — so by the time this custom target's command runs,
the merged input file is guaranteed to already exist.

A simpler alternative (full, unfiltered copy of the merged database dropped into each
core directory instead of a per-core filtered one) is also functionally correct for
LSP purposes, since `compile_commands.json` entries are matched by absolute file path
regardless of which copy of the database an LSP server loaded from — filtering is a
tidiness/performance improvement, not a correctness requirement.

---

## 7. Reusable checklist for the next conversion

1. Locate and read `.cproject` (or equivalent Managed Build XML) directly — do not
   infer flags from documentation or IDE screenshots.
2. For each build configuration: extract compiler path, all flag-bearing options
   (checking *every* separate option field, not just the obvious "misc flags" one),
   include paths, linker flags, library list **with order preserved**, library
   search paths, and the full `postbuildStep`/`postannouncebuildStep` string.
3. Treat any `sourceEntries excluding=` filter with suspicion — verify actual
   compiled-file membership against a shipped linker `.map` file's object list, not
   against your reading of the exclusion pattern.
4. Write the CMake toolchain file with: `CMAKE_SYSTEM_NAME Generic` (or appropriate),
   explicit `.exe`/host-suffix handling via `CMAKE_HOST_WIN32` (not `WIN32`), and
   `CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY` if the target uses `-nocrt0` or
   otherwise cannot link a trivial default executable.
5. Wrap prebuilt archives as `IMPORTED GLOBAL` library targets exposing include dirs,
   link dirs, and (if useful) their own compile/link option requirements as
   `INTERFACE` properties. Check for existing vendor-authored wrapper
   `CMakeLists.txt` files first — their presence/absence pattern can itself be a clue
   about which conversion path the vendor anticipated.
6. Reproduce the post-build artifact pipeline (objcopy, signing, packaging) as
   `POST_BUILD` custom commands writing into the build tree, never into any directory
   holding a reference artifact you intend to diff against.
7. Configure, build, and validate equivalence using the layered approach in §5:
   normalized textual diff scope → `.map` object-set diff → cross-reference/call-graph
   structural diff → honest per-target reporting.
8. If LSP support is wanted, enable `CMAKE_EXPORT_COMPILE_COMMANDS` and, for
   multi-target/multi-root projects, post-process into per-root copies using
   `string(JSON ...)` filtering wired as an `ALL`-target custom step.
9. Gitignore the build directory and any regenerated `compile_commands.json` copies
   placed inside the source tree — they are build outputs, not source.
