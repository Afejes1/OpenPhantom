#include "api.h"
// FUNCTION: WMAIN 0x4969df
void op_audio_shutdown(void)
{
 if(op_audio_gate_word){
  if(op_audio_primary){
   op_audio_primary->vt->volume(op_audio_primary,op_audio_primary_volume);
   op_audio_primary->vt->release(op_audio_primary);
   op_audio_primary=0;
  }
  ((OP_AUDIO_DEVICE *)op_audio_gate_word)->vt->release((OP_AUDIO_DEVICE *)op_audio_gate_word);
  op_audio_gate_word=0;
 }
}
