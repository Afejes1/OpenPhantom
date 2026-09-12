# Complete object evidence and compiler listing limits

The research artifact audit found a real truncation problem: all five VC5 /FAcs
listings from the pinned Docker/wibo 1.2.0 runtime stop before their function
ENDP and file END directives, several in mid-line. Earlier schema-2 validation
checked only nonempty output and hashes. That established retained-file identity,
not complete listing output. The historical hash receipts remain unchanged.

## Scope of the finding

The COFF objects have complete, bounded sections and relocation tables. The
strict matching gate compares those object bytes, not text listings, so the
finding does not invalidate its reported mismatches or historical accepted
functions. A later native compile using the identical fingerprinted VC5 binaries
produced terminated listings for all five candidates. Their complete function
bytes and relocation metadata agree exactly with the Docker objects. No newly
matching function was found: the culler still differs by six bytes, the static
collector by five, and the other three candidates fail relocation-position checks.

The observed failure is specific to listing output in the tested runtime setup.
The pinned [wibo process-exit implementation](https://github.com/decompals/wibo/blob/1.2.0/dll/kernel32/processthreadsapi.cpp)
uses an immediate host exit path; stream/DLL cleanup is a possible cause, not a
proven root-cause fix. No compiler, runtime DLL, game byte, Docker image, saved
configuration or toolchain lock was patched or replaced. A Docker dumpbin
alternative failed on an unimplemented MSVCP50 import and is not used.

## Schema 3 and private inspection inventories

Each candidate now also produces <function>.function.json in its ignored build
directory. It contains every byte stored in the COFF section in consecutive 16-byte
rows, the exact section size and hash, the object hash, and every relocation's
offset, kind, addend and complete symbol identity. This includes embedded switch
tables and alignment. These are authored candidate bytes with unresolved COFF
operands, not extracted retail instructions or a disassembly/equivalence claim.

The runner reads the inventory back and requires exact equality with the parsed
object. At completion it rechecks every artifact hash and checks the inventory
against the object again. Missing bytes, padding, relocated-symbol metadata or
rows cannot be excused by a refreshed JSON hash. No byte masks are involved.

The assembly_listing_has_terminators field checks the expected function PROC
and ENDP in order and the final file END, ignoring source-comment lines. This
is an envelope check, not proof that every textual instruction is accurate.
A false result adds an explicit artifact notice. Research completion requires
the complete verified object inventory; it does not silently upgrade a partial
listing. Object matching remains the unchanged strict acceptance gate.

## Reproduce the optional native listing diagnostic

First create a schema-3 Docker comparison or behavior report with probe.py.
On Windows, with the user's existing authorized compiler installation:

    python engine/research/surface-emission/native_listing_diagnostic.py --docker-report engine/build/<batch>/research-result.json

This command verifies the Docker artifact hashes, current source hashes and
exact current toolchain lock before compiling the same five candidate sources.
It uses the same compiler and candidate flags, creates a separate output folder,
requires listing terminators, and compares all function bytes and relocation
identities with Docker. It rechecks inputs, both sets of artifacts and the lock
before marking its diagnostic report complete. It never executes generated
programs and never reads or executes the original game. A diagnostic report
cannot add accepted functions or replace the saved Docker configuration.

The matching and synthetic behavior workflows remain Docker-based and portable.
A non-Windows contributor can inspect the complete object byte/relocation
inventory while the pinned runtime's listing limitation remains unresolved.

## Validation

All five Docker candidates retain the preceding function-section hashes and
matching results. All four focused synthetic executables pass, including 3,038
culling, 4,208 static-cell and 1,195 legacy-cell checks. Eleven artifact tests pass;
new cases cover partial listings, misleading/commented/wrong-order terminators,
lost padding, byte holes/edits, missing fixups, wrong symbols/addends, and a
modified inventory even when its hash is updated. Full regression and recovery
were not repeated. The accepted baseline remains fourteen functions with nineteen
historical records and an intentionally historical full-source fingerprint.

The preceding 23 scheduling probes covered affine-call return/parameter
declarations, array-base float/double expressions and physical source-line
grouping. None resolved the remaining culler/collector differences. Their local
receipt is retained at engine/build/surface-scheduler-f2da0c5a/results.json;
its hash is in the [public checkpoint](listing-integrity-20260912.json).
