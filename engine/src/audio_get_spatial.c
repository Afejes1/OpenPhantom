#include "audio_helpers.h"
// FUNCTION: WMAIN 0x496bcd
OP_AUDIO_SPATIAL *op_audio_get_spatial(OP_AUDIO_BUFFER *o)
{
 OP_AUDIO_SPATIAL *result;
 int status;
 status=o->vt->query(o,op_audio_spatial_iid,&result);
 if(status>=0)return result;
 return 0;
}
