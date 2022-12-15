#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "input/day14input.h"

int main()
{
    const clock_t t1 = clock();

    size_t min_x = SIZE_MAX, max_x = 0;
    size_t min_y = SIZE_MAX, max_y = 0;

    static char world[200][600] = {0};

    while (*input) {
        size_t x = strtoul(input, &input, 10); input += 1;
        size_t y = strtoul(input, &input, 10);

        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;

        while (*input != '\n') {
            input += 4;

            size_t nx = strtoul(input, &input, 10); input += 1;
            size_t ny = strtoul(input, &input, 10);

            if (nx < min_x) min_x = nx;
            if (nx > max_x) max_x = nx;
            if (ny < min_y) min_y = ny;
            if (ny > max_y) max_y = ny;

            size_t x_dir = (x < nx) - (x > nx);
            size_t y_dir = (y < ny) - (y > ny);

            for (;;) {
                for (;;) {
                    world[y][x] = 1;
                    if (y == ny) break;
                    y += y_dir;
                }        
                if (x == nx) break;
                x += x_dir;
            }
        }

        input += 1;
    }

    uint64_t part1 = 0;
    uint64_t part2 = 0;

    size_t floor = max_y + 1;

    for (uint64_t N = 0;; N++) {
        size_t x = 500;
        size_t y = 0;

        // simulation
        while (y != floor) {
            while (!world[y+1][x] && y != floor) y += 1;
            if (y == floor) break;
            if (!world[y+1][x-1]) x -= 1, y += 1;
            else if (!world[y+1][x+1]) x += 1, y += 1;
            else break;
        }

        if (y == floor && !part1) part1 = N;
        if (y == 0) { part2 = N + 1; break; }

        world[y][x] = 1;
    }

    const clock_t t2 = clock();

    printf("%lu\n", part1);
    printf("%lu\n", part2);

    printf("Time: %ld\n", t2 - t1);
}
