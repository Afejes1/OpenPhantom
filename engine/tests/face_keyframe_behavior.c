#include "../src/face_keyframe.h"
#include <stdio.h>
#include <string.h>
static int fk_active, fk_mode;

#include "../src/face_keyframe.h"
#include <stdio.h>
#include <string.h>
static int fi_checks, fi_failures;
static void fi_check(int ok, int line)
{
    ++fi_checks;
    if (!ok)
    {
        ++fi_failures;
        printf("line %d failed\n", line);
    }
}
#define fi_CHECK(x) fi_check(!!(x), __LINE__)
typedef struct fi_OWNED_FACE
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} fi_OWNED_FACE;
static fi_OWNED_FACE fi_faces[4], fi_expected[4];
static unsigned int fi_tokens[4];
static void fi_seed(void)
{
    int i;
    memset(fi_faces, 0x69, sizeof(fi_faces));
    for (i = 0; i < 4; ++i)
    {
        fi_faces[i].value.array18 = &fi_tokens[i];
        fi_faces[i].value.array1c = &fi_tokens[(i + 1) % 4];
    }
    memcpy(fi_expected, fi_faces, sizeof(fi_faces));
}
static void fi_verify(void)
{
    fi_CHECK(memcmp(fi_faces, fi_expected, sizeof(fi_faces)) == 0);
}
static int fk_test_face_init(void)
{
    int i, p;
    for (p = 0; p < 3; ++p)
        for (i = 0; i < 4; ++i)
        {
            fi_seed();
            memset(&fi_faces[i].value, p == 0 ? 0 : (p == 1 ? 0xff : 0x57), sizeof(OP_RD_FACE));
            memcpy(fi_expected, fi_faces, sizeof(fi_faces));
            fi_expected[i].value.field00 = 0;
            fi_expected[i].value.field04 = 0;
            fi_expected[i].value.field14 = 0;
            fi_expected[i].value.array18 = 0;
            fi_expected[i].value.array1c = 0;
            fi_expected[i].value.field20 = 0;
            fi_expected[i].value.field24 = -1;
            memset(fi_expected[i].value.vector38, 0, 12);
            memset(fi_expected[i].value.vector28, 0, 8);
            fi_expected[i].value.field30 = 0;
            fi_expected[i].value.field48 = 0;
            fi_expected[i].value.field44 = 0;
            fi_CHECK(op_face_init(&fi_faces[i].value) == 1);
            fi_verify();
        }
    printf("face_init: %d checks, %d failures\n", fi_checks, fi_failures);
    return fi_failures != 0;
}

#undef fi_CHECK

