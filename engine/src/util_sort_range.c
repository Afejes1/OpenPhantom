#include "utilities.h"
// FUNCTION: WMAIN 0x00497656
void op_util_sort_range(int low, int high, OP_UTIL_COMPARE compare, OP_UTIL_SWAP swap, void *context)
{
    int right;
    int pivot;
    int left;
    if (high - low < 1)
        return;
    if (high - low == 1)
    {
        if (compare(low, high, context) > 0)
            swap(low, high, context);
        return;
    }
    pivot = low;
    left = low + 1;
    right = high;
    while (left < right)
    {
        while (left < right && compare(left, pivot, context) <= 0)
            ++left;
        while (left < right && compare(pivot, right, context) < 0)
            --right;
        if (left < right)
            swap(left, right, context);
    }
    if (compare(pivot, left, context) > 0)
    {
        swap(pivot, left, context);
        ++right;
    }
    else
        --left;
    if (left - low < high - right)
    {
        op_util_sort_range(low, left, compare, swap, context);
        op_util_sort_range(right, high, compare, swap, context);
    }
    else
    {
        op_util_sort_range(right, high, compare, swap, context);
        op_util_sort_range(low, left, compare, swap, context);
    }
}
