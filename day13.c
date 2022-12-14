#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "day13input.h"

static inline
uint64_t in_order_recursive(const char **packet1, const char **packet2)
{
    const uint64_t UNDEFINED = 2;

    uint64_t packet1_is_list = **packet1 == '[';
    uint64_t packet2_is_list = **packet2 == '[';

    *packet1 += packet1_is_list;
    *packet2 += packet2_is_list;

    // both lists
    if (packet1_is_list && packet2_is_list) {
        while (**packet1 != ']') {
            if (**packet2 == ']') return 0;
            uint64_t order = in_order_recursive(packet1, packet2);
            if (order != UNDEFINED) return order;
            *packet1 += **packet1 == ',';
            *packet2 += **packet2 == ',';
        }

        if (**packet2 != ']') return 1;

        *packet1 += 1;
        *packet2 += 1;

        return 2;
    }

    // mismatch
    else if (packet1_is_list ^ packet2_is_list) {
        uint64_t order = in_order_recursive(packet1, packet2);

        if (order == UNDEFINED) {
            if (packet1_is_list && **packet1 != ']') order = 0;
            if (packet2_is_list && **packet2 != ']') order = 1;
        }

        while (packet1_is_list) {
            packet1_is_list -= **packet1 == ']';
            *packet1 += 1;
        }

        while (packet2_is_list) {
            packet2_is_list -= **packet2 == ']';
            *packet2 += 1;
        }

        return order;
    }

    // both numbers
    else {
        uint64_t a = strtoul(*packet1, packet1, 10);
        uint64_t b = strtoul(*packet2, packet2, 10);
        return (a == b) ? UNDEFINED : (a < b);
    } 
}

static inline
uint64_t in_order(const char *packet1, const char *packet2) {
    return in_order_recursive(&packet1, &packet2);
}

int main()
{
    const clock_t t1 = clock();

    uint64_t part1 = 0;
    uint64_t less2 = 1, less6 = 2;
    uint64_t index = 1;

    while (*input) {
        const char *packet1 = input; input += strcspn(input, "\n") + 1;
        const char *packet2 = input; input += strcspn(input, "\n") + 1;

        part1 += index &- in_order(packet1, packet2);
        index += 1;

        less2 += in_order(packet1, "[[2]]");
        less2 += in_order(packet2, "[[2]]");
        less6 += in_order(packet1, "[[6]]");
        less6 += in_order(packet2, "[[6]]");

        input += 1; // skip newline
    } 

    uint64_t part2 = less2 * less6;

    const clock_t t2 = clock();

    printf("%lu\n", part1);
    printf("%lu\n", part2);
    printf("Time: %ld\n", t2 - t1);
}
