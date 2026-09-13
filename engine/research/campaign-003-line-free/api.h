#ifndef OP_CAMPAIGN_003_LINE_FREE_API_H
#define OP_CAMPAIGN_003_LINE_FREE_API_H

#include "line.h"

#ifdef __cplusplus
extern "C" {
#endif

void op_scan_release(void *memory);
void op_free_scan_line(op_scan_line *line);

#ifdef __cplusplus
}
#endif

#endif
