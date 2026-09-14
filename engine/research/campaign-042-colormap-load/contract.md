# colormap_load

Allocate844 bytes, clear entire allocation before read-entry, return captured pointer for any nonzero reader result. Allocation/read failure follows common free(original pointer),NULL return. Focused allocator-NULL path uses non-dereferencing authored free observer only; actual free-entry dereferences its argument, so that path is not safe once real cleanup is connected. No defensive null behavior invented.

Team not_done. Full original spans/all operands statically reviewed. Pinned VC5/Od/MT, no exclusions. Fifteen-minute/ten-attempt function cap; stop after five non-improving attempts. Native compile only; authored fixtures execute isolated Docker. Original executable never run.
