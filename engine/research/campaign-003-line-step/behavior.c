#include "api.h"
#include <stdio.h>
#include <string.h>

static int checks;
static int failures;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static void run_once(int error, int index, int count, int expected_return,
                     const char *message)
{
    op_scan_line before;
    op_scan_line line;
    int use_diagonal = error >= 0;
    int expected_x_step;
    int expected_y_step;
    int expected_error_step;

    memset(&line, 0x3c, sizeof(line));
    line.count = count;
    line.index = index;
    line.x = 17;
    line.y = -9;
    line.step_x = 123;
    line.step_y = -456;
    line.error = error;
    line.diagonal_x = -3;
    line.diagonal_y = 5;
    line.diagonal_error = -11;
    line.straight_x = 7;
    line.straight_y = -2;
    line.straight_error = 13;
    before = line;

    expected_x_step = use_diagonal ? before.diagonal_x : before.straight_x;
    expected_y_step = use_diagonal ? before.diagonal_y : before.straight_y;
    expected_error_step = use_diagonal ? before.diagonal_error : before.straight_error;
    check(op_step_scan_line(&line) == expected_return, message);
    check(line.index == before.index + 1, "index increments");
    check(line.step_x == expected_x_step && line.step_y == expected_y_step,
          "selected step fields");
    check(line.x == before.x + expected_x_step &&
          line.y == before.y + expected_y_step, "position update");
    check(line.error == before.error + expected_error_step, "error update");
    check(line.count == before.count && line.start_x == before.start_x &&
          line.start_y == before.start_y && line.end_x == before.end_x &&
          line.end_y == before.end_y &&
          line.major_delta == before.major_delta &&
          line.minor_delta == before.minor_delta &&
          line.diagonal_x == before.diagonal_x &&
          line.diagonal_y == before.diagonal_y &&
          line.diagonal_error == before.diagonal_error &&
          line.straight_x == before.straight_x &&
          line.straight_y == before.straight_y &&
          line.straight_error == before.straight_error &&
          line.unused_4c == before.unused_4c, "untouched fields");
}

int main(void)
{
    op_scan_line line;
    int first_return;
    int second_return;

    run_once(-2, 1, 5, 0, "updated index remains below count");
    run_once(-1, 3, 4, 0, "negative error and equality return");
    run_once(0, 4, 4, 1, "zero error and count-plus-one return");
    run_once(6, 9, 3, 1, "positive error already beyond count");

    memset(&line, 0, sizeof(line));
    line.count = 1;
    line.index = 0;
    line.x = 2;
    line.y = 4;
    line.error = -2;
    line.diagonal_x = 10;
    line.diagonal_y = 20;
    line.diagonal_error = -100;
    line.straight_x = -1;
    line.straight_y = 3;
    line.straight_error = 5;
    first_return = op_step_scan_line(&line);
    second_return = op_step_scan_line(&line);
    check(first_return == 0 && second_return == 1, "repeated returns");
    check(line.index == 2 && line.x == 11 && line.y == 27,
          "repeated branch trajectory");
    check(line.step_x == 10 && line.step_y == 20 && line.error == -97,
          "repeated selected state");

    printf("Line step: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
