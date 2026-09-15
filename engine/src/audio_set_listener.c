#include "audio_helpers.h"
// FUNCTION: WMAIN 0x496df3
void op_audio_set_listener(const OP_AUDIO_VECTOR *position,const OP_AUDIO_VECTOR *front,const OP_AUDIO_VECTOR *up)
{
 int status;
 if(op_audio_listener){
  status=op_audio_listener->vt->position(op_audio_listener,position->x,position->y,position->z,1);
  status=op_audio_listener->vt->orientation(op_audio_listener,front->x,front->y,front->z,up->x,up->y,up->z,1);
  status=op_audio_listener->vt->commit(op_audio_listener);
  (void)status;
 }
}
