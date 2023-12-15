#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t u32;

int main(void) {
	u32 cardTotal = 0;

	#define MAX_LINES 197
	u32 cardCopies[MAX_LINES] = { 1 };
	for (u32 i = 0; i < MAX_LINES; ++i) {
		cardCopies[i] = 1;
	}

	size_t lineSize = 150;
	char *line = malloc(lineSize);
	size_t lineNum = 0;
	while (getline(&line, &lineSize, stdin) >= 0) {
		char *saveClean = NULL;
		char *clean = strtok_r(line, ":", &saveClean);
		clean = strtok_r(NULL, ":", &saveClean);

		char *saveWinners = NULL;
		char *winners = strtok_r(clean, "|", &saveWinners);
		char *candidates = strtok_r(NULL, "|", &saveWinners);

		bool isWinner[100] = { false };

		char *nextWinner = NULL;
		char *winner = strtok_r(winners, " ", &nextWinner);
		while (winner != NULL) {
			isWinner[atoi(winner)] = true;
			winner = strtok_r(NULL, " ", &nextWinner);
		}

		char *nextCandidate = NULL;
		char *candidate = strtok_r(candidates, " ", &nextCandidate);
		u32 wins = 0;
		while (candidate != NULL) {
			if (isWinner[atoi(candidate)]) {
				++wins;
			}
			candidate = strtok_r(NULL, " ", &nextCandidate);
		}

		u32 maxWins = MAX_LINES - lineNum;
		wins = wins > maxWins ? maxWins : wins;
		for (u32 j = cardCopies[lineNum]; j > 0; --j) {
			for (u32 i = wins; i > 0; --i) {
				++cardCopies[lineNum + i];
			}
		}
		printf("%zu\n", lineNum);

		cardTotal += cardCopies[lineNum];
		++lineNum;
	}
	free(line);

	printf("\n%d\n", cardTotal);

	return 0;
}
