#ifndef OP_CAMPAIGN_002_LINE_RESET_H
#define OP_CAMPAIGN_002_LINE_RESET_H

typedef struct op_scan_line {
    int count;
    int index;
    int x;
    int y;
    int step_x;
    int step_y;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int major_delta;
    int minor_delta;
    int error;
    int diagonal_x;
    int diagonal_y;
    int diagonal_error;
    int straight_x;
    int straight_y;
    int straight_error;
    int unused_4c;
} op_scan_line;

#ifdef __cplusplus
extern "C" {
#endif

void *op_scan_allocate(unsigned int size);
op_scan_line *op_reset_scan_line(op_scan_line *line, int start_x, int start_y,
                                 int end_x, int end_y);

#ifdef __cplusplus
}
#endif

#endif
