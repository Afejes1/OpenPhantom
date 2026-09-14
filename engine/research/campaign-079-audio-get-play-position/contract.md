# audio_get_play_position

Complete original span 0x00496F6F to0x00496FC4, 85 bytes, no tail. Latest manifest not_done and no earlier source campaign. Cdecl wrappers, stdcall callbacks, all address bindings listed in target.json.

Music gate862064 skips when zero; device86205C and IAT8C16B0 feed MCI commands. Stop sends (device,808,0,NULL) and ignores status. Is-playing uses command814/flags100 and a16-byte status block {callback,value,item,track} initialized {0,0,4,0}. Its20-byte stack frame also has a separate return-status local; the fixture never reads that fifth dword. API return status is ignored; only returned value20D means stopped. Callback side effects on global gate/device remain intact.

Audio position first calls slot24(buffer,&status). Nonzero call status returns-1. Clear status bit1 returns0. Otherwise optional output triggers a fresh vtable read and slot10(buffer,output,&secondary); its result is ignored and wrapper returns1. Fixture retargets the live vtable during the first callback, checks callback-time state and full object/table/output snapshots, negative/positive failures, NULL output, and high-bit status/result words. Stack output pointees are not read before callbacks initialize them. No original execution or published game data.
