#include "api.h"
#include <limits.h>
#include <stdio.h>
static int checks, failures, calls;
static unsigned int expected;
static void check(int ok, const char *text) { ++checks; if (!ok) { ++failures; printf("FAIL: %s\n", text); } }
void op_sound_release_slot(unsigned int slot) { ++calls; check(slot == expected, "slot bits forwarded unchanged"); }
static void run(unsigned int slot) { expected = slot; calls = 0; op_sound_stop_channel(slot); check(calls == 1, "exactly one release call"); }
int main(void) { run(0); run(11); run(12); run(0x80000000U); run(UINT_MAX); printf("Sound stop channel: %d checks, %d failures\n", checks, failures); return failures != 0; }