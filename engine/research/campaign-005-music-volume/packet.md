# Prepared music helper: music_get_volume

Team-not-done bp/bapmusic.c. Complete interval 0x0041073F to 0x0041074A: 11 body/full bytes, no trailing alignment. Coordinator reviewed Ghidra full body, every instruction and next-function boundary. Target PE identity and complete DIR32 inventory were independently verified before source experiments. Immutable target.json lists every operand.

Original VC5 C /Od /MT. Observed API (cdecl): `float op_music_get_volume(void); extern float op_music_volume;`

Return the float32 global through x87 ST0. No clamp or initialized gate. Check ordinary finite values, signed zero and infinities; signaling NaN handling is not a bit-preservation promise. Calls from menu 0x441FA4 use volume.

Music-state setter 0x4105A3 caches 0x4AA41C; sequence setter 0x41060E caches 0x4AA420. Initialization and detach set 0x5BAB8C to 1/0 respectively. The volume setter owns 0x4AA418; caller inspection continues independently.

Author natural readable C and a focused authored fixture. Worker owns candidate.c, api.h, behavior.c, README.md and worker-log.json only. No target/case/shared tooling/Git/Ghidra edits. Log every attempt including failures with hashes and reports. Stop per function after 15 minutes, ten compiler invocations, or five successive nonimprovements. Once exact, freeze immediately; coordinator performs fresh verification. Native compile only; execute fixtures in locked Docker only. Preserve all bytes and operand requirements; no original execution or content publication.