#include "../src/face_keyframe.h"
#include <stdio.h>
#include <string.h>
static int fa_checks, fa_failures;
static void fa_check(int ok, int line)
{
    ++fa_checks;
    if (!ok)
    {
        ++fa_failures;
        printf("line %d failed\n", line);
    }
}
#define fa_CHECK(x) fa_check(!!(x), __LINE__)
typedef struct fa_OWNED_FACE
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} fa_OWNED_FACE;
static fa_OWNED_FACE fa_faces[4], fa_expected[4];
static unsigned int fa_tokens[4];
static void fa_seed(void)
{
    int i;
    memset(fa_faces, 0x69, sizeof(fa_faces));
    for (i = 0; i < 4; ++i)
    {
        fa_faces[i].value.array18 = &fa_tokens[i];
        fa_faces[i].value.array1c = &fa_tokens[(i + 1) % 4];
    }
    memcpy(fa_expected, fa_faces, sizeof(fa_faces));
}
static void fa_verify(void)
{
    fa_CHECK(memcmp(fa_faces, fa_expected, sizeof(fa_faces)) == 0);
}
static int fa_row, fa_stage, fa_hasfirst, fa_hassecond, fa_mutate, fa_alias, fa_expected_calls;
void fk_array_release(void *p)
{
    fa_CHECK(fa_stage < fa_expected_calls);
    fa_CHECK(p == (fa_hasfirst && !fa_stage
                       ? &fa_tokens[0]
                       : (fa_mutate == 1 && fa_hasfirst ? &fa_tokens[2] : (fa_alias ? &fa_tokens[0] : &fa_tokens[1]))));
    fa_verify();
    if (fa_hasfirst && !fa_stage)
    {
        if (fa_mutate)
            fa_faces[fa_row].value.array1c = fa_expected[fa_row].value.array1c = fa_mutate == 1 ? &fa_tokens[2] : 0;
        fa_faces[fa_row].value.array18 = fa_expected[fa_row].value.array18 = &fa_tokens[3];
    }
    ++fa_stage;
}
static int fk_test_face_free_arrays(void)
{
    for (fa_row = 0; fa_row < 4; ++fa_row)
        for (fa_hasfirst = 0; fa_hasfirst < 2; ++fa_hasfirst)
            for (fa_hassecond = 0; fa_hassecond < 2; ++fa_hassecond)
                for (fa_mutate = 0; fa_mutate < 3; ++fa_mutate)
                    for (fa_alias = 0; fa_alias < 2; ++fa_alias)
                    {
                        fa_seed();
                        fa_faces[fa_row].value.array18 = fa_hasfirst ? &fa_tokens[0] : 0;
                        fa_faces[fa_row].value.array1c = fa_hassecond ? &fa_tokens[fa_alias ? 0 : 1] : 0;
                        memcpy(fa_expected, fa_faces, sizeof(fa_faces));
                        fa_stage = 0;
                        fa_expected_calls = fa_hasfirst + (fa_hasfirst && fa_mutate ? (fa_mutate == 1) : fa_hassecond);
                        op_face_free_arrays(&fa_faces[fa_row].value);
                        fa_CHECK(fa_stage == fa_expected_calls);
                        fa_verify();
                    }
    printf("face_free_arrays: %d checks, %d failures\n", fa_checks, fa_failures);
    return fa_failures != 0;
}

#undef fa_CHECK

#include "../src/face_keyframe.h"
#include <stdio.h>
#include <string.h>
static int kl_checks, kl_failures;
static void kl_check(int ok, int line)
{
    ++kl_checks;
    if (!ok)
    {
        ++kl_failures;
        printf("line %d failed\n", line);
    }
}
#define kl_CHECK(x) kl_check(!!(x), __LINE__)
typedef struct kl_OWNED_FACE
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} kl_OWNED_FACE;
static kl_OWNED_FACE kl_faces[4], kl_expected[4];
static unsigned int kl_tokens[4];
static void kl_seed(void)
{
    int i;
    memset(kl_faces, 0x69, sizeof(kl_faces));
    for (i = 0; i < 4; ++i)
    {
        kl_faces[i].value.array18 = &kl_tokens[i];
        kl_faces[i].value.array1c = &kl_tokens[(i + 1) % 4];
    }
    memcpy(kl_expected, kl_faces, sizeof(kl_faces));
}
static void kl_verify(void)
{
    kl_CHECK(memcmp(kl_faces, kl_expected, sizeof(kl_faces)) == 0);
}
OP_KEYFRAME_LOAD_HOOK op_keyframe_load_hook;
OP_KEYFRAME_UNLOAD_HOOK op_keyframe_unload_hook;
static int kl_invoked;
static OP_KEYFRAME *kl_load_a(char *name)
{
    (void)name;
    ++kl_invoked;
    return 0;
}
static OP_KEYFRAME *kl_load_b(char *name)
{
    (void)name;
    ++kl_invoked;
    return 0;
}
static void kl_unload_a(OP_KEYFRAME *entry)
{
    (void)entry;
    ++kl_invoked;
}
static void kl_unload_b(OP_KEYFRAME *entry)
{
    (void)entry;
    ++kl_invoked;
}
static int fk_test_keyframe_set_load_hook(void)
{
    int old, new, repeat;
    OP_KEYFRAME_LOAD_HOOK choices[3];
    choices[0] = 0;
    choices[1] = kl_load_a;
    choices[2] = kl_load_b;
    for (old = 0; old < 3; ++old)
        for (new = 0; new < 3; ++new)
            for (repeat = 0; repeat < 2; ++repeat)
            {
                kl_seed();
                kl_invoked = 0;
                op_keyframe_load_hook = choices[old];
                op_keyframe_unload_hook = (repeat ? kl_unload_a : kl_unload_b);
                kl_CHECK(op_keyframe_set_load_hook(choices[new]) == choices[old]);
                kl_CHECK(op_keyframe_load_hook == choices[new]);
                kl_CHECK(op_keyframe_unload_hook == (repeat ? kl_unload_a : kl_unload_b));
                if (repeat)
                {
                    kl_CHECK(op_keyframe_set_load_hook(choices[new]) == choices[new]);
                    kl_CHECK(op_keyframe_load_hook == choices[new]);
                }
                kl_CHECK(kl_invoked == 0);
                kl_verify();
            }
    printf("keyframe_set_load_hook: %d checks, %d failures\n", kl_checks, kl_failures);
    return kl_failures != 0;
}

