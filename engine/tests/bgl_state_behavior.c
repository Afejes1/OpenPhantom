#include "../src/bgl_state.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef char op_bgl_matrix_size_is_48[(sizeof(OP_BGL_MATRIX) == 48) ? 1 : -1];
typedef char op_bgl_words_start_at_zero[(offsetof(OP_BGL_MATRIX, words) == 0) ? 1 : -1];
typedef char op_bgl_words_are_48_bytes[(sizeof(((OP_BGL_MATRIX *)0)->words) == 48) ? 1 : -1];

OP_BGL_MATRIX op_bgl_stack[64];
OP_BGL_MATRIX *op_bgl_current;
int op_bgl_depth;
/* Synthetic nonuniform copy oracle, not a recovered original data table. */
const OP_BGL_MATRIX op_bgl_identity_matrix = {{
    0x01234567U, 0x89abcdefU, 0x80000000U, 0x7fc12345U,
    0x10203040U, 0x50607080U, 0xabcdef01U, 0xfedcba98U,
    0x13579bdfU, 0x2468ace0U, 0x00000001U, 0xffffffffU
}};

static int bgl_checks;
static int bgl_failures;

static void bgl_check(int condition, const char *description)
{
    ++bgl_checks;
    if (!condition) {
        ++bgl_failures;
        printf("FAIL: %s\n", description);
    }
}

static void bgl_fill_matrix(OP_BGL_MATRIX *matrix, unsigned int seed)
{
    int i;
    for (i = 0; i < 12; ++i)
        matrix->words[i] = seed + (unsigned int)i;
}

static void bgl_fill_stack(unsigned int seed)
{
    int i;
    int j;
    for (i = 0; i < 64; ++i)
        for (j = 0; j < 12; ++j)
            op_bgl_stack[i].words[j] = seed + (unsigned int)(i * 100 + j);
}

static void bgl_test_linked_sequence(void)
{
    OP_BGL_MATRIX source;
    OP_BGL_MATRIX output;
    OP_BGL_MATRIX expected_stack[64];
    unsigned int translation[5] = {
        0x11111111U, 0x22222222U, 0x33333333U, 0x44444444U, 0x55555555U
    };

    bgl_fill_stack(0x10000000U);
    bgl_fill_matrix(&source, 0x71000000U);
    source.words[0] = 0x80000000U;
    source.words[1] = 0x7fc12345U;
    source.words[9] = 0x90123456U;
    source.words[10] = 0xa1234567U;
    source.words[11] = 0xb2345678U;
    memcpy(expected_stack, op_bgl_stack, sizeof(expected_stack));

    op_bgl_current = &op_bgl_stack[0];
    op_bgl_depth = 0;
    op_bgl_load_matrix(&source);
    expected_stack[0] = source;
    bgl_check(memcmp(op_bgl_stack, expected_stack, sizeof(expected_stack)) == 0,
          "load changes only current matrix");

    memset(&output, 0xa5, sizeof(output));
    op_bgl_get_matrix(&output);
    bgl_check(memcmp(&output, &source, sizeof(output)) == 0,
          "get returns complete loaded matrix");

    op_bgl_push();
    expected_stack[1] = source;
    bgl_check(op_bgl_depth == 1 && op_bgl_current == &op_bgl_stack[1],
          "push advances shared state");
    bgl_check(memcmp(op_bgl_stack, expected_stack, sizeof(expected_stack)) == 0,
          "push copies all 48 bytes from indexed stack source");

    op_bgl_identity();
    expected_stack[1] = op_bgl_identity_matrix;
    bgl_check(memcmp(op_bgl_stack, expected_stack, sizeof(expected_stack)) == 0,
          "identity replaces only pushed current matrix");

    op_bgl_get_translation(&translation[1]);
    bgl_check(translation[0] == 0x11111111U &&
          translation[1] == op_bgl_identity_matrix.words[9] &&
          translation[2] == op_bgl_identity_matrix.words[10] &&
          translation[3] == op_bgl_identity_matrix.words[11] &&
          translation[4] == 0x55555555U,
          "translation getter writes exact three words and preserves neighbors");

    op_bgl_pop();
    bgl_check(op_bgl_depth == 0 && op_bgl_current == &op_bgl_stack[0],
          "pop returns to loaded matrix");
    bgl_check(memcmp(op_bgl_stack, expected_stack, sizeof(expected_stack)) == 0,
          "pop does not change matrix storage");
    bgl_check(memcmp(op_bgl_current, &source, sizeof(source)) == 0,
          "loaded matrix remains beneath pushed identity");
}

