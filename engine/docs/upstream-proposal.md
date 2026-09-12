# Draft: original-toolchain matching baseline

This remains an unpublished experiment in the contributor's fork. On 2026-09-12
the contributor confirmed direct discussion with the authors and authorized
continued registry, restore, and module reconstruction work. A public issue is
not a prerequisite for this fork's work. This does not claim upstream acceptance
or publication of the component design.

Establish a VC5 RTM build and comparison workflow for the retail WMAIN.EXE
identified in engine-identification.md, beginning with its four reference
functions. Use independently reconstructed C, contributor-supplied compiler
tools and reference files, explicit relocation accounting, and regression
checks that preserve existing matches. Separate code-generation matching from
literal equality of the linked executable.

The fork now calibrates all four reference functions with VC5 RTM inside
Docker/wibo and verifies projection padding as well as instruction bytes.
Public proof metadata contains hashes and counts; private binaries stay local.

Proposed layout: engine/src for candidates, engine/tools for build/comparison,
engine/tests for synthetic tests, and engine/docs for evidence and instructions.
No game executable, extracted data, compiler package, or binary diff is included.
Public CI can test the tooling without retail inputs. A private verification run
must supply the pinned original executable and a fingerprinted VC5 installation.

Module expansion uses the recorded comparison and regression criteria. The
unresolved startup/heap runtime-library revision is
tracked independently; original machine-code fragments will not be copied into
the reconstruction to conceal that gap.
