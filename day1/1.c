#include <stdio.h>
#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef size_t size;

static inline
bool charIsDigit(const char c) {
	return (c >= '0' && c <= '9');
}


int main(void) {
	FILE *f = fopen("input", "r");
	if (f == NULL) {
		perror("File could not be opened");
		return EXIT_FAILURE;
	}

	regex_t r;
	if (regcomp(&r, "[0..9]|one|two|three|four|five|six|seven|eight|nine", REG_EXTENDED) != 0) {
		perror("Bad regex");
		return EXIT_FAILURE;
	}

	regfree(&r);

	u16 sum = 0;
	char line[53];
	while (fgets(line, sizeof line, f) != NULL) {
		u8 i = 0;
		// All lines have numbers so this will terminate
		while (!charIsDigit(line[i])) {
			++i;
		}
		u8 const firstDigitIndex = i;

		u8 lastDigitIndex = firstDigitIndex;
		while (line[++i] != '\0') {
			if (charIsDigit(line[i])) {
				lastDigitIndex = i;
			}
		}

		sum += 10 * (line[firstDigitIndex] - '0') + (line[lastDigitIndex] - '0');
	}

	printf("%d\n", sum);

	fclose(f);
}
