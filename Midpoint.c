#include "deck_functions.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <wchar.h>

#ifdef linux
#include <bsd/unistd.h>
#endif

int main(void) {
	card * deck[52];

	// Set the console to unicode mode
	_setmode(_fileno(stdout), 0x00040000);

	wprintf(L"\nCreating deck...\n");
	create_deck(deck);

	wprintf(L"\nCreated Deck:\n");
	print_deck(deck);

	wprintf(L"\nShuffling Deck...\n");
	shuffle_deck(deck);

	wprintf(L"\nShuffed Deck:\n");
	print_deck(deck);
}
