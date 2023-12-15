#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint16_t u16;
typedef uint8_t u8;
typedef size_t size;

enum color {
	colorRed,
	colorGreen,
	colorBlue
};
#define ENUM_COLOR_MEMBER_COUNT 3

enum color colorOfStr(const char *str) {
	if (strcmp(str, "red") == 0) {
		return colorRed;
	} else if (strcmp(str, "green") == 0) {
		return colorGreen;
	} else if (strcmp(str, "blue") == 0) {
		return colorBlue;
	} else {
		printf("Error: can't convert string '%s' to `enum color`\n", str);
		exit(69);
	}
}

int main(void) {
	char* line = NULL;
	size lineSize;

	u8 bag[ENUM_COLOR_MEMBER_COUNT] = { 12, 13, 14 };

	size gameId = 1, result = 0, powerSum = 0;
	while (getline(&line, &lineSize, stdin) >= 0) {
		char* cleanSave;
		char* clean = strtok_r(line, ":", &cleanSave);
		clean = strtok_r(NULL, ":", &cleanSave);

		u8 game[ENUM_COLOR_MEMBER_COUNT] = { 0 };
		for (char *cubeSetSave, *cubeSet = strtok_r(clean, ";\n", &cubeSetSave);
			cubeSet != NULL;
			cubeSet = strtok_r(NULL, ";\n", &cubeSetSave))
		{
			for (char *cubeSave, *cube = strtok_r(cubeSet, ",", &cubeSave);
				cube != NULL;
				cube = strtok_r(NULL, ",", &cubeSave))
			{
				char *colorDataSave;
				const char *colorCount = strtok_r(cube, " ", &colorDataSave);
				const char *color = strtok_r(NULL, " ", &colorDataSave);

				const enum color c = colorOfStr(color);
				const u8 cn = atoi(colorCount);
				if (game[c] < cn) {
					game[c] = cn;
				}
			}
		}

		// printf("%d, %d, %d\n", game[0], game[1], game[2]);

		bool gameIsPossible = true;
		for (u8 i = 0; i < ENUM_COLOR_MEMBER_COUNT && gameIsPossible; ++i) {
			if (game[i] > bag[i]) {
				gameIsPossible = false;
			}
		}
		if (gameIsPossible) {
			result += gameId;
		}

		u16 power = 1;
		for (u8 i = 0; i < ENUM_COLOR_MEMBER_COUNT; ++i) {
			power *= game[i];
		}
		powerSum += power;

		++gameId;
	}
	free(line);

	printf("1: %zu\n2: %zu", result, powerSum);

	return 0;
}
