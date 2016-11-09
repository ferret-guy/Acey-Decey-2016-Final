#include "deck_functions.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <wchar.h>

#ifdef linux
#include <sys/io.h>
#endif

typedef struct player_s {
	wchar_t name[100];
	float bank_roll;
	struct player_s *next;
} player;

int main(void) {
	// Set the console to unicode mode
	_setmode(_fileno(stdout), 0x00040000);

	int numOfPlayers, i;
	player * head = malloc(sizeof(struct player_s));
	head->next = NULL;
	player * curr;
	card * deck[52];

	wprintf(L"Welcome to Acey Deucy!\n\nPlease enter the number of players: ");
	scanf("%d", &numOfPlayers);
	if (numOfPlayers < 1) {
		wprintf(L"Cannot have 0 players try again");
		exit(0);
	}

	curr = head;
	for (int i = 1; i < numOfPlayers+1; i++) {
		// Initilize current node
		curr->next = malloc(sizeof(struct player_s));
		curr = curr->next;
		curr->next = NULL;
		// Get Player input
		wchar_t name[100];
		float bank_roll;
		wprintf(L"\nEnter player %d's name: ", i);
		fseek(stdin, 0, SEEK_END);
		wscanf(L"%99[0-9a-zA-Z ]", name);
		wprintf(L"Enter player %d's bank ammount: ", i);
		wscanf(L"%f", &bank_roll);
		// Set Values
		wcsncpy(curr->name, name, 100);
		curr->bank_roll = bank_roll;
	}
	curr = head;
	while(curr->next != NULL) {
		curr = curr->next;
		wprintf(L"Player name: %s, Player cash: $%.2f\n",
				curr->name, curr->bank_roll);
	}

	create_deck(deck);
	shuffle_deck(deck);
	print_deck(deck);
}
