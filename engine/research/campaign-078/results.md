# Campaign 078

Three audio wrappers matched on their first compiler attempt: pause 35 bytes, play 52 and unlock 50. The complete 137 bytes include one verified global operand; play and unlock are literal raw matches. Final native/Docker code and relocation records agree, with 42,620 focused checks passing. Shared acceptance pending in [Acceptance 056](../acceptance-056/result.json).

Static stack setup resolves misleading Ghidra thiscall metadata: wrappers are cdecl and their virtual calls are stdcall with explicit this pointers. Pause checks the audio gate and ignores callback status. Play normalizes its looping flag; play and unlock return true only for exactly zero callback status. Tests cover object/table preservation and deliberate callback mutations with independently prepared expected state.

All three addresses are marked unfinished in the latest contributor snapshot. No byte exclusions or warning waivers; whole-executable identity remains unfinished.
