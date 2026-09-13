#ifndef OP_SYSTEM_FONT_API_H
#define OP_SYSTEM_FONT_API_H
typedef struct OP_FONT OP_FONT;
extern OP_FONT *op_system_font;
OP_FONT *op_get_system_font(void);
typedef char pointer_width[sizeof(void *) == 4 ? 1 : -1];
#endif
