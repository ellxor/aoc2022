#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "input/day08input.h"

static inline size_t max(size_t a, size_t b) { return a > b ? a : b; }

int main()
{
    const clock_t t1 = clock();

    const size_t width = strcspn(input, "\n");
    const size_t true_width = width + 1;
    size_t height = 0;

    const char *scanner = input;

    for (const char *scanner = input; scanner[1]; scanner += true_width)
        ++height;

    uint64_t part1 = 0;
    uint64_t part2 = 0;

    for (size_t col = 0; col < width; col++) {
        for (size_t row = 0; row < height; row++) {
            size_t dir[4][3] = {
                 1,  0,  1,
                -1,  0, -1,
                 0, -1, -true_width,
                 0,  1,  true_width,
            };

            uint64_t reach[4] = {0};
            uint64_t visible = 0;

            for (size_t i = 0; i < 4; i++) {
                size_t x = col + dir[i][0], y = row + dir[i][1];
                size_t index = row * true_width + col;

                char h = input[index], v = 1;
                index += dir[i][2];

                while (0 <= x && x < width && 0 <= y && y < height) {
                    ++reach[i];
                    if (input[index] >= h) { v = 0; break; }
                    x += dir[i][0];
                    y += dir[i][1];
                    index += dir[i][2];
                }

                visible |= v;
            }

            part1 += visible;
            part2 = max(part2, reach[0] * reach[1] * reach[2] * reach[3]);
        }
    }

    const clock_t t2 = clock();

    printf("%lu\n", part1);
    printf("%lu\n", part2);
    printf("Time: %ld\n", t2 - t1);
}
