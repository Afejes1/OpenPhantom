# Draft: original-toolchain matching baseline

This is an unpublished proposal for upstream discussion. Work on this fork is
experimental; upstream has not approved the component design.

Establish a VC5 RTM build and comparison workflow for the retail WMAIN.EXE
identified in engine-identification.md, beginning with its four reference
functions. Use independently reconstructed C, contributor-supplied compiler
tools and reference files, explicit relocation accounting, and regression
checks that preserve existing matches. Separate code-generation matching from
literal equality of the linked executable.

Proposed layout: engine/src for candidates, engine/tools for build/comparison,
engine/tests for synthetic tests, and engine/docs for evidence and instructions.
No game executable, extracted data, compiler package, or binary diff is included.
Public CI can test the tooling without retail inputs. A private verification run
must supply the pinned original executable and a fingerprinted VC5 installation.

Before expanding into complete engine modules, agree on this layout and the
acceptance criteria. The unresolved startup/heap runtime-library revision is
tracked independently; original machine-code fragments will not be copied into
the reconstruction to conceal that gap.
