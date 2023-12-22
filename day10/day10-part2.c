#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef int32_t i32;
typedef uint32_t u32;
typedef size_t size;

// Size without newlines or anything

// input
#define LINE_SIZE 140
#define LINE_COUNT 140

// example2
// #define LINE_SIZE 11
// #define LINE_COUNT 9

// example3
// #define LINE_SIZE 10
// #define LINE_COUNT 9

// example4
// #define LINE_SIZE 20
// #define LINE_COUNT 10


typedef struct
{
	u32 line, col;
} Coordinates;

enum Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

enum MapState
{
	LOOP_TBD,
	LOOP_PART,
	LOOP_LEFT,
	LOOP_RIGHT,
	LOOP_COUNTED,
};

char MapStateToChar(const enum MapState s)
{
	switch (s) {
		case LOOP_TBD: return '?';
		case LOOP_PART: return '#';
		case LOOP_LEFT: return 'L';
		case LOOP_RIGHT: return 'R';
		case LOOP_COUNTED: return '.';
		default: exit(420);
	}
}

static inline void
mapStateSetLeft(enum MapState mapState[LINE_COUNT][LINE_SIZE], Coordinates coord, enum MapState state)
{
	if (coord.col != 0 && mapState[coord.line][coord.col - 1] != LOOP_PART) {
		mapState[coord.line][coord.col - 1] = state;
	}
}

static inline void
mapStateSetRight(enum MapState mapState[LINE_COUNT][LINE_SIZE], Coordinates coord, enum MapState state)
{
	if (coord.col + 1 < LINE_SIZE && mapState[coord.line][coord.col + 1] != LOOP_PART) {
		mapState[coord.line][coord.col + 1] = state;
	}
}

static inline void
mapStateSetAbove(enum MapState mapState[LINE_COUNT][LINE_SIZE], Coordinates coord, enum MapState state)
{
	if (coord.line != 0 && mapState[coord.line - 1][coord.col] != LOOP_PART) {
		mapState[coord.line - 1][coord.col] = state;
	}
}

static inline void
mapStateSetBelow(enum MapState mapState[LINE_COUNT][LINE_SIZE], Coordinates coord, enum MapState state)
{
	if (coord.line + 1 < LINE_COUNT && mapState[coord.line + 1][coord.col] != LOOP_PART) {
		mapState[coord.line + 1][coord.col] = state;
	}
}


