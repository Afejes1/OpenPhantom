# Campaign022 function map

| Address | Original Name | New Name | Confidence | Purpose | Evidence |
|---|---|---|---|---|---|
| 0x0041F37E | FUN_0041f37e | mark_b3d_cell_lod_run_tails | HIGH | Set/clear only tail bit8 using later sentinel and mask-containment scan | Complete249-byte body; caller0041F2CE; no calls/globals; u8 count/masks and u16 material sentinel independently reviewed. |
| 0x0041F56F | FUN_0041f56f | compute_b3d_cell_lod_runs | HIGH | Build capped254-byte LOD run lengths, retaining skipped records | Complete258-byte body; caller0041F2CE; current flag0x80 skip, next flag inclusion and sentinel stop; no calls/globals. |
| 0x0045C47B | FUN_0045c47b | get_sw3d_model_load_state | HIGH | Return model field0xD0 unchanged | Full14-byte body; two callers pass modelatowner+0x34 and test state; no callees/globals. |
| 0x0043AFD3 | FUN_0043afd3 | get_shield_slot_radius | HIGH | Return active shield radius or positive zero for invalid/inactive slot | Full67-byte body; signed0..31 range, table stride0xB4, radius0x4C; both collision callers use x87 result. |
| 0x0046B754 | FUN_0046b754 | get_current_system_font | HIGH | Return the current system font pointer | Full10-byte body, global004B77EC, two consuming callers; no mutation/calls. |
