#include "audio_helpers.h"
// FUNCTION: WMAIN 0x496bfb
OP_AUDIO_BUFFER *op_audio_duplicate(OP_AUDIO_BUFFER *source)
{
 OP_AUDIO_BUFFER *result;
 int status;
 if(!op_audio_gate_word)return 0;
 status=((OP_AUDIO_DEVICE *)op_audio_gate_word)->vt->duplicate((OP_AUDIO_DEVICE *)op_audio_gate_word,source,&result);
 if(status!=0)return 0;
 return result;
}
