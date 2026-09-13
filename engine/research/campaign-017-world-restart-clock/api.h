#ifndef OP_CAMPAIGN_017_WORLD_RESTART_CLOCK_API_H
#define OP_CAMPAIGN_017_WORLD_RESTART_CLOCK_API_H
typedef struct OP_WORLD { unsigned char a[0x50]; unsigned int timestamp; unsigned char b[4]; unsigned int mirror; unsigned int elapsed; } OP_WORLD;
unsigned int op_clock_ticks(void);
void op_world_clock_hook(OP_WORLD *world);
void op_world_restart_clock(OP_WORLD *world);
#endif
