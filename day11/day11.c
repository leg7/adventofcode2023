#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef size_t usize;
typedef int32_t i32;

typedef struct
{
	i32 x, y;
} Coord;

i32 CoordCompareX(const void *a, const void *b)
{
	return ((Coord*)(a))->x - ((Coord*)(b))->x;
}

#define GALAXIES_MAX_COUNT 450
#define EXPANSION_FACTOR 1000000

int main(void)
{
	usize galaxiesLen = 0;
	Coord galaxies[GALAXIES_MAX_COUNT];
	{
		char input;
		usize col = 0, line = 0;
		bool galaxyFound = false;
		while ((input = getchar()) != EOF)  {
			if (input == '#') {
				galaxyFound = true;
				galaxies[galaxiesLen++] = (Coord){ col, line };
				++col;
			} else if (input == '\n') {
				if (!galaxyFound) { // vertical expansion
					line += EXPANSION_FACTOR;
				} else {
					++line;
				}
				col = 0;
				galaxyFound = false;
			} else {
				++col;
			}
		}

		// horizontal expansion
		qsort(galaxies, galaxiesLen, sizeof *galaxies, CoordCompareX);

		i32 galaxiesSteps[GALAXIES_MAX_COUNT];
		galaxiesSteps[0] = galaxies[0].x;

		for (usize i = 1; i < galaxiesLen; ++i) {
			galaxiesSteps[i] = galaxies[i].x - galaxies[i - 1].x;
		}

		if (galaxies[0].x > 0) {
			galaxies[0].x *= EXPANSION_FACTOR;
		}
		for (usize i = 1; i < galaxiesLen; ++i) {
			if (galaxiesSteps[i] > 1){
				galaxies[i].x = galaxies[i - 1].x + ((galaxiesSteps[i] - 1) * EXPANSION_FACTOR) + 1;
			} else {
				galaxies[i].x = galaxies[i - 1].x + galaxiesSteps[i];
			}
		}
	}
	// Distances
	usize distance = 0;
	for (usize i = 0; i < galaxiesLen - 1; ++i) {
		for (usize j = i + 1; j < galaxiesLen; ++j) {
			distance += abs(galaxies[j].x - galaxies[i].x) + abs(galaxies[i].y - galaxies[j].y);
		}
	}

	printf("%zu", distance);

	return 0;
}
