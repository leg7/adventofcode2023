#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef int32_t i32;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

/*
* I represent a node by sticking the ascii value of each of it's characters together
* to for example the the node "ZZZ" is concat(95,95,95) -> 959595
*/
#define ZZZ 909090
#define AAA 656565
#define NODES_RANGE_MIN 0
#define NODES_RANGE_MAX (ZZZ - AAA)
#define NODES_RANGE_SIZE (NODES_RANGE_MAX + 1)


struct NodesOut
{
	u32 left, right;
};

u32 nodeOfChars(const char a, const char b, const char c)
{
	return (10000 * a + 100 * b + c) - AAA;
}

int main(void)
{
	struct NodesOut hashTable[NODES_RANGE_SIZE];
	for (u32 i = 0; i < NODES_RANGE_SIZE; ++i) {
		hashTable[i] = (struct NodesOut){ 0, 0 };
	}

	u16 directionsSize = 300;
	char directions[directionsSize];
	fgets(directions, directionsSize, stdin);

	u8 lineSize = 18;
	char line[lineSize];
	fgets(line, lineSize, stdin); // ignore empty line

	while ((fgets(line, lineSize, stdin) != NULL)) {
		u32 nodeIn = nodeOfChars(line[0], line[1], line[2]);
		hashTable[nodeIn] = (struct NodesOut) {
				nodeOfChars(line[7], line[8], line[9]),
				nodeOfChars(line[12], line[13], line[14])
		};
	}

	u16 directionsLen = strlen(directions) - 1;
	u32 currentNode = NODES_RANGE_MIN;
	u32 steps = 0;
	// This assumes you have to repeat the full direction list atleast 1 time to find the exit
	while (currentNode != NODES_RANGE_MAX) {
		for (u16 i = 0; i < directionsLen; ++i) {
			switch (directions[i]) {
				case 'L': currentNode = hashTable[currentNode].left; break;
				case 'R': currentNode = hashTable[currentNode].right; break;
				default: exit(69);
			}
		}
		steps += directionsLen;
	}

	printf("steps: %d", steps);

	return 0;
}
