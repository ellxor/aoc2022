#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "input/day03input.h"

int main()
{
    clock_t t1 = clock();

    unsigned part1 = 0;
    unsigned part2 = 0;

    uint64_t group[3], index = 0;

    while (*input) {
        size_t length = strcspn(input, "\n");
        
        uint64_t backpack[2] = {0};

        for (unsigned i = 0; i < 2; i++) {
            for (int j = 0; j < length >> 1; j++) {
                char c = input[j];
                /// TODO: do better conversion here to remove id normalisation
                int id = (32 &~ c) + (c &~ 32) - 'A';
                backpack[i] |= 1ull << id;
            }
            input += length >> 1;
        }

        uint64_t bit = backpack[0] & backpack[1];
        unsigned id = __builtin_ctzll(bit);
        id += 1 - 6*((id & 32) >> 5); // normalise id

        part1 += id;

        group[index] = backpack[0] | backpack[1];

        if (index == 2) {
            uint64_t groupID = group[0] & group[1] & group[2];

            unsigned id = __builtin_ctzll(groupID);
            id += 1 - 6*((id & 32) >> 5); // normalise id
            part2 += id;
            index = -1;
        }

        index += 1;
        input += 1;
    }

    clock_t t2 = clock();

    printf("%u\n", part1);
    printf("%u\n", part2);

    printf("Time: %lu\n", t2 - t1);
}
