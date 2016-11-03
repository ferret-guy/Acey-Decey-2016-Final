#include<stdio.h>
#include "deck_functions.h"


int main(void){
	int numOfPlayers, i;
	
	printf("Welcome to Acey Deucy!\n\nPlease enter the number of players: ");
	scanf("%d", &numOfPlayers);
	card* deck[52];
	create_deck(deck);
	shuffle_deck(deck);
	print_deck(deck);
}