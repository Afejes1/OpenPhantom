# Original-toolchain matching baseline

This fork contains comparison infrastructure and four **unverified C source
candidates**, not a playable reconstructed OpenPhantom engine.

The required code generator is the original **Visual C++ 5.0 RTM** identified in
[engine-identification.md](engine-identification.md#1a-resolved-the-toolchain-by-byte-for-byte-compilation).
This profile runs it natively on Windows. No VC5 toolchain has been obtained or
run in this fork yet; **zero functions are claimed as verified matches**.
Modern MSVC is used only for a separate finite-behavior smoke test.

## What is available

- A pinned retail reference manifest with four full function extents and explicit
  address-operand bindings, validated against a contributor's local executable.
- An original-toolchain driver with private configuration, complete tool/header/
  library fingerprints, per-source options, fresh build directories, artifact
  hashes, and stale-build rejection.
- A bounded PE32/COFF comparator, regression gate, and literal whole-file check.
- Optional pinned reccmp 0.1.7 diagnostics. Its scores never grant strict matches.
- Synthetic parser/comparator/provenance tests and a separate modern x86 fixture.

See [baseline evidence](docs/baseline-evidence.md) for addresses, layouts,
floating-point behavior, comparison limits, and remaining reconstruction work.
[Toolchain access](docs/toolchain-access.md) records the unresolved acquisition
prerequisite. [The upstream proposal](docs/upstream-proposal.md) is an unpublished
draft; this experiment has not been agreed with upstream.

## Tests that need no original compiler or game

Run from the repository root with Python 3.10 or newer:

```powershell
python -m unittest discover -s engine/tests -p "test_*.py" -v
```

For the supplemental finite-behavior smoke, use modern MSVC and CMake:

```powershell
cmake -S engine/tests -B engine/build/modern-smoke -A Win32
cmake --build engine/build/modern-smoke --config Release
ctest --test-dir engine/build/modern-smoke -C Release --output-on-failure
```

This test checks synthetic inputs and 32-bit structure offsets. It does **not**
prove VC5 code generation or x87 unordered-comparison behavior. The latter is
tested by the fixture only when built through the original-toolchain driver.
A green public CI run makes only the synthetic/supplemental claims in its name.

## Private original-toolchain workflow

Keep licensed tools, original files, full diffs and reports out of version
control. Do not copy any game binary into this checkout. The driver reads the
reference in place and never executes or modifies it.

1. Obtain the original toolchain as described in the access document. Create
   `engine/private/toolchain.json` from `engine/toolchain.example.json`; replace
   paths and provenance with real values.
2. Set the reference path to your retail copy. Both its SHA-256 and every
   configured original operand must agree:
   ```powershell
   $retail = 'C:\YourGame\GAMEDATA\BIN\WMAIN.EXE'
   python engine/verify.py reference --reference $retail
   python engine/verify.py configure
   python engine/verify.py preflight --reference $retail
   ```
3. Build a fresh set of candidates; save the directory printed by the command:
   ```powershell
   python engine/verify.py build --reference $retail
   $buildDir = 'engine/build/<directory printed above>'
   python engine/verify.py test --reference $retail --build $buildDir
   python engine/verify.py compare --reference $retail --build $buildDir
   ```
   Inspect failures, refine the relevant C source/flags from evidence, and build
   again. A changed source or driver invalidates old outputs. The linked
   `WMAIN.EXE` here is a **console test fixture**, never the game or an installer
   payload. Only that verified fixture is executed by the `test` command.
4. When all four code comparisons and VC5 behavioral tests pass, establish a
   separately named private baseline:
   ```powershell
   python engine/verify.py accept --reference $retail --build $buildDir --baseline engine/private/baseline-01.json
   ```
   Build and test again from scratch, then check that second run:
   ```powershell
   python engine/verify.py check --reference $retail --build $buildDir --baseline engine/private/baseline-01.json
   ```
   Use the second build's directory. Existing baselines are never overwritten
   by `accept`. Changes to the target manifest or toolchain require explicit
   review and a new baseline; they cannot silently weaken an old gate.

Commands return 0 for the stated check passing, 1 for a measured mismatch or
failed behavioral test, and 2 for missing prerequisites, unsupported structures,
stale builds or invalid regression evidence. Missing private inputs are failures,
not successful zero-function runs. JSON reports stay in the build directory.

## Reading results correctly

- `raw-code-match`: the entire one-function object code section equals the
  original function's bytes. Linked placement remains unverified.
- `relocation-adjusted-match`: other bytes are equal, and every adjusted DIR32
  operand matches its declared symbol/addend or defined constant. Reports give
  both complete code size and adjusted-byte count.
- `mismatch` / `unresolved`: the function is not accepted.
- `complete` in a comparison report requires all four code checks plus the
  hashed VC5 behavioral fixture result. It is not whole-executable completion.

First-batch objects must contain one complete code section for one function.
No padding is stripped and no register, branch, opcode, immediate, missing
symbol, or unknown relocation is excused. The current parser deliberately
rejects other object layouts and relocation kinds rather than guessing.

The full-file gate is available as:

```powershell
python engine/verify.py whole --reference $retail --build $buildDir
```

It compares every byte, including headers, timestamp, padding and data, with
no exclusions. The diagnostic fixture is expected to fail this gate. Original
module/link order, resources, libraries and historical link metadata remain
future reconstruction work. No copied startup bytes or executable patching
are used to manufacture equality.

## Optional local diagnostics

```powershell
python -m venv engine/.venv
engine/.venv/Scripts/python.exe -m pip install -r engine/requirements-diagnostics.txt
engine/.venv/Scripts/reccmp-decomplint.exe --target WMAIN --warnfail engine/src
engine/.venv/Scripts/python.exe engine/verify.py diagnose --reference $retail --build $buildDir
```

Detailed HTML disassembly remains in the ignored build directory. The shared
reccmp project file contains only metadata; generated user/build files are
ignored. The Python comparator does not import or copy reccmp implementation
code. The original-toolchain PDB/HTML path cannot be validated until VC5 is
available.

No changes to `legacy/` or `installer/` are required. Do not ship this fixture
or any original game data in a commit, release, or issue attachment.