static void bgl_test_push_source_and_guards(void)
{
    OP_BGL_MATRIX alternate[3];
    OP_BGL_MATRIX before_stack[64];
    OP_BGL_MATRIX before_alternate[3];
    OP_BGL_MATRIX expected;

    bgl_fill_stack(0x20000000U);
    memset(alternate, 0x6b, sizeof(alternate));
    op_bgl_stack[2].words[0] = 0x80000000U;
    op_bgl_stack[2].words[1] = 0x7fc12345U;
    expected = op_bgl_stack[2];
    memcpy(before_stack, op_bgl_stack, sizeof(before_stack));
    memcpy(before_alternate, alternate, sizeof(before_alternate));
    before_alternate[1] = expected;
    op_bgl_depth = 2;
    op_bgl_current = &alternate[0];
    op_bgl_push();
    bgl_check(op_bgl_depth == 3 && op_bgl_current == &alternate[1],
          "push current pointer advances independently of stack source");
    bgl_check(memcmp(&alternate[1], &expected, sizeof(expected)) == 0,
          "push captures stack indexed by old depth");
    bgl_check(memcmp(op_bgl_stack, before_stack, sizeof(before_stack)) == 0,
          "alternate push preserves complete source stack");
    bgl_check(memcmp(alternate, before_alternate, sizeof(before_alternate)) == 0,
          "alternate push preserves neighboring matrices");

    bgl_fill_stack(0x30000000U);
    memcpy(before_stack, op_bgl_stack, sizeof(before_stack));
    op_bgl_depth = 63;
    op_bgl_current = &op_bgl_stack[20];
    op_bgl_push();
    bgl_check(op_bgl_depth == 63 && op_bgl_current == &op_bgl_stack[20],
          "push depth 63 guard preserves state");
    bgl_check(memcmp(op_bgl_stack, before_stack, sizeof(before_stack)) == 0,
          "push depth 63 guard preserves full stack");

    op_bgl_depth = 64;
    op_bgl_current = 0;
    op_bgl_push();
    bgl_check(op_bgl_depth == 64 && op_bgl_current == 0,
          "push above bound skips null current");

    op_bgl_depth = 0;
    op_bgl_current = 0;
    op_bgl_pop();
    bgl_check(op_bgl_depth == 0 && op_bgl_current == 0,
          "pop depth zero guard skips null current");
    op_bgl_depth = -1;
    op_bgl_pop();
    bgl_check(op_bgl_depth == -1 && op_bgl_current == 0,
          "pop negative depth guard preserves state");
}

static void bgl_test_copy_provenance(void)
{
    OP_BGL_MATRIX source[3];
    OP_BGL_MATRIX destination[3];
    OP_BGL_MATRIX before_source[3];
    OP_BGL_MATRIX expected_destination[3];

    memset(source, 0x5a, sizeof(source));
    memset(destination, 0xa5, sizeof(destination));
    bgl_fill_matrix(&source[1], 0x42000000U);
    source[1].words[0] = 0x80000000U;
    source[1].words[1] = 0x7fc12345U;
    memcpy(before_source, source, sizeof(before_source));
    memcpy(expected_destination, destination, sizeof(expected_destination));
    expected_destination[1] = source[1];
    op_bgl_current = &destination[1];
    op_bgl_depth = 17;
    op_bgl_load_matrix(&source[1]);
    bgl_check(memcmp(source, before_source, sizeof(source)) == 0,
          "load preserves complete disjoint source");
    bgl_check(memcmp(destination, expected_destination, sizeof(destination)) == 0,
          "load preserves destination neighbors");

    memset(source, 0x3c, sizeof(source));
    memcpy(expected_destination, source, sizeof(source));
    expected_destination[1] = destination[1];
    op_bgl_get_matrix(&source[1]);
    bgl_check(memcmp(source, expected_destination, sizeof(source)) == 0,
          "get preserves output neighbors and copies complete current");
    bgl_check(op_bgl_current == &destination[1] && op_bgl_depth == 17,
          "load/get preserve current and depth");
}

