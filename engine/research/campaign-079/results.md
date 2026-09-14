# Campaign 079

Music stop, music-is-playing and audio-play-position reconstruct 214 complete bytes with six verified address operands. Stop matched on attempt one, audio position on attempt two, and music status on attempt four. Seven implementation compiler invocations total. All final native/Docker pairs agree; 42,640 focused assertions pass. Shared acceptance completed in [Acceptance 056](../acceptance-056/result.json).

Parent review corrected a preparation error before implementation: the music function allocates20 stack bytes, comprising16 bytes of initialized MCI parameters plus a separate4-byte return-status local. The fixture inspects only the16-byte block. VC5 intrinsic memset yields the observed initialization sequence, without warning waivers or altered compiler flags. Audio local names select the observed stack-slot arrangement without adding storage.

Callbacks can mutate music globals and the live audio vtable. The audio position query uses the updated vtable and the supplied optional output; all focused mutation and preservation checks are retained in shared tests. Whole-executable identity remains unfinished.
