#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "day02input.h"

int main()
{
	clock_t t1 = clock();

	unsigned part1 = 0;
	unsigned part2 = 0;

	/// TODO: use simd to make loop faster (compiler aleady does this but we can make it faster)

	for (unsigned i = 0; i < LINES; i++) {
		uint8_t opp = input[0] - 'A';
		uint8_t you = input[2] - 'X';

		/// TODO: logic needs cleanup

		// part 1
		uint8_t win  = ((0x9u >> (2 - you)) & 0x3u) == opp;
		uint8_t draw = opp == you;

		unsigned score = (you + 1) + 6*win + 3*draw;
		part1 += score;

		// part 2
		switch (you) {
			case 0: part2 += 1 + ((0x9u >> (2 - opp)) & 0x3u); break;
			case 1: part2 += 4 + opp; break;
			case 2: part2 += 7 + ((0x1u << opp) & 0x3u); break;
			default: __builtin_unreachable();
		}

		input += 4;
	}

	clock_t t2 = clock();

	printf("%u\n", part1);
	printf("%u\n", part2);

	printf("time: %ld\n", t2 - t1);
}
