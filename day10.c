#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "day10input.h"

int main()
{
    const clock_t t1 = clock();

    int64_t part1 = 0;

    int64_t cpu_reg = 1;
    int64_t cpu_clock = 0;

    static char CRT[40 * 6] = { [0 ... 40*6-1] = '.' };

    while (*input) {
        char instr = input[0];
        input += 4; // skip instr

        if (cpu_reg - 1 <= cpu_clock % 40 && cpu_clock % 40 <= cpu_reg + 1)
            CRT[cpu_clock] = '#';

        ++cpu_clock;

        if (cpu_clock % 40 == 20)
            part1 += cpu_reg * cpu_clock; 

        if (instr == 'a') {
            if (cpu_reg - 1 <= cpu_clock % 40 && cpu_clock % 40 <= cpu_reg + 1)
                CRT[cpu_clock] = '#';

            ++cpu_clock;

            if (cpu_clock % 40 == 20)
                part1 += cpu_reg * cpu_clock;

            cpu_reg += strtoll(input, &input, 10);
        }

        input += 1;
    }

    const clock_t t2 = clock();

    printf("%ld\n", part1);

    for (int line = 0; line < 6; line++)
        printf("%.40s\n", CRT + 40*line);

    printf("Time: %ld\n", t2 - t1);
}