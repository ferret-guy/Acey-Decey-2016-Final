#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <wchar.h>
#include <time.h>

#include "deck_functions.h"

// Creates a card with given face, suit, and pointer
void card_create(card* thisCard, wchar_t inSuit, int inFace, card* nextCard) {
	thisCard->face = inFace;
	thisCard->suit = inSuit;
	thisCard->listP = nextCard;
	return;
}

void card_insertAfter(card* thisCard, card* newCard) {
	card* tmpNext = NULL;
	tmpNext = thisCard->listP;
	thisCard->listP = newCard;
	newCard->listP = tmpNext;
	return;
}

void swap_cards(card* card1, card* card2) {
	card tmpCard;
	tmpCard.face = card1->face;
	tmpCard.suit = card1->suit;
	tmpCard.listP = NULL;
	card1->face = card2->face;
	card1->suit = card2->suit;
	card2->face = tmpCard.face;
	card2->suit = tmpCard.suit;
	return;
}

void print_deck(card* deck[]) {
	for (int i = 0; i < 52; i++) {
		switch(deck[i]->face) {
			case 11:
				wprintf(L"Jack of %c\n", deck[i]->suit);
				break;
			case 12:
				wprintf(L"Queen of %c\n", deck[i]->suit);
				break;
			case 13:
				wprintf(L"King of %c\n", deck[i]->suit);
				break;
			case 14:
				wprintf(L"Ace of %c\n", deck[i]->suit);
				break;
			default:
				wprintf(L"%d of %c\n", deck[i]->face, deck[i]->suit);
		}
	}
}

void shuffle_deck(card* deck[]) {
	srand((int) time(0));
	card* tmpDeck[52];
	tmpDeck[0] = deck[0];

	for (int i = 1; i < 52; i++) {
		int randNum = (rand() % (i + 1));
		if (randNum == i) {
			tmpDeck[i] = deck[i];
			card_insertAfter(tmpDeck[randNum - 1], tmpDeck[randNum]);
		} else {
			for (int j = i - 1; j >= randNum; j--) {
				tmpDeck[j + 1] = tmpDeck[j];
			}
			tmpDeck[randNum] = deck[i];
			if (randNum == 0) {
				tmpDeck[randNum] = deck[i];
				tmpDeck[randNum]->listP = tmpDeck[randNum + 1];
			} else {
				card_insertAfter(tmpDeck[randNum - 1], tmpDeck[randNum]);
			}
		}
	}
	tmpDeck[51]->listP = NULL;
	for (int i = 0; i < 52; i++) {
		deck[i] = tmpDeck[i];
	}
}

void shuffle_deck2(card* deck[]) {
	srand((int) time(0));
	for(int j = 0; j < 600; j++) {
		for(int i = 0; i < 52; i++) {
			swap_cards(deck[i], deck[(rand()%52)]);
		}
	}
}

void create_deck(card* deck[]) {
	wchar_t suit = L'♠';
	int face = 14;

	for (int i = 0; i < 52; i++) {
		deck[i] = NULL;
		deck[i] = (card*)malloc(sizeof(card));
	}
	// Create entire deck in order A-K  spade->heart->club->diamond
	// 2-10, 11:Jack, 12:Queen, 13:King, 14:Ace
	for (int i = 0; i < 52; i++) {
		card_create(deck[i], suit, face, NULL);
		if (face == 13) {
			if (suit == L'♠') {
				suit = L'♥';
			} else if (suit == L'♥') {
				suit = L'♣';
			} else if (suit == L'♣') {
				suit = L'♦';
			}
		}
		if (face == 14) {
			face = 2;
		} else {
			face++;
		}
	}
	for (int i = 0; i < 51; i++) {
		card_insertAfter(deck[i], deck[i + 1]);
	}
}
