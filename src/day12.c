#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "input/day12input.h"

static
void solve(size_t x, size_t y,
           const size_t width, const size_t height,
           uint64_t map[width][height])
{
    size_t h = input[x + y * (width + 1)];

    for (ssize_t dx = -1; dx <= 1; dx++) {
        for (ssize_t dy = -1; dy <= 1; dy++) {
            size_t nx = x + dx;
            size_t ny = y + dy;

            if ((!dx ^ !dy) && (0 <= nx && nx < width)
                       && (0 <= ny && ny < height))
            {
                size_t nh = input[nx + ny * (width + 1)];
                size_t v = 1 + map[x][y];

                if ((nh >= h - 1) && (v < map[nx][ny]))
                {
                    map[nx][ny] = v;
                    solve(nx, ny, width, height, map);
                }
            }
        }
    }
}

int main()
{
    const clock_t t1 = clock();

    size_t width = strcspn(input, "\n");
    size_t height = 0;

    size_t t_width = width + 1;

    for (const char *scanner = input; scanner[1]; scanner += t_width)
        ++height;

    size_t start[2] = {0};
    size_t end[2] = {0};

    for (char *scanner = input; *scanner; ++scanner) {
        size_t i = scanner - input;

        switch (*scanner) {
            case 'S': {
                start[0] = i % t_width;
                start[1] = i / t_width;
                *scanner = 'a';
                break;
            }

            case 'E': {
                end[0] = i % t_width;
                end[1] = i / t_width;
                *scanner = 'z';
                break;
            }
        }
    }

    uint64_t map[width][height];
    memset(map, 0xFF, sizeof map);
    map[end[0]][end[1]] = 0;

    solve(end[0], end[1], width, height, map);

    uint64_t part1 = map[start[0]][start[1]];
    uint64_t part2 = UINT64_MAX;

    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            size_t i = x + y * (width + 1);

            if (input[i] == 'a' && map[x][y] < part2)
                part2 = map[x][y];
        }
    }

    const clock_t t2 = clock();

    printf("%lu\n", part1);
    printf("%lu\n", part2);
    printf("Time: %ld\n", t2 - t1);
}
