#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "input/day09input.h"

int main()
{
    const clock_t t1 = clock();

    static char visited[400][400][2];
    const size_t KNOTC = 10;

    static struct { int x, y; } knots[KNOTC] = { [0 ... KNOTC-1] = {200,200} };

    uint64_t part1 = 0;
    uint64_t part2 = 0;

    while (*input) {
        char dir = input[0];

        char h = __builtin_popcount(dir) == 3;
        char d = ((dir >> 3) & 2) - 1;

        int dx = h ? d : 0;
        int dy = h ? 0 : d;

        unsigned count = strtoul(input + 2, &input, 10);
        input += 1; // skip new line

        for (unsigned j = 0; j < count; j++) {
            knots[0].x += dx;
            knots[0].y += dy;

            for (size_t i = 1; i < KNOTC; i++) {
                int dx = knots[i].x - knots[i-1].x;
                int dy = knots[i].y - knots[i-1].y;

                if (dx*dx + dy*dy > 2) {
                    knots[i].x = knots[i-1].x + dx / 2;
                    knots[i].y = knots[i-1].y + dy / 2;
                }

                else break; // early exit if knot didn't move
            }

            part1 += 1 ^ visited[knots[1].x][knots[1].y][0];
            part2 += 1 ^ visited[knots[9].x][knots[9].y][1];

            visited[knots[1].x][knots[1].y][0] = 1;
            visited[knots[9].x][knots[9].y][1] = 1;
        }
    }

    const clock_t t2 = clock();

    printf("%lu\n", part1);
    printf("%lu\n", part2);
    printf("Time: %ld\n", t2 - t1);
}
