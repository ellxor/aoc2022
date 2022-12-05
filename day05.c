#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "day05input.h"

static char STACKS[NSTACKS*2][100];
static int STACK_SZ[NSTACKS*2];

static inline void push(size_t stack_index, char v) {
	STACKS[stack_index][STACK_SZ[stack_index]++] = v;
}

static inline char pop(size_t stack_index) {
	return STACKS[stack_index][--STACK_SZ[stack_index]];
}

static inline void move_chunk(size_t count, size_t from, size_t to) {
	STACK_SZ[from] -= count;
	memcpy(&STACKS[to][STACK_SZ[to]], &STACKS[from][STACK_SZ[from]], count);
	STACK_SZ[to] += count;
}

#define constexpr_strlen(s) (sizeof(s)-1)

int main()
{
	clock_t t1 = clock();

	size_t line_length = 4 * NSTACKS;
	size_t depth = 0;

	while (input[1] != '1') {
		input += line_length;
		++depth;
	}

	char *instr = input + line_length + 1;

	do {
		input -= line_length;

		for (size_t i = 0; i < NSTACKS; i++) {
			char v = input[4*i + 1];
			if (v != ' ') push(i,v), push(i + NSTACKS, v);
		}
	}
	while (--depth);

	while (*instr) {
		instr += constexpr_strlen("move ");
		size_t count = strtoul(instr, &instr, 10);
		instr += constexpr_strlen(" from ");
		size_t i = strtoul(instr, &instr, 10) - 1;
		instr += constexpr_strlen(" to ");
		size_t j = strtoul(instr, &instr, 10) - 1;
		++instr; // skip newline

		for (size_t c = 0; c < count; c++)
			push(j, pop(i));

		move_chunk(count, i + NSTACKS, j + NSTACKS);
	}

	static char part1[NSTACKS + 1];
	static char part2[NSTACKS + 1];

	for (int i = 0; i < NSTACKS; i++) {
		part1[i] = pop(i);
		part2[i] = pop(i + NSTACKS);
	}
	
	clock_t t2 = clock();

	puts(part1);
	puts(part2);

	printf("Time: %ld\n", t2 - t1);
}
