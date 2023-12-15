#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef size_t size;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint8_t u8;

static inline
bool charIsDigit(const char c)
{
	return c >= '0' && c <= '9';
}

static inline
bool charIsSymbol(const char c)
{
	return (c >= 33 && c <= 45) || c == 47 || (c >= 58 && c <= 64) ||
		  (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

static inline
size extractFullNumberAndZero(char **text, const size line, const size column)
{
	size left = column;
	while (left > 0 && charIsDigit(text[line][left - 1])) {
		--left;
	}

	size right = column;
	while (text[line][right] != '\0' && charIsDigit(text[line][right + 1])) {
		++right;
	}

	char* numberStr = strndup(text[line] + left, right - left + 1);
	size number = atoi(numberStr);
	free(numberStr);

	for (size i = left; i <= right; ++i)
	{
		text[line][i] = '.';
	}

	return number;
}

int main(void)
{
	size textLength = 0, textSize = 100;
	char** text = malloc(textSize * sizeof(char*));

	char* line = NULL;
	size lineSize;
	while (getline(&line, &lineSize, stdin) >= 0)
	{
		if (textLength == textSize)
		{
			textSize *= 2;
			text = realloc(text, textSize * sizeof(*text));
		}

		text[textLength++] = strdup(line);
	}
	free(line);

	size sum = 0;
	size gearRatioSum = 0;
	// should segfault because on line 0 I try to get line -1 and on the last line I try to get last + 1
	for (size i = 0; i < textLength; ++i)
	{
		for (size j = 0; text[i][j] != '\0'; ++j)
		{
			u8 partNumberCount = 0;
			bool isGear = text[i][j] == '*';
			size gearRatio = isGear ? 1 : 0;

			if (charIsSymbol(text[i][j]))
			{
				size column;
				size line = i - 1;
				for (column = j - 1; column <= j + 1; ++column)
				{
					if (charIsDigit(text[line][column]))
					{
						size number = extractFullNumberAndZero(text, line, column);
						sum += number;
						gearRatio *= number;
						++partNumberCount;
					}
				}

				++line;

				column = j + 1;
				if (charIsDigit(text[line][column]))
				{
					size number = extractFullNumberAndZero(text, line, column);
					sum += number;
					gearRatio *= number;
					++partNumberCount;
				}

				column = j - 1;
				if (charIsDigit(text[line][column]))
				{
					size number = extractFullNumberAndZero(text, line, column);
					sum += number;
					gearRatio *= number;
					++partNumberCount;
				}

				++line;
				for (; column <= j + 1; ++column) // column is already at j - 1
				{
					if (charIsDigit(text[line][column]))
					{
						size number = extractFullNumberAndZero(text, line, column);
						sum += number;
						gearRatio *= number;
						++partNumberCount;
					}
				}

				if (isGear && partNumberCount >= 2)
				{
					gearRatioSum += gearRatio;
				}
			}
		}
	}

	printf("Sum: %zu\nGear Ratio Sum: %zu\n", sum, gearRatioSum);

	for (size i = 0; i < textLength; ++i)
	{
		free(text[i]);
	}
	free(text);
	printf("\n");

	return 0;
}