#undef kl_CHECK

#include "../src/face_keyframe.h"
#include <stdio.h>
#include <string.h>
static int ku_checks, ku_failures;
static void ku_check(int ok, int line)
{
    ++ku_checks;
    if (!ok)
    {
        ++ku_failures;
        printf("line %d failed\n", line);
    }
}
#define ku_CHECK(x) ku_check(!!(x), __LINE__)
typedef struct ku_OWNED_FACE
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} ku_OWNED_FACE;
static ku_OWNED_FACE ku_faces[4], ku_expected[4];
static unsigned int ku_tokens[4];
static void ku_seed(void)
{
    int i;
    memset(ku_faces, 0x69, sizeof(ku_faces));
    for (i = 0; i < 4; ++i)
    {
        ku_faces[i].value.array18 = &ku_tokens[i];
        ku_faces[i].value.array1c = &ku_tokens[(i + 1) % 4];
    }
    memcpy(ku_expected, ku_faces, sizeof(ku_faces));
}
static void ku_verify(void)
{
    ku_CHECK(memcmp(ku_faces, ku_expected, sizeof(ku_faces)) == 0);
}

static int ku_invoked;
static OP_KEYFRAME *ku_load_a(char *name)
{
    (void)name;
    ++ku_invoked;
    return 0;
}
static OP_KEYFRAME *ku_load_b(char *name)
{
    (void)name;
    ++ku_invoked;
    return 0;
}
static void ku_unload_a(OP_KEYFRAME *entry)
{
    (void)entry;
    ++ku_invoked;
}
static void ku_unload_b(OP_KEYFRAME *entry)
{
    (void)entry;
    ++ku_invoked;
}
static int fk_test_keyframe_set_unload_hook(void)
{
    int old, new, repeat;
    OP_KEYFRAME_UNLOAD_HOOK choices[3];
    choices[0] = 0;
    choices[1] = ku_unload_a;
    choices[2] = ku_unload_b;
    for (old = 0; old < 3; ++old)
        for (new = 0; new < 3; ++new)
            for (repeat = 0; repeat < 2; ++repeat)
            {
                ku_seed();
                ku_invoked = 0;
                op_keyframe_unload_hook = choices[old];
                op_keyframe_load_hook = (repeat ? ku_load_a : ku_load_b);
                ku_CHECK(op_keyframe_set_unload_hook(choices[new]) == choices[old]);
                ku_CHECK(op_keyframe_unload_hook == choices[new]);
                ku_CHECK(op_keyframe_load_hook == (repeat ? ku_load_a : ku_load_b));
                if (repeat)
                {
                    ku_CHECK(op_keyframe_set_unload_hook(choices[new]) == choices[new]);
                    ku_CHECK(op_keyframe_unload_hook == choices[new]);
                }
                ku_CHECK(ku_invoked == 0);
                ku_verify();
            }
    printf("keyframe_set_unload_hook: %d checks, %d failures\n", ku_checks, ku_failures);
    return ku_failures != 0;
}

