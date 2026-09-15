#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496A46
void op_audio_set_master_volume(float value)
{
 int result;
 int volume;
 if(!op_audio_primary)return;
 volume=(int)((1.0f-value)*(1.0f-value)*-5000.0f);
 (void)(volume>=-10000 ? (volume<=-1 ? volume : -1) : -10000);
 result=op_audio_primary->vt->volume(op_audio_primary,volume);
 (void)result;
}
