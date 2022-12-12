#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "day11input.h"

const uint64_t OP_ADD = 0;
const uint64_t OP_MUL = 1;
const uint64_t SELF = UINT64_MAX;

#define constexpr_strlen(str) (sizeof(str)-1)

typedef struct {
    uint64_t items[2][50], count[2];
    uint64_t op_type, op_arg;
    uint64_t mod, true, false;
    uint64_t visits[2];
} Monkey;

static Monkey monkey[8] = {0};

static inline
void solve(size_t part)
{
    uint64_t prod = 1;

    for (int i = 0; i < 8; i++) {
        prod *= monkey[i].mod;
    }

    for (int ROUNDS = 0; ROUNDS < (part ? 10000 : 20); ROUNDS++) {
        // foreach monkey
        for (int i = 0; i < 8; i++) {
            // foreach item
            for (uint64_t j = 0; j < monkey[i].count[part]; j++) {
                uint64_t worry = monkey[i].items[part][j];

                uint64_t arg = monkey[i].op_arg;
                if (arg == SELF) arg = worry;

                worry = (monkey[i].op_type == OP_ADD) ? worry + arg : worry * arg;
                worry = part ? (worry % prod) : (worry / 3);

                uint64_t to = ((worry % monkey[i].mod) == 0) ? monkey[i].true : monkey[i].false;
                monkey[to].items[part][monkey[to].count[part]++] = worry;
            }

            monkey[i].visits[part] += monkey[i].count[part];
            monkey[i].count[part] = 0;
        }
    }
}

int main()
{
    const clock_t t1 = clock();

    for (int i = 0; i < 8; i++) {
        input += constexpr_strlen("Monkey _:\n  Starting items: ");

        for (;;) {
            monkey[i].items[0][monkey[i].count[0]++] =
            monkey[i].items[1][monkey[i].count[1]++] = strtoul(input, &input, 10);
            if (*input == '\n') break;
            input += constexpr_strlen(", ");
        }

        input += constexpr_strlen("\n  Operation: new = old ");
        monkey[i].op_type = (*input == '*');
        input += constexpr_strlen("_ ");
        
        if (*input == 'o') {
            monkey[i].op_arg = SELF;
            input += constexpr_strlen("old");
        } else {
            monkey[i].op_arg = strtoull(input, &input, 10);
        }

        input += constexpr_strlen("\n  Test: divisible by ");
        monkey[i].mod = strtoull(input, &input, 10);

        input += constexpr_strlen("\n    If true: throw to monkey ");
        monkey[i].true = *input - '0';
        input += constexpr_strlen("_\n    If false: throw to monkey ");
        monkey[i].false = *input - '0';

        input += constexpr_strlen("_\n\n");
    }

    solve(0);
    solve(1);

    uint64_t part1[2] = {0}, part2[2] = {0};

    for (int i = 0; i < 8; i++) {
        uint64_t u = monkey[i].visits[0];
        uint64_t v = monkey[i].visits[1];

        if (u > part1[1]) { part1[0] = part1[1]; part1[1] = u; }
        else if (u > part1[0]) part1[0] = u;

        if (v > part2[1]) { part2[0] = part2[1]; part2[1] = v; }
        else if (v > part2[0]) part2[0] = v;
    }

    const clock_t t2 = clock();

    printf("%lu\n", part1[0] * part1[1]);
    printf("%lu\n", part2[0] * part2[1]);
    printf("Time: %ld\n", t2 - t1);
}