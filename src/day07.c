#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <x86intrin.h>

#include "day07input.h"

typedef struct DIR { uint64_t ID, size, count; struct DIR *parent, *subdirs[10]; } DIR;

static inline
DIR *new_dir(uint64_t ID, DIR *parent)
{
	DIR *dir = calloc(1, sizeof(DIR));
	dir->ID = ID;
	dir->parent = parent;
	return dir;
}

static inline
uint64_t parseID(const char **input)
{
	uint64_t ID = **(uint64_t **)input;
	uint64_t mask = ID ^ 0x0A0A0A0A0A0A0A0A;

	uint64_t C = ID;

	mask = (mask - 0x0101010101010101) &~ mask & 0x8080808080808080;
	size_t length = _tzcnt_u64(mask) >> 3;

	*input += length + 1;
	ID &= (length != 8) ? (1ULL << ((8 * length))) - 1 : -1;

	return ID;
}

static inline
DIR *find_subdir(DIR *dir, uint64_t ID)
{
	for (uint64_t i = 0; i < dir->count; i++)
		if (dir->subdirs[i]->ID == ID)
			return dir->subdirs[i];

	return NULL;
}

static inline
void populate_dir(const char **input, DIR *dir)
{
	while (**input && **input != '$') {
		if (**input == 'd') {
			*input += 4;
			dir->subdirs[dir->count++] = new_dir(parseID(input), dir);
		}

		else {
			dir->size += strtoull(*input, (char **)input, 10);
			*input += strcspn(*input, "\n") + 1;
		}
	}
}

static uint64_t part1 = 0;
static uint64_t part2 = UINT64_MAX;

static
uint64_t accumulate(DIR *dir)
{
	for (uint64_t i = 0; i < dir->count; i++) {
		dir->size += accumulate(dir->subdirs[i]);
	}

	if (dir->size <= 100000) part1 += dir->size;
	return dir->size;
}

static
void find_smallest(DIR *dir, const uint64_t size)
{
	if (size <= dir->size && dir->size < part2) part2 = dir->size;

	for (uint64_t i = 0; i < dir->count; i++)
		find_smallest(dir->subdirs[i], size);
}

int main()
{
	clock_t t1 = clock();

	DIR *root = new_dir(0, NULL);
	input += sizeof "$ cd /";

	DIR *current = root;

	while (*input) {
		char instr = input[2];

		if (instr == 'l') {
			input += 5;
			populate_dir(&input, current);
		}

		else {
			input += 5;
			uint64_t ID = parseID(&input);

			current = (ID == '..') ? current->parent
								   : find_subdir(current, ID);
		}
	}

	accumulate(root);
	find_smallest(root, 30000000 - (70000000 - root->size));

	clock_t t2 = clock();

	printf("%lu\n", part1);
	printf("%lu\n", part2);
	printf("Time: %ld\n", t2 - t1);
}
