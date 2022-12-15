#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "input/day15input.h"

#define constexpr_strlen(s) (sizeof(s) - 1)

int main()
{
    clock_t t1 = clock();
    clock_t t2;

    size_t beacons = 0;
    static int64_t range[LINES][2];
    static int64_t beacon[LINES];
    static int64_t sensor[LINES][2];
    static int64_t radius[LINES];

    for (size_t line = 0; line < LINES; ++line) {
        // parse input
        input += constexpr_strlen("Sensor at x="); 
        int64_t sx = strtol(input, &input, 10); input += constexpr_strlen(", y=");
        int64_t sy = strtol(input, &input, 10); input += constexpr_strlen(": closest beacon is at x=");
        int64_t bx = strtol(input, &input, 10); input += constexpr_strlen(", y=");
        int64_t by = strtol(input, &input, 10); input += 1; // skip newline

        const int64_t row = IS_SAMPLE ? 10 : 2000000;

        // compute manhatten distance, and horizontal coverage on `row`
        int64_t bdist = labs(sx - bx) + labs(sy - by);
        int64_t rdist = bdist - labs(sy - row);

        // range [a,b] that must be empty
        int64_t a = sx - rdist;
        int64_t b = sx + rdist;

        // iterate over previous ranges to remove overlap
        for (size_t i = 0; i < line; i++) {
            // if previous range is a subset of this range, eliminate it
            if (a <= range[i][0] && range[i][1] <= b) {
                range[i][0] = INT64_MAX; //use this value to mark as invalid
                range[i][1] = INT64_MAX;
            }
            
            // do the same if this range is a subet
            else if (range[i][0] <= a && b <= range[i][1]) {
                a = INT64_MAX; b = INT64_MAX;
            }

            // overlapping ranges
            else {
                if (a <= range[i][0] && range[i][0] <= b) range[i][0] = b + 1;
                if (a <= range[i][1] && range[i][1] <= b) range[i][1] = a - 1;
                if (range[i][0] <= a && a <= range[i][1]) a = range[i][1] + 1;
                if (range[i][0] <= b && b <= range[i][1]) b = range[i][0] - 1;
            }
        }

        range[line][0] = a;
        range[line][1] = b;

        // construct beacon set (on row)
        if (by == row) {
            size_t index;

            for (index = 0; index < beacons; index++)
                if (beacon[index] == bx) break;

            beacon[index] = bx;
            beacons += (index == beacons);
        }

        sensor[line][0] = sx;
        sensor[line][1] = sy;
        radius[line] = bdist;
    }

    uint64_t part1 = 0;
    uint64_t part2 = 0;

    for (size_t i = 0; i < LINES; i++)
        if (range[i][0] != INT64_MAX)
            part1 += range[i][1] - range[i][0] + 1;

    part1 -= beacons;

    const int64_t limit = IS_SAMPLE ? 20 : 4000000;

    for (size_t i = 0; i < LINES; ++i) {
        int64_t rad = radius[i] + 1;

        int64_t top_row = sensor[i][1] - rad;
        int64_t bot_row = sensor[i][1] + rad;

        if (top_row < 0) top_row = 0;
        if (bot_row > limit) bot_row = limit;

        for (int64_t y = top_row; y <= bot_row; y++) {
            int64_t delta = rad - labs(sensor[i][1] - y);
            int64_t xs[2] = { sensor[i][0] - delta, sensor[i][0] + delta };

            for (size_t j = 0; j < 2; ++j) {
                int64_t x = xs[j];

                if (0 <= x && x <= limit) {
                    size_t found = 1;

                    for (size_t k = 0; k < LINES; ++k) {
                        int64_t ox = sensor[k][0];        
                        int64_t oy = sensor[k][1];

                        int64_t mdist = labs(x - ox) + labs(y - oy);
                        if (mdist <= radius[k]) { found = 0; break; }
                    }

                    if (found) {
                        part2 = 4000000 * x + y;
                        goto end;
                    }
                }
            }
        }
    }

end:
    t2 = clock();

    printf("%lu\n", part1);
    printf("%lu\n", part2);
    printf("Time: %ld\n", t2 - t1);
}
