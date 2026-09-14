# Acceptance 047 results

Six new module functions bring the accepted total from 267 to 273. They match 900
complete original function bytes after eleven verified address operands, with
zero differences, byte exclusions or new warning waivers. All 273 shared comparisons
and the original-toolchain authored fixture pass. The six retained case matrices
execute the real accepted finder and pass 1,613,760 checks; these assertions are not
additional function counts or game-wide coverage.

Clean source `4789186cfddc4781d88da548aa2807ea120171ec` builds as `20260914-095145-92db9e9c`.
[Immutable event 70](../../evidence/runs/000070-5617c8a530e377e3.json) records all source and toolchain
evidence. All 468 source inputs also match their committed Git blobs exactly.
[Same-source supplemental CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34851910959)
passes, including modern x86 warnings-as-errors and the shared behavioral smoke.

Independent review confirmed the prior 267 target/registry entries are unchanged,
all six canonical bodies differ from their focused sources only by the shared
header include, and no assertions were lost during fixture namespacing. One shared
full build was used for this batch. The original game was never executed.

All six additions are not_done in the latest contributor snapshot. Against that
snapshot, 265 of 943 unfinished entries now have local acceptance, leaving 678 if
the team's 1,298 done claims are correct. Eight local matches overlap their done
list and must not be subtracted twice. Their source remains unverified here.
Whole-executable byte identity remains unfinished.
