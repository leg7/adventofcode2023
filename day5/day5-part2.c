#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef uint32_t u32;
typedef uint8_t u8;
typedef int32_t i32;
typedef size_t size;

typedef struct
{
	size sourceRangeStart, destinationRangeStart, range;
} Map;

typedef struct MapBst MapBst;
struct MapBst
{
	Map *map;
	MapBst *left, *right;
};

typedef struct
{
	size start, range;
} Seed;

void MapBstAdd(MapBst **t, Map *m)
{
	if (*t == NULL) {
		*t = malloc(sizeof **t);
		(*t)->map = m;
		(*t)->left = NULL;
		(*t)->right = NULL;
		return;
	}

	MapBst *parent, *backup = *t;
	bool wasLeft;
	while (*t != NULL) {
		parent = *t;
		if (m->sourceRangeStart > (*t)->map->sourceRangeStart) {
			wasLeft = false;
			*t = (*t)->right;
		} else if (m->sourceRangeStart < (*t)->map->sourceRangeStart) {
			wasLeft = true;
			*t = (*t)->left;
		} else {
			exit(69);
		}
	}
	*t = malloc(sizeof **t);
	(*t)->map = m;
	(*t)->left = NULL;
	(*t)->right = NULL;

	if (wasLeft) {
		parent->left = *t;
	} else {
		parent->right = *t;
	}

	*t = backup;
}

void MapBstFree(MapBst **t)
{
	if (t == NULL || *t == NULL) {
		return;
	}
	MapBstFree(&(*t)->left);
	MapBstFree(&(*t)->right);
	free((*t)->map);
	(*t)->map = NULL;
	free(*t);
	*t = NULL;
}

size MapBstSourceToDest(MapBst *t, const size source)
{
	if (t == NULL) {
		exit(420);
	}

	do {
		if (source < t->map->sourceRangeStart) {
			t = t->left;
		} else if (source < t->map->sourceRangeStart + t->map->range) {
			return t->map->destinationRangeStart + (source - t->map->sourceRangeStart);
		} else {
			t = t->right;
		}
	} while (t != NULL);

	return source;
}

int main(void)
{

	size_t lineSize = 250;
	char line[lineSize];
	fgets(line, lineSize, stdin); // skip text
	fgets(line, lineSize, stdin);
	char *save = NULL;
	char *token = strtok_r(line, " ", &save);

	// Use scanf next time
	size seedsLen = 1, seedsSize = 30;
	Seed seeds[seedsSize];
	seeds[0] = (Seed){ atoll(token), atoll(strtok_r(NULL, " ", &save)) };
	while ((token = strtok_r(NULL, " ", &save))) {
		seeds[seedsLen++] = (Seed){ atoll(token), atoll(strtok_r(NULL, " ", &save))};
	}
	fgets(line, lineSize, stdin); // skip empty newline

	size mapsSize = 7, mapsLen = 0;
	MapBst *maps[mapsSize];
	MapBst *map = NULL;
	while (fgets(line, lineSize, stdin) != NULL) {
		if (line[0] > 'a' && line[0] < 'z') {
			map = NULL;
			continue;
		} else if (line[0] == '\n') {
			maps[mapsLen++] = map;
		} else {
			char *save2 = NULL;
			Map *m = malloc(sizeof *m);
			m->destinationRangeStart = atoll(strtok_r(line, " ", &save2));
			m->sourceRangeStart = atoll(strtok_r(NULL, " ", &save2));
			m->range = atoll(strtok_r(NULL, " ", &save2));
			MapBstAdd(&map, m);
		}
	}

	size min = SIZE_MAX;
	for (u8 i = 0; i < seedsLen; ++i) {
		printf("new seed\n");
		for (size k = seeds[i].start; k < seeds[i].start + seeds[i].range; ++k) {
			size candidate = k;
			for (size j = 0; j < mapsLen; ++j) {
				candidate = MapBstSourceToDest(maps[j], candidate);
			}
			if (candidate < min) {
				min = candidate;
			}
		}
	}
	printf("\n%zu\n", min);

	for (size i = 0; i < mapsLen; ++i) {
		MapBstFree(&maps[i]);
	}

	return 0;
}
