#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "input/day16input.h"

#define constexpr_strlen(s) (sizeof(s)-1)

typedef struct { uint16_t ID, flow, count, tunnels[10]; } room_t;

static inline uint16_t parseID(const char *id) { return id[0] + (id[1] << 8); }
static inline uint8_t min(uint8_t a, uint8_t b) { return (a < b) ? a : b; }
static inline uint64_t max(uint64_t a, uint64_t b) { return (a > b) ? a : b; }

// TODO: implement hashmap to memoize function

static
uint64_t solve(size_t i, size_t AA, size_t time, room_t rooms[LINES], uint8_t dist[LINES][LINES], uint64_t visit)
{
	uint64_t res = 0;
	visit &= ~(1ull << i);

	if (time <= 2) return (AA != SIZE_MAX) ? solve(AA, SIZE_MAX, 26, rooms, dist, visit) : 0; 

	for (uint64_t mask = visit; mask; mask &= mask - 1) {
		size_t j = __builtin_ctzll(mask);

		if (dist[i][j] < time) {
			size_t remainder = time - dist[i][j] - 1;
			res = max(res, remainder * rooms[j].flow + solve(j, AA, remainder, rooms, dist, visit));
		}
	}

	return res;
}

int main()
{
	const clock_t t1 = clock();

	static room_t rooms[LINES] = {0};

	// parse input
	for (size_t i = 0; i < LINES; ++i) {
		input += constexpr_strlen("Valve ");	
		rooms[i].ID = parseID(input);
		input += constexpr_strlen("__ has flow rate=");
		rooms[i].flow = strtoul(input, &input, 10);
		input += constexpr_strlen("; tunnels lead to valve");
		input += (*input == 's') - 1;

		while (*input != '\n') {
			input += constexpr_strlen(", ");	
			rooms[i].tunnels[rooms[i].count++] = parseID(input);
			input += 2;
		}

		input += 1;
	}

	// normalise IDs to indices
	for (size_t i = 0; i < LINES; ++i) {
		for (uint16_t j = 0; j < rooms[i].count; ++j) {
			uint16_t ID = rooms[i].tunnels[j];

			for (uint16_t k = 0; k < LINES; ++k) {
				if (rooms[k].ID == ID) {
					rooms[i].tunnels[j] = k; break;
				}
			}
		}
	}

	// floyd-warshall algorithm to construct adjacency matrix
	// initialise matrix to infinity (2 * LINES)
	static uint8_t dist[LINES][LINES] = { [0 ... LINES-1] = { [0 ... LINES-1] = 2 * LINES }};

	for (size_t i = 0; i < LINES; ++i) {
		for (size_t j = 0; j < rooms[i].count; ++j)
			dist[i][rooms[i].tunnels[j]] = 1;
	}

	for (size_t k = 0; k < LINES; ++k)
		for (size_t i = 0; i < LINES; ++i)
			for (size_t j = 0; j < LINES; ++j)
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

	// construct bitset of visited nodes
	uint64_t visit_mask = 0;
	for (size_t i = 0; i < LINES; ++i) if (rooms[i].flow) visit_mask |= (1ull << i);

	// find room AA
	size_t start;
	for (start = 0; start < LINES; ++start)
		if (rooms[start].ID == 'AA') break;

	// use dfs to find solution
	uint64_t part1 = solve(start, SIZE_MAX, 30, rooms, dist, visit_mask);
	uint64_t part2 = solve(start, start, 26, rooms, dist, visit_mask);

	const clock_t t2 = clock();

	printf("%lu\n", part1);
	printf("%lu\n", part2);
	printf("Time: %ld\n", t2 - t1);
}
