#include "deck_functions.h"
#include <stdlib.h>
#include <string.h>
#include<stdio.h>

typedef struct player_s {
	char name[100];
	float bank_roll;
	struct player_s *next;
} player;

int main(void) {
	int numOfPlayers, i;
	player * head = malloc(sizeof(struct player_s));
	head->next = NULL;
	player * curr;
	card * deck[52];

	printf("Welcome to Acey Deucy!\n\nPlease enter the number of players: ");
	scanf("%d", &numOfPlayers);
	if (numOfPlayers < 1) {
		printf("Cannot have 0 players try again");
		exit(0);
	}

	curr = head;
	for (int i = 1; i < numOfPlayers+1; i++) {
		// Initilize current node
		curr->next = malloc(sizeof(struct player_s));
		curr = curr->next;
		curr->next = NULL;
		// Get Player input
		char name[100];
		float bank_roll;
		printf("\nEnter player %d's name: ", i);
		scanf("%100s", name);
		printf("Enter player %d's bank ammount: ", i);
		scanf("%f", &bank_roll);
		// Set Values
		strncpy(curr->name, name, 100);
		curr->bank_roll = bank_roll;
	}
	curr = head;
	while(curr->next != NULL) {
		curr = curr->next;
		printf("Player name: %s, Player cash: $%.2f\n", curr->name, curr->bank_roll);
	}

	create_deck(deck);
	shuffle_deck(deck);
	print_deck(deck);
}
