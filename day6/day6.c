#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef int32_t i32;
typedef size_t size;

i32 main(void)
{
	size timesLen = 0, distancesLen = 0;
	size times[4], distances[4];

	size lineSize = 100;
	char line[lineSize];
	fgets(line, lineSize, stdin);
	char *token;
	strtok(line, " ");
	while ((token = strtok(NULL, " "))) {
		times[timesLen++] = atoll(token);
	}

	fgets(line, lineSize, stdin);
	strtok(line, " ");
	while ((token = strtok(NULL, " "))) {
		distances[distancesLen++] = atoll(token);
	}

	size total = 1;
	for (size i = 0; i < timesLen; ++i) {
		size winningSpeeds = 0;
		for (size speed = 1; speed < times[i]; ++speed) {
			size timeLeft = times[i] - speed;
			size distance = timeLeft * speed;
			if (distance > distances[i]) {
				++winningSpeeds;
			}
		}
		if (winningSpeeds != 0) {
			total *= winningSpeeds;
		}
	}

	printf("%zu\n", total);

	return 0;
}
