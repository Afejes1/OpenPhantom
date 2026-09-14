#ifndef OP_GAMEPLAY_RANDOM_H
#define OP_GAMEPLAY_RANDOM_H
extern int op_gameplay_random_state;
int op_gameplay_random(void);
int op_gameplay_seed(int);
int op_gameplay_roll(int, unsigned int);
#endif
