#include <stdint.h>
#include <stdio.h>

typedef int32_t i32;
typedef uint32_t u32;
typedef size_t size;

#define LINE_SIZE 141
#define LINE_COUNT 140
// #define LINE_SIZE 6
// #define LINE_COUNT 5

typedef struct
{
	u32 line, col;
} Coordinates;

enum Direction
{
	LEFT,
	RIGHT,
	ABOVE,
	BELOW,
};


i32 main(void)
{

	Coordinates start;
	char maze[LINE_COUNT][LINE_SIZE];
	for (size line = 0; line < LINE_COUNT; ++line) {
		for (size col = 0; col < LINE_SIZE; ++col) {
			maze[line][col] = getchar();
			if (maze[line][col] == 'S') {
				start = (Coordinates){ line, col };
			}
			// printf("%c", maze[line][col]);
		}
	}

	Coordinates pos = start;
	u32 steps = 0;
	enum Direction cameFrom = -1;
	do {
		switch (maze[pos.line][pos.col]) {
			case '|':
			if (cameFrom == BELOW) {
					--pos.line;
			} else {
					++pos.line;
					cameFrom = ABOVE;
			}
			break;

			case '-':
			if (cameFrom == LEFT) {
					++pos.col;
			} else {
					--pos.col;
					cameFrom = RIGHT;
			}
			break;

			case 'L':
			if (cameFrom == ABOVE) {
					++pos.col;
					cameFrom = LEFT;
			} else {
					--pos.line;
					cameFrom = BELOW;
			}
			break;

			case 'J':
			if (cameFrom == ABOVE) {
					--pos.col;
					cameFrom = RIGHT;
			} else {
					--pos.line;
					cameFrom = BELOW;
			}
			break;

			case '7':
			if (cameFrom == BELOW) {
					--pos.col;
					cameFrom = RIGHT;
			} else {
					++pos.line;
					cameFrom = ABOVE;
			}
			break;

			case 'F':
			if (cameFrom == BELOW) {
					++pos.col;
					cameFrom = LEFT;
			} else {
					++pos.line;
					cameFrom = ABOVE;
			}
			break;

			case 'S': {
				const char left = maze[pos.line][pos.col - 1];
				const char right = maze[pos.line][pos.col + 1];
				const char above = maze[pos.line - 1][pos.col];
				const char below = maze[pos.line + 1][pos.col];
				if (right == '-' || right == 'J' || right == '7') {
					++pos.col;
					cameFrom = LEFT;
				} else if (left == '-' || left == 'F' || left == 'L') {
					--pos.col;
					cameFrom = RIGHT;
				} else if (above == '|' || above == '7' || above == 'F') {
					--pos.line;
					cameFrom = BELOW;
				} else if (below == '|' || below == 'J' || below == 'L') {
					++pos.line;
					cameFrom = ABOVE;
				}
			}

		}
		++steps;
	} while (maze[pos.line][pos.col] != 'S');

	printf("Steps: %d", steps / 2);

	return 0;
}
