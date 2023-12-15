#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool char_is_digit(char c) {
	return '0' <= c && c <= '9';
}

enum cellcode { CODE_EMPTY = 0, CODE_NUMBER = INT16_MAX, CODE_PART = INT16_MIN };

int main(void) {
	long int total = 0;

	short buf[200] = { CODE_EMPTY };
	int c = getchar();

	while (c != EOF) {
		bool is_part = false;
		int number = 0;
		int i = 0;
		while (c != '\n' && c != '\r') {
			if (c == '.') {
				is_part = (buf[i] == -1);
				number = (buf[i] > 0 ? buf[i] : 0);
				buf[i] = 0;
				c = getchar();
				++i;
			} else if ('1' <= c && c <= '9') {
				number = (c - '0');
				is_part |= (buf[i] == -1);
				c = getchar();
				++i;
				while('0' <= c && c <= '9') {
					number = number * 10 + (c - '0');
					is_part = (is_part || buf[i] == -1);
					buf[i-1] = -2;
					c = getchar();
					++i;
				}

				if (buf[i] == -1 || is_part) {
					total += number;
					is_part = false;
					number = 0;
					if (buf[i-1] > 0)
						number = buf[i-1];
					buf[i-1] = 0;
				} else if (buf[i-1] > 0) {
					int temp = buf[i-1];
					buf[i-1] = number;
					number = temp;
				} else {
					buf[i-1] = number;
					number = 0;
				}
			} else {
				total += number;
				for (int j=(i < 1 ? 0 : i-1); j <= i+1; ++j) {
					while (buf[j] == -2) j++;
					if (buf[j] > 0) {
						total += buf[j];
					}
				}

				buf[i] = -1;

				is_part = true;
				number = 0;
				c = getchar();
				++i;
			}
		}
		c = getchar();
	}

	printf("Sum of part numbers:\n\t%ld\n", total);

	return 0;
}
