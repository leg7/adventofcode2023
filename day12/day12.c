#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define EXIT_ALLOC_ERROR 2
#define EXIT_BAD_INPUT 3

typedef int32_t i32;
typedef uint8_t u8;
typedef size_t usize;

enum condition
{
	CONDITION_OPERATIONAL = '.',
	CONDITION_DAMAGED = '#',
	CONDITION_TBD = '?'
};

void possible_arrangements(usize* arrangements, usize cursor, i32 min_space_left_required, u8* damaged, usize damaged_len, char* row, usize row_len)
{
	while (cursor < row_len && row[cursor] == '.') {
		++cursor;
	}
	usize space_used_this_iteration = damaged_len == 1 && row_len - cursor == *damaged ?
		*damaged :
		*damaged + 1;

	min_space_left_required -= space_used_this_iteration;
	i32 space_used = cursor + space_used_this_iteration,
	space_left = row_len - space_used;

	while (space_left >= min_space_left_required) {
		bool compatible = true;

		// Checking if contigous broken bloc fits (###);
		for (usize i = 0; compatible && i < *damaged; ++i) {
			if (row[cursor + i] == '.') {
				compatible = false;
			}
		}

		// Checking if trailing . to separate blocs fits
		if (compatible && cursor + *damaged < row_len && row[cursor + *damaged] == '#') {
			compatible = false;
		}

		// If we've processed all possible damaged blocs check if the ending of row doesn't contain any additonal ones
		if (compatible && damaged_len == 1 && space_left != 0) {
			for (usize i = cursor + *damaged + 1; compatible && i < row_len; ++i) {
				if (row[i] == '#') {
					compatible = false;
				}
			}
		}

		if (compatible && damaged_len == 1) {
			++(*arrangements);
		} else if (compatible && damaged_len > 1 && space_left > 0) { // If it all fits check if the next damage bloc fits
			possible_arrangements(arrangements, cursor + space_used_this_iteration, min_space_left_required, damaged + 1, damaged_len - 1, row, row_len);
		}

		if (row[cursor] != '#') {
			++space_used;
			++cursor;
			--space_left;
		} else {
			break;
		}
	}
}

i32 main(void)
{
	char* row = NULL;
	usize row_size = 10;
	usize row_len = 0;
	if ((row = reallocarray(row, row_size, 1)) == NULL) {
		exit(EXIT_ALLOC_ERROR);
	}

	u8* damaged = NULL;
	usize damaged_size = 10;
	usize damaged_len = 0;
	if ((damaged = reallocarray(damaged, damaged_size, sizeof *damaged)) == NULL) {
		exit(EXIT_ALLOC_ERROR);
	}


	usize sum = 0;
	char input = getchar();
	usize line = 0;
	do {
		// input parsing
		while (input != ' ') {
			row[row_len++] = input;
			if (row_len == row_size) {
				row_size *= 2;
				char *tmp = reallocarray(row, row_size, sizeof *tmp);
				if (tmp == NULL) {
					free(row);
					exit(EXIT_ALLOC_ERROR);
				} else {
					row = tmp;
				}
			}
			input = getchar();
		}

		while (input != '\n') {
			u8 number = 0;
			input = getchar();
			while (input >= '0' && input <= '9') {
				number = 10 * number + (input - '0');
				input = getchar();
			}
			damaged[damaged_len++] = number;
			if (damaged_len == damaged_size) {
				damaged_size *= 2;
				u8 *tmp = reallocarray(damaged, damaged_size, sizeof *tmp);
				if (tmp == NULL) {
					free(tmp);
					exit(EXIT_ALLOC_ERROR);
				} else {
					damaged = tmp;
				}
			}
		}

		// possible arrangements calculation
		i32 min_space_required = 0;
		for (usize i = 0; i < damaged_len - 1; ++i) {
			min_space_required += damaged[i] + 1;
		}
		min_space_required += damaged[damaged_len - 1];

		usize arrangements = 0;
		possible_arrangements(&arrangements, 0, min_space_required, damaged, damaged_len, row, row_len);
		sum += arrangements;
		printf("line %zu: %zu\n", line, arrangements);

		row_len = 0;
		damaged_len = 0;
		++line;
	} while ((input = getchar()) != EOF);

	free(row);
	free(damaged);

	printf("\n%zu\n", sum);

	return EXIT_SUCCESS;
}