#undef ku_CHECK

#include "../src/face_keyframe.h"
#include <stdio.h>
#include <string.h>
static int kh_checks, kh_failures;
static void kh_check(int ok, int line)
{
    ++kh_checks;
    if (!ok)
    {
        ++kh_failures;
        printf("line %d failed\n", line);
    }
}
#define kh_CHECK(x) kh_check(!!(x), __LINE__)
char op_default_resource_name[] = "authored-default-resource-name-for-fixture-only";
typedef struct kh_OWNED_KEYFRAME
{
    unsigned int before;
    OP_KEYFRAME value;
    unsigned int after;
} kh_OWNED_KEYFRAME;
static kh_OWNED_KEYFRAME kh_entries[4], kh_want_entries[4];
static int kh_row, kh_stage, kh_mode;
static char kh_expected_name[sizeof(op_default_resource_name)];
char *op_copy_keyframe_name(char *out, const char *in, unsigned int count)
{
    unsigned int i;
    int ended = 0;
    kh_CHECK(kh_stage == 0 && out == kh_entries[kh_row].value.name && in == op_default_resource_name && count == 31);
    kh_CHECK(memcmp(kh_entries, kh_want_entries, sizeof(kh_entries)) == 0);
    for (i = 0; i < count; ++i)
    {
        char value = 0;
        if (!ended)
            value = in[i];
        if (!value)
            ended = 1;
        out[i] = value;
        kh_want_entries[kh_row].value.name[i] = value;
    }
    if (kh_mode)
    {
        out[31] = 'X';
        kh_want_entries[kh_row].value.name[31] = 'X';
    }
    kh_stage = 1;
    return out;
}
static int fk_test_keyframe_init_header(void)
{
    int fill, len;
    static int lengths[] = {0, 1, 30, 31, 40};
    for (kh_row = 0; kh_row < 4; ++kh_row)
        for (fill = 0; fill < 3; ++fill)
            for (len = 0; len < 5; ++len)
                for (kh_mode = 0; kh_mode < 2; ++kh_mode)
                {
                    memset(kh_entries, fill == 0 ? 0 : (fill == 1 ? 0xff : 0x69), sizeof(kh_entries));
                    memcpy(kh_want_entries, kh_entries, sizeof(kh_entries));
                    memset(&kh_want_entries[kh_row].value, 0, sizeof(OP_KEYFRAME));
                    memset(op_default_resource_name, 'Q', sizeof(op_default_resource_name));
                    op_default_resource_name[lengths[len]] = 0;
                    memcpy(kh_expected_name, op_default_resource_name, sizeof(kh_expected_name));
                    kh_stage = 0;
                    op_keyframe_init_header(&kh_entries[kh_row].value);
                    kh_want_entries[kh_row].value.name[31] = 0;
                    kh_CHECK(kh_stage == 1);
                    kh_CHECK(memcmp(kh_entries, kh_want_entries, sizeof(kh_entries)) == 0);
                    kh_CHECK(memcmp(op_default_resource_name, kh_expected_name, sizeof(kh_expected_name)) == 0);
                }
    printf("keyframe_init_header: %d checks, %d failures\n", kh_checks, kh_failures);
    return kh_failures != 0;
}

#undef kh_CHECK

