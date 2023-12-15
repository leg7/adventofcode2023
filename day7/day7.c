#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef size_t size;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

enum Card
{
	CARD_2,
	CARD_3,
	CARD_4,
	CARD_5,
	CARD_6,
	CARD_7,
	CARD_8,
	CARD_9,
	CARD_T,
	CARD_J,
	CARD_Q,
	CARD_K,
	CARD_A,
};
#define CARD_MEMBERS 13

enum Card charToCard(const char c)
{
	if (c >= '2' && c <= '9') {
		return c - '2';
	}
	switch (c) {
		case 'T': return CARD_T;
		case 'J': return CARD_J;
		case 'Q': return CARD_Q;
		case 'K': return CARD_K;
		case 'A': return CARD_A;
		default: exit(69);
	}
}

enum HandType
{
	HANDTYPE_HIGHCARD  = 0b00000,
	HANDTYPE_ONEPAIR   = 0b00001,
	HANDTYPE_TWOPAIR   = 0b00010,
	HANDTYPE_3OAK      = 0b00100,
	HANDTYPE_FULLHOUSE = 0b00101,
	HANDTYPE_4OAK,
	HANDTYPE_5OAK,
};

enum HandType HandTypeOfCardCounts(const u8 cardCounts[CARD_MEMBERS])
{
	enum HandType result = HANDTYPE_HIGHCARD;

	for (u8 i = 0; i < CARD_MEMBERS; ++i) {
		switch (cardCounts[i]) {
			case 2:
				++result;
			break;
			case 3: result += HANDTYPE_3OAK; break;
			case 4: result = HANDTYPE_4OAK; break;
			case 5: result = HANDTYPE_5OAK; break;
		}
	}

	return result;
}

int HandTypecompare(const enum HandType a, const enum HandType b)
{
	return a - b;
}

struct Turn
{
	enum Card hand[5];
	enum HandType handType;
	u16 bid;
};

int TurnCompare(const void *a, const void *b)
{
	const struct Turn *TurnA = (const struct Turn*)a;
	const struct Turn *TurnB = (const struct Turn*)b;

	int comp = TurnA->handType - TurnB->handType;
	u8 card = 0;
	while (card < 5 && comp == 0) {
		comp = TurnA->hand[card] - TurnB->hand[card];
		++card;
	}
	return comp;
}

int main(void)
{
	char input;
	size turnsLength = 0, turnsSize = 1000;
	struct Turn turns[turnsSize];

	while ((input = getchar()) != EOF) {
		u8 cardCounts[CARD_MEMBERS] = { 0 };
		for (u8 i = 0; i < 5; ++i) {
			turns[turnsLength].hand[i] = charToCard(input);
			cardCounts[turns[turnsLength].hand[i]]++;
			input = getchar();
		}
		turns[turnsLength].handType = HandTypeOfCardCounts(cardCounts);

		turns[turnsLength].bid = getchar() - '0';
		while ((input = getchar()) != '\n') {
			turns[turnsLength].bid = (turns[turnsLength].bid * 10) + (input - '0');
		}

		++turnsLength;
	}

	qsort(turns, turnsLength, sizeof(struct Turn), TurnCompare);

	size total = 0;
	for (size i = 0; i < turnsLength;) {
		total += turns[i].bid * ++i;
	}
	printf("Total: %zu", total);

	return 0;
}
