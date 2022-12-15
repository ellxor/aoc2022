#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "input/day06input.h"

unsigned solve(size_t N)
{
	const char *buffer = input;
	uint32_t prev = 0;
	
	for (int i = 0; i < N; i++)
		prev ^= 1 << (*buffer++ - 'a');

	while (*buffer && __builtin_popcount(prev) != N)
	{
		prev ^= 1 << (*buffer - 'a');
		prev ^= 1 << (*(buffer - N) - 'a');
		++buffer;
	}

	return buffer - input;
}

int main()
{
	clock_t t1 = clock();

	unsigned part1 = solve(4);
	unsigned part2 = solve(14);

	clock_t t2 = clock();

	printf("%u\n", part1);
	printf("%u\n", part2);

	printf("Time: %ld\n", t2 - t1);
}
