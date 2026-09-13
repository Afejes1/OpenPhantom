# Prepared music helper: music_is_detached

Team-not-done bp/bapmusic.c. Complete interval 0x00410592 to 0x004105A3: 17 body/full bytes, no trailing alignment. Coordinator reviewed Ghidra full body, every instruction and next-function boundary. Target PE identity and complete DIR32 inventory were independently verified before source experiments. Immutable target.json lists every operand.

Original VC5 C /Od /MT. Observed API (cdecl): `int op_music_is_detached(void); extern int op_music_initialized;`

Return normalized int (initialized == 0), without mutation. Check 0, 1, negative and other nonzero values. Two menu caller references agree.

Music-state setter 0x4105A3 caches 0x4AA41C; sequence setter 0x41060E caches 0x4AA420. Initialization and detach set 0x5BAB8C to 1/0 respectively. The volume setter owns 0x4AA418; caller inspection continues independently.

Author natural readable C and a focused authored fixture. Worker owns candidate.c, api.h, behavior.c, README.md and worker-log.json only. No target/case/shared tooling/Git/Ghidra edits. Log every attempt including failures with hashes and reports. Stop per function after 15 minutes, ten compiler invocations, or five successive nonimprovements. Once exact, freeze immediately; coordinator performs fresh verification. Native compile only; execute fixtures in locked Docker only. Preserve all bytes and operand requirements; no original execution or content publication.
