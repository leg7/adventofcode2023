#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <numeric>

typedef int32_t i32;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef size_t size;

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

	// I know there are no more than 10 starting Nodes
	u32 startNodes[10] = { 0 }, startNodesLen = 0;

	while ((fgets(line, lineSize, stdin) != NULL)) {
		u32 nodeIn = nodeOfChars(line[0], line[1], line[2]);

		if (line[2] == 'A') {
			startNodes[startNodesLen++] = nodeIn;
		}

		hashTable[nodeIn] = (struct NodesOut) {
				nodeOfChars(line[7], line[8], line[9]),
				nodeOfChars(line[12], line[13], line[14])
		};
	}

	u16 directionsLen = strlen(directions) - 1;

	for (u8 n = 0; n < startNodesLen; ++n) {
		u32 steps = 0;
		bool keepGoing = true;
		while (keepGoing) {
			for (u16 i = 0; i < directionsLen && keepGoing; ++i) {
				switch (directions[i]) {
					case 'L': startNodes[n] = hashTable[startNodes[n]].left; break;
					case 'R': startNodes[n] = hashTable[startNodes[n]].right; break;
					default: exit(69);
				}
				++steps;
				if ((startNodes[n] + AAA) % 100 == 'Z') {
					startNodes[n] = steps;
					keepGoing = false;
				}
			}
		}
	}

	return 0;
}
