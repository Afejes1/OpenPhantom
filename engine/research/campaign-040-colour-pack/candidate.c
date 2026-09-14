#include "api.h"
// FUNCTION: WMAIN 0x0047c4f8
unsigned int op_colour_pack(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha,
                            const OP_COLOUR_FORMAT *format)
{
    unsigned int result = 0;
    red >>= format->red_reduction;
    result |= (unsigned int)red << format->red_position;
    green >>= format->green_reduction;
    result |= (unsigned int)green << format->green_position;
    blue >>= format->blue_reduction;
    result |= (unsigned int)blue << format->blue_position;
    alpha >>= format->alpha_reduction;
    result |= (unsigned int)alpha << format->alpha_position;
    return result;
}
