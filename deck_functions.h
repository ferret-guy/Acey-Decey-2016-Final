#ifndef DECK_FUNCTIONS_H_
#define DECK_FUNCTIONS_H_

// Creates a card type including a suit, face value, and pointer
typedef struct card_s {
	char suit;
	int face;
	struct card_s* listP;
} card;

void create_deck(card *deck[]);
void shuffle_deck2(card *deck[]);
void shuffle_deck(card *deck[]);
void print_deck(card *deck[]);
void card_insertAfter(card *thisCard, card *newCard);
void card_create(card *thisCard, char inSuit, int inFace, card *nextCard);
void swap_cards(card* card1, card* card2);

#endif  // DECK_FUNCTIONS_H_
