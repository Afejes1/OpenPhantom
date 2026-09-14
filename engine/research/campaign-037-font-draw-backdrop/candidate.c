#include "api.h"
// FUNCTION: WMAIN 0x0046b693
void op_font_draw_backdrop(void)
{
    float right_bound, bottom_bound, top_bound, horizontal_border, vertical_border, left_bound;
    if (!op_current_font || !op_current_font->backdrop)
        return;
    op_get_last_textbox((unsigned int *)&left_bound, (unsigned int *)&right_bound, (unsigned int *)&top_bound,
                        (unsigned int *)&bottom_bound);
    horizontal_border = op_current_font->backdrop_width * op_font_screen_width;
    left_bound -= horizontal_border;
    right_bound += horizontal_border;
    vertical_border = op_current_font->backdrop_height * op_font_screen_height;
    top_bound -= vertical_border;
    bottom_bound += vertical_border;
    op_draw_font_backdrop(op_current_font->backdrop, left_bound, right_bound, top_bound, bottom_bound);
}