i32 main(void)
{
	Coordinates loopStart;
	char map[LINE_COUNT][LINE_SIZE];
	for (size line = 0; line < LINE_COUNT; ++line) {
		for (size col = 0; col < LINE_SIZE; ++col) {
			map[line][col] = getchar();
			if (map[line][col] == 'S') {
				loopStart = (Coordinates){ line, col };
			}
			// printf("%c", maze[line][col]);
		}
		getchar();
	}
	enum MapState mapState[LINE_COUNT][LINE_SIZE] = { LOOP_TBD };
	Coordinates pos = loopStart;
	mapState[pos.line][pos.col] = LOOP_PART;

	char const left = (pos.col != 0) ? map[pos.line][pos.col - 1] : '.';
	char const right = (pos.col + 1 < LINE_SIZE) ? map[pos.line][pos.col + 1] : '.';
	char const above = (pos.line != 0) ? map[pos.line - 1][pos.col] : '.';
	char const below = (pos.line + 1 < LINE_COUNT) ? map[pos.line + 1][pos.col] : '.';
	enum Direction going;
	if (right == '-' || right == 'J' || right == '7') {
		going = RIGHT;
		mapState[pos.line - 1][pos.col] = LOOP_LEFT;
		mapState[pos.line + 1][pos.col] = LOOP_RIGHT;
		mapState[pos.line][pos.col - 1] = LOOP_LEFT;
		++pos.col;
	} else if (left == '-' || left == 'F' || left == 'L') {
		going = LEFT;
		mapState[pos.line - 1][pos.col] = LOOP_RIGHT;
		mapState[pos.line + 1][pos.col] = LOOP_LEFT;
		mapState[pos.line][pos.col + 1] = LOOP_RIGHT;
		--pos.col;
	} else if (above == '|' || above == '7' || above == 'F') {
		going = UP;
		mapState[pos.line][pos.col - 1] = LOOP_LEFT;
		mapState[pos.line][pos.col + 1] = LOOP_RIGHT;
		--pos.line;
	} else if (below == '|' || below == 'J' || below == 'L') {
		going = DOWN;
		mapState[pos.line][pos.col + 1] = LOOP_LEFT;
		mapState[pos.line][pos.col - 1] = LOOP_RIGHT;
		++pos.line;
	}

	do {
		mapState[pos.line][pos.col] = LOOP_PART;

		switch (map[pos.line][pos.col]) {
			case '|':
			if (going == UP) {
					mapStateSetLeft(mapState, pos, LOOP_LEFT);
					mapStateSetRight(mapState, pos, LOOP_RIGHT);
					--pos.line;
			} else {
					mapStateSetLeft(mapState, pos, LOOP_RIGHT);
					mapStateSetRight(mapState, pos, LOOP_LEFT);
					++pos.line;
					going = DOWN;
			}
			break;

			case '-':
			if (going == RIGHT) {
					mapStateSetAbove(mapState, pos, LOOP_LEFT);
					mapStateSetBelow(mapState, pos, LOOP_RIGHT);
					++pos.col;
			} else {
					mapStateSetAbove(mapState, pos, LOOP_RIGHT);
					mapStateSetBelow(mapState, pos, LOOP_LEFT);
					--pos.col;
					going = LEFT;
			}
			break;

			case 'L':
			if (going == DOWN) {
					mapStateSetLeft(mapState, pos, LOOP_RIGHT);
					mapStateSetBelow(mapState, pos, LOOP_RIGHT);
					++pos.col;
					going = RIGHT;
			} else {
					mapStateSetLeft(mapState, pos, LOOP_LEFT);
					mapStateSetBelow(mapState, pos, LOOP_LEFT);
					--pos.line;
					going = UP;
			}
			break;

			case 'J':
			if (going == DOWN) {
					mapStateSetRight(mapState, pos, LOOP_LEFT);
					mapStateSetBelow(mapState, pos, LOOP_LEFT);
					--pos.col;
					going = LEFT;
			} else {
					mapStateSetRight(mapState, pos, LOOP_RIGHT);
					mapStateSetBelow(mapState, pos, LOOP_RIGHT);
					--pos.line;
					going = UP;
			}
			break;

			case '7':
			if (going == UP) {
					mapStateSetRight(mapState, pos, LOOP_RIGHT);
					mapStateSetAbove(mapState, pos, LOOP_RIGHT);
					--pos.col;
					going = LEFT;
			} else {
					mapStateSetRight(mapState, pos, LOOP_LEFT);
					mapStateSetAbove(mapState, pos, LOOP_LEFT);
					++pos.line;
					going = DOWN;
			}
			break;

			case 'F':
			if (going == UP) {
					mapStateSetLeft(mapState, pos, LOOP_LEFT);
					mapStateSetAbove(mapState, pos, LOOP_LEFT);
					++pos.col;
					going = RIGHT;
			} else {
					mapStateSetLeft(mapState, pos, LOOP_RIGHT);
					mapStateSetAbove(mapState, pos, LOOP_RIGHT);
					++pos.line;
					going = DOWN;
			}
			break;

		}
	} while (mapState[pos.line][pos.col] != LOOP_PART);

	// size area = 0;
	size totalArea = LINE_SIZE * LINE_COUNT;
	size rightOfLoopArea = 0, loopArea = 0;
	size leftOfLoopArea = 0;
	size count = 0;
	enum MapState lastSeen = -1;
	for (size line = 0; line < LINE_COUNT; ++line) {
		for (size col = 0; col < LINE_SIZE; ++col) {
			switch (mapState[line][col]) {
				case LOOP_COUNTED:
				exit(69);

				case LOOP_TBD:
					++count;
					// mapState[line][col] = LOOP_COUNTED;
				break;

				case LOOP_PART:
					++loopArea;
				break;

				case LOOP_RIGHT:
					rightOfLoopArea += count + 1;
					count = 0;
					// mapState[line][col] = LOOP_COUNTED;
					lastSeen = LOOP_RIGHT;
				break;

				case LOOP_LEFT:
					leftOfLoopArea += count + 1;
					count = 0;
					// mapState[line][col] = LOOP_COUNTED;
					lastSeen = LOOP_LEFT;
				break;

			}
		}
		if (lastSeen == LOOP_RIGHT) {
			rightOfLoopArea += count;
			count = 0;
		} else if (lastSeen == LOOP_LEFT){
			leftOfLoopArea += count;
			count = 0;
		}
	}

	for (size line = 0; line < LINE_COUNT; ++line) {
		for (size col = 0; col < LINE_SIZE; ++col) {
			printf("%c", MapStateToChar(mapState[line][col]));
		}
		printf("\n");
	}
	printf("\ntotalArea: %zu\nloopArea: %zu\nrightOfLoopArea %zu\nleftOfLoopArea: %zu\nTotal: %zu\n", totalArea, loopArea, rightOfLoopArea, leftOfLoopArea, loopArea + rightOfLoopArea + leftOfLoopArea);

	return 0;
}