typedef struct FK_FACE
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} FK_FACE;
static FK_FACE fk_faces[4], fk_expected[4];
static unsigned int fk_tokens[4];
static int fk_row, fk_stage, fk_first, fk_second, fk_mutate, fk_alias, fk_expected_array_calls, fk_checks, fk_failures;
static void fk_check(int ok, int line)
{
    ++fk_checks;
    if (!ok)
    {
        ++fk_failures;
        printf("face connected line%d failed\n", line);
    }
}
#define FK_CHECK(x) fk_check(!!(x), __LINE__)
static void fk_verify(void)
{
    FK_CHECK(memcmp(fk_faces, fk_expected, sizeof(fk_faces)) == 0);
}
static void fk_seed(void)
{
    int i;
    memset(fk_faces, 0x69, sizeof(fk_faces));
    for (i = 0; i < 4; ++i)
    {
        fk_faces[i].value.array18 = &fk_tokens[0];
        fk_faces[i].value.array1c = &fk_tokens[1];
    }
    memcpy(fk_expected, fk_faces, sizeof(fk_faces));
    fk_stage = 0;
}
static void fk_connected_release(void *p)
{
    if (fk_stage < fk_expected_array_calls)
    {
        FK_CHECK(p == (fk_first && !fk_stage
                           ? &fk_tokens[0]
                           : (fk_first && fk_mutate == 1 ? &fk_tokens[2] : &fk_tokens[fk_alias ? 0 : 1])));
        fk_verify();
        if (fk_first && !fk_stage)
        {
            if (fk_mutate)
                fk_faces[fk_row].value.array1c = fk_expected[fk_row].value.array1c = fk_mutate == 1 ? &fk_tokens[2] : 0;
            fk_faces[fk_row].value.array18 = fk_expected[fk_row].value.array18 = &fk_tokens[3];
        }
    }
    else
    {
        FK_CHECK(fk_stage == fk_expected_array_calls && p == &fk_faces[fk_row].value);
        fk_verify();
    }
    ++fk_stage;
}
static void fk_release(void *p)
{
    if (fk_mode == 1)
        fk_array_release(p);
    else
        fk_connected_release(p);
}
static int op_test_face_keyframe(void)
{
    int total = 0, i;
    fk_active = 1;
    fk_mode = 1;
    total += fk_test_face_init();
    total += fk_test_face_free_arrays();
    total += fk_test_keyframe_set_load_hook();
    total += fk_test_keyframe_set_unload_hook();
    total += fk_test_keyframe_init_header();
    fk_mode = 2;
    fk_seed();
    op_face_free(0);
    FK_CHECK(fk_stage == 0);
    fk_verify();
    for (fk_row = 0; fk_row < 4; ++fk_row)
        for (fk_first = 0; fk_first < 2; ++fk_first)
            for (fk_second = 0; fk_second < 2; ++fk_second)
                for (fk_mutate = 0; fk_mutate < 3; ++fk_mutate)
                    for (fk_alias = 0; fk_alias < 2; ++fk_alias)
                    {
                        fk_seed();
                        fk_faces[fk_row].value.array18 = fk_first ? &fk_tokens[0] : 0;
                        fk_faces[fk_row].value.array1c = fk_second ? &fk_tokens[fk_alias ? 0 : 1] : 0;
                        memcpy(fk_expected, fk_faces, sizeof(fk_faces));
                        fk_expected_array_calls = fk_first + (fk_first && fk_mutate ? (fk_mutate == 1) : fk_second);
                        op_face_free(&fk_faces[fk_row].value);
                        FK_CHECK(fk_stage == fk_expected_array_calls + 1);
                        fk_verify();
                    }
    for (i = 0; i < 4; ++i)
    {
        fk_seed();
        fk_row = i;
        FK_CHECK(op_face_init(&fk_faces[i].value) == 1);
        memcpy(fk_expected, fk_faces, sizeof(fk_faces));
        FK_CHECK(fk_faces[i].value.array18 == 0 && fk_faces[i].value.array1c == 0);
        fk_expected_array_calls = 0;
        op_face_free(&fk_faces[i].value);
        FK_CHECK(fk_stage == 1);
        fk_verify();
    }
    fk_active = 0;
    printf("face/keyframe connected: %d checks, %d failures\n",
           fk_checks + fi_checks + fa_checks + kl_checks + ku_checks + kh_checks,
           fk_failures + fi_failures + fa_failures + kl_failures + ku_failures + kh_failures);
    return total + (fk_failures != 0);
}
