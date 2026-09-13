# Campaign019 results

Two vertex readers match all 301 bytes after verified call bindings. Three other
readers are near matches, and dialog remains a research candidate above 5%.
All six complete spans total 907 bytes and all 627 authored behavior checks pass.
Only zero-difference functions are eligible for batch acceptance.

| Function | Full bytes | Differences | Category | Checks |
| --- | ---: | ---: | --- | ---: |
| world_read_dialog | 153 | 8 | research-candidate | 67 |
| world_read_global_vertices | 149 | 0 | exact-function-match | 112 |
| world_read_local_vertices | 152 | 0 | exact-function-match | 112 |
| world_read_materials | 149 | 6 | near-match | 112 |
| world_read_light1 | 152 | 6 | near-match | 112 |
| world_read_light2 | 152 | 6 | near-match | 112 |

One Sol medium worker made 12 candidate compiler invocations. Parent review kept
every candidate body unchanged and strengthened every fixture with owned storage,
guards, callback-visible mutations and explicit forwarding checks. Native and
locked-Docker compilation agree on each final function and its relocation map.
The exact functions needed three worker compiles for global vertices and one for
local vertices. This packet had fewer first-attempt matches than Campaign018.

An initial parent fixture-generation substitution corrupted two type identifiers;
Docker rejected that fixture at compilation. Its failed receipt is retained in
metrics.json. The corrected fixture passed a fresh pair. Final verification used
12 candidate compiles and six successful fixture compiles; the failed pair adds
two candidate compiles and one unsuccessful fixture compile. No candidate-body,
matcher, original-byte or warning-policy changes were needed.

The three near cases differ by six local stack displacements each. Dialog differs
by eight pointer/index stack displacements, 5.2288% of its 153-byte span. The 5%
threshold schedules later research; it does not waive bytes or establish parity.
The original local-vertex payload mismatch returns success without reading; the
source and fixtures retain that behavior. All loop readers capture their record
pointer before the first read, whereas dialog captures it after its bulk read.

Ghidra comments were updated and saved; existing function names were retained.
Registered history remains 26 functions and 23 immutable records. The next
acceptance checkpoint will integrate the two exact vertex readers into canonical
world/header types and run the combined original-toolchain regression once.
Final linked placement, real backend behavior and whole-executable identity remain
pending. No original executable or real backend was executed.
