# Prepared sound lifecycle dispatcher

Full004158F0..004159F0 target256bytes:187body followed44bytes of jump targets and25bytes
of switch mapping. All24 address operands verified, including13 internal table/label
bindings,9 cdecl near calls and2 globals. No table or trailing byte may be dropped.
Registered with Bapsound name in initialize_game_systems; function has int event argument
and returns int result. Default result2; switchsource follows natural original cases.

1: result=op_sound_module_init();2: result=op_sound_shutdown();25:result=op_sound_register_level();
6:result=op_sound_remove_level(0);18:op_sound_reset_places();result=0;
19:op_sound_stop_all();result=0;13:op_sound_quality_level=op_sound_performance_level-1;
FALL THROUGH to14:result=op_sound_periodic();8:op_sound_pause_all();result=0;
9:op_sound_resume_all();result=0. All other events return2 without calls or statechanges.
Callbacks are cdecl; init/shutdown/register_level/periodic return int(void);remove_level
returns int(int ignored); reset/stop/pause/resume are void(void). Globals are int words;
use bounded performance inputs to avoid an undefined signed-subtraction oracle.
Case source order above is observed code order. The compiler naturally emits one
byte-index table and one target table. Existing strict dir32-internal handling uses
original relative label offsets, not fabricated names. If layout differs, park it.

Fixture: explicit dispatch table for0..26 and negative/extreme unknown IDs, independently
set expected callback and return, nontrivial callback returns, event13 updates quality
before periodic and returns its result;14 preserves quality; callback-mutation behavior
and exact single-call/no-call counts. Whole state observation; no backend game execution.
No rawbytes/inline assembly/manufactured frames/table padding or byte masks.15minute/
10compile cap and five-no-improvements stop; parent does independent final pair.
