#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "day04input.h"

int main()
{
    unsigned part1 = 0;
    unsigned part2 = 0;

    clock_t t1 = clock();

    while (*input) {
        /// TODO: faster simdjson style parser
        unsigned a = strtoul(input, &input, 10); input += 1; // skip '-'
        unsigned b = strtoul(input, &input, 10); input += 1; // skip ','
        unsigned c = strtoul(input, &input, 10); input += 1; // skip '-'
        unsigned d = strtoul(input, &input, 10); input += 1; // skip newline

        part1 += (a <= c && d <= b) || (c <= a && b <= d);
        part2 += !(b < c || d < a);
    }

    clock_t t2 = clock();

    printf("%u\n", part1);
    printf("%u\n", part2);

    printf("Time: %ld\n", t2 - t1);
}