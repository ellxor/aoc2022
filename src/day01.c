#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "input/day01input.h"

/// TODO: create aoc library with useful functions
static inline unsigned max(unsigned a, unsigned b) { return (a > b) ? a : b; }
static inline void swap(unsigned *a, unsigned *b) { unsigned t = *a; *a = *b; *b = t; }

int main()
{
    clock_t t1 = clock();

    unsigned total = 0, m1 = 0, m2 = 0, m3 = 0;

    for (; *input; input++) {
        if (*input == '\n') {
            m3 = max(m3, total);

            // unrolled insertion sort
            if (m3 > m2) swap(&m2, &m3);
            if (m2 > m1) swap(&m1, &m2);

            total = 0;
            continue;
        }

        /// TODO: implement faster parser (simdjson style)
        total += strtoul(input, &input, 10);
    }

    unsigned part1 = m1;
    unsigned part2 = m1 + m2 + m3;

    clock_t t2 = clock();

    printf("%u\n", part1);
    printf("%u\n", part2);

    printf("time: %ld\n", t2 - t1);
}
