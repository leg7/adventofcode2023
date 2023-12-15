#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t u32;

int main(void) {
	u32 pointsSum = 0;

	size_t lineSize = 150;

	char *line = malloc(lineSize);
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
		u32 points = 0;
		while (candidate != NULL) {
			if (isWinner[atoi(candidate)]) {
				if (points == 0) {
					points = 1;
				} else {
					points *= 2;
				}
			}
			candidate = strtok_r(NULL, " ", &nextCandidate);
		}

		pointsSum += points;
	}
	free(line);

	printf("%d", pointsSum);

	return 0;
}