static void bgl_test_translation_aliases(void)
{
    OP_BGL_MATRIX matrix;
    OP_BGL_MATRIX expected;
    unsigned int same;
    unsigned int outputs[3];
    int mask;

    bgl_fill_matrix(&matrix, 0x51000000U);
    matrix.words[9] = 0x80000000U;
    matrix.words[10] = 0x7fc12345U;
    matrix.words[11] = 0xffffffffU;
    expected = matrix;
    op_bgl_depth = -7;
    op_bgl_current = 0;
    op_bgl_get_translation_xyz(0, 0, 0);
    bgl_check(op_bgl_current == 0 && op_bgl_depth == -7,
          "all-null XYZ does not dereference current");

    for (mask = 0; mask < 8; ++mask) {
        outputs[0] = 0x11U;
        outputs[1] = 0x22U;
        outputs[2] = 0x33U;
        op_bgl_current = &matrix;
        op_bgl_get_translation_xyz((mask & 1) ? &outputs[0] : 0,
                                   (mask & 2) ? &outputs[1] : 0,
                                   (mask & 4) ? &outputs[2] : 0);
        bgl_check(outputs[0] == ((mask & 1) ? 0x80000000U : 0x11U) &&
              outputs[1] == ((mask & 2) ? 0x7fc12345U : 0x22U) &&
              outputs[2] == ((mask & 4) ? 0xffffffffU : 0x33U),
              "XYZ null combination uses independent oracle");
        bgl_check(memcmp(&matrix, &expected, sizeof(matrix)) == 0,
              "XYZ null combination preserves matrix");
    }

    same = 0;
    op_bgl_get_translation_xyz(&same, &same, &same);
    bgl_check(same == 0xffffffffU, "fully aliased XYZ leaves final Z value");

    matrix.words[9] = 1;
    matrix.words[10] = 2;
    matrix.words[11] = 3;
    expected = matrix;
    expected.words[9] = 1;
    expected.words[10] = 1;
    expected.words[11] = 1;
    op_bgl_get_translation_xyz(&matrix.words[10], &matrix.words[11],
                               &matrix.words[9]);
    bgl_check(memcmp(&matrix, &expected, sizeof(matrix)) == 0,
          "XYZ overlapping outputs observe live writes in order");

    bgl_fill_matrix(&matrix, 0x61000000U);
    matrix.words[9] = 0x91U;
    matrix.words[10] = 0xa2U;
    matrix.words[11] = 0xb3U;
    expected = matrix;
    expected.words[8] = 0x91U;
    expected.words[9] = 0xa2U;
    expected.words[10] = 0xb3U;
    op_bgl_get_translation(&matrix.words[8]);
    bgl_check(memcmp(&matrix, &expected, sizeof(matrix)) == 0,
          "three-word getter supports valid overlapping destination");
}

int op_test_bgl_state(void)
{
    bgl_checks = 0;
    bgl_failures = 0;
    bgl_check(sizeof(OP_BGL_MATRIX) == 48, "matrix size");
    bgl_check(offsetof(OP_BGL_MATRIX, words) == 0, "matrix words offset");
    bgl_check(sizeof(op_bgl_stack) == 64U * 48U, "stack extent");
    bgl_test_linked_sequence();
    bgl_test_push_source_and_guards();
    bgl_test_copy_provenance();
    bgl_test_translation_aliases();
    printf("BGL state integration: %d checks, %d failures\n", bgl_checks, bgl_failures);
    return bgl_failures;
}
