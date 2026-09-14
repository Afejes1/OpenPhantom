#ifndef OP_SKILLS_STATE_H
#define OP_SKILLS_STATE_H
#include <stddef.h>
#include <string.h>
typedef struct OP_SKILLS_STATE
{
    int words[48];
    int difficulty;
    unsigned char reservedc4[8];
    float elapsed;
    int current;
    unsigned char reservedd4[4];
} OP_SKILLS_STATE;
typedef char op_skills_layout[(sizeof(OP_SKILLS_STATE) == 0xd8 && offsetof(OP_SKILLS_STATE, difficulty) == 0xc0 &&
                               offsetof(OP_SKILLS_STATE, elapsed) == 0xcc && offsetof(OP_SKILLS_STATE, current) == 0xd0)
                                  ? 1
                                  : -1];
extern OP_SKILLS_STATE op_skills_state;
extern int op_skills_active_index, op_skills_previous_index;
void op_skills_new_game(void);
void op_skills_easier(void);
void op_skills_harder(void);
int op_skills_save(int);
int op_skills_restore(int);
extern float op_skills_frame_delta;
extern int op_skills_level_status;
void *op_skills_get_player(void);
void op_skills_tick(void);
void op_skills_show_level(void);
#endif
