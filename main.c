#include "deck_functions.h"
#include <windows.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <wchar.h>

typedef struct player_s {
	wchar_t name[100];
	float bank_roll;
	struct player_s *next;
} player;

// Used to keep track of the current card, no generators in c :(
int cardNumber = 0;
// Used in order for the signal handeler to work
player * signalHead;

// Function to set cursor position (only works on windows)
void gotoxy(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	fflush(stdout);
}

// Prints card in specified 0 indexed position
void print_card(int i, card* c){
	// Set Chars to be printed
	wchar_t cardChar[4];
	switch(c->face) {
			case 11:
				wsprintfW(cardChar, L"J%c", c->suit);
				break;
			case 12:
				wsprintfW(cardChar, L"Q%c", c->suit);
				break;
			case 13:
				wsprintfW(cardChar, L"K%c", c->suit);
				break;
			case 14:
				wsprintfW(cardChar, L"A%c", c->suit);
				break;
			default:
				wsprintfW(cardChar, L"%d%c", c->face, c->suit);
	}
	gotoxy(i * 7, 0);
	wprintf(L"+-----+");
	gotoxy(i * 7, 1);
	wprintf(L"|%-3s  |", cardChar);
	gotoxy(i * 7, 2);
	wprintf(L"|     |");
	gotoxy(i * 7, 3);
	wprintf(L"|     |");
	gotoxy(i * 7, 3);
	wprintf(L"|  %3s|", cardChar);
	gotoxy(i * 7, 4);
	wprintf(L"+-----+");
}

// Caculates the pot ante
int player_ante(player * head){
	int ante = 0;
	player * curr = head;
	curr = head;
	while(curr->next != NULL) {
		curr = curr->next;
		ante += 5;
		curr->bank_roll -= 5;
	}
	return ante;
}

// Gets the next card from the deck shuffling if necessary
card * nextCard(card * deck[]){
	if (cardNumber >= 49){
		shuffle_deck(deck);
		cardNumber = 0;
		wprintf(L"Deck Shuffled, print deck? (y/n): ");
		fseek(stdin, 0, SEEK_END);
		wchar_t c = 0;
		wscanf(L"%1c", &c);
		if (c == L'Y' || c == L'y') {
			print_deck(deck);
			fseek(stdin, 0, SEEK_END);
			wprintf(L"Press enter to continue");
			while (c != L'\n') { c = fgetwc(stdin); }
			c = 0;
			system("cls");
		}
	}
	cardNumber++;
	return deck[cardNumber - 1];
}

player * readInFile(char fileName[]){
	FILE * inFile = fopen(fileName, "r");
	player* head = malloc(sizeof(struct player_s));
	head->next = NULL;
	player* curr = head;
	wchar_t name[100];
	float bank_roll;
	while (fwscanf(inFile, L"%99[^\n\t]\t%f\n", name, &bank_roll) == 2){
		// Initilize current node
		curr->next = malloc(sizeof(struct player_s));
		curr = curr->next;
		curr->next = NULL;
		wcsncpy(curr->name, name, 100);
		curr->bank_roll = bank_roll;
	}
	return head;
}

// Writes the output file
void writeOutFile(player * head){
	FILE * outFile = fopen("out.txt", "w");
	player * curr = head;
	while(curr->next != NULL) {
		curr = curr->next;
		fwprintf(outFile, L"%s\t%f\n", curr->name, curr->bank_roll);
	}
	fclose(outFile);
}

// Quit Signal handeler
void quitHandeler(int sig){
	system("cls");
	wprintf(L"Writting current players and bankrolls to file...");
	writeOutFile(signalHead);
	exit(0);
}

// Checks if anyone has won, and declares the winner exiting the progam
void checkWin(player * head, int pot){
	int playersWinning = 0;
	player * curr = head;
	player * winner;
	curr = head;
	while(curr->next != NULL) {
		curr = curr->next;
		if (curr->bank_roll > 0){
			playersWinning++;
			winner = curr;
		}
	}
	if (playersWinning == 0){
		wprintf(L"Game Over everyone is out of money!");
		writeOutFile(head);
		exit(0);
	} else if (playersWinning == 1){
		wprintf(L"%s Wins with $%.2f!", winner->name, winner->bank_roll);
		writeOutFile(head);
		exit(0);
	} else if (pot <= 0){
		wprintf(L"Game Over pot is out of money!");
		wprintf(L"Final standings:");
		curr = head;
		while(curr->next != NULL) {
			curr = curr->next;
			wprintf(L"%s has $%.2f", curr->name, curr->bank_roll);
		}
		writeOutFile(head);
		exit(0);
	}
}

//Function that prints a status bar
void statusBar(player * head, player * active){
	gotoxy(0, 17);
	player * curr = head;
	curr = head;
	while(curr->next != NULL) {
		curr = curr->next;
		// Get a nick name thjat is the first 10 chars or until a space
		wchar_t nick[11];
		swscanf(curr->name, L"%10s", &nick);
		if (curr == active){
			wprintf(L"->  %10s  $%.2f\n\n", nick, curr->bank_roll);
		} else {
			wprintf(L"    %10s  $%.2f\n\n", nick, curr->bank_roll);
		}
	}
}

// Function to prompt for players and returns the head of the linked list
player * get_players() {
	wchar_t c;
	// Check if we already have an infile
	if (access("out.txt", R_OK) == 0){
		wprintf(L"out.txt exists load it (y,n)? ");
		fseek(stdin, 0, SEEK_END);
		wscanf(L"%1c", &c);
		if ((c == L'Y' || c == L'y')){
			return readInFile("out.txt");
		}
	}
	// Initilize player linked list
	int numOfPlayers = 0;
	player* head = malloc(sizeof(struct player_s));
	head->next = NULL;
	player* curr;

	curr = head;
	for (;;) {
		// Initilize current node
		curr->next = malloc(sizeof(struct player_s));
		curr = curr->next;
		curr->next = NULL;
		// Get Player input
		wchar_t name[100];
		float bank_roll = 0;
		wprintf(L"\nEnter player %d's name: ", numOfPlayers+1);
		fseek(stdin, 0, SEEK_END);
		wscanf(L"%99[^\n\t]", name);
		while (bank_roll < 5) {
			wprintf(L"Enter player %d's bank ammount: ", numOfPlayers+1);
			fseek(stdin, 0, SEEK_END);
			wscanf(L"%f", &bank_roll);
			if (bank_roll < 5) {
				wprintf(L"You must have at least $5 to play!\n");
			}
		}
		// Set Values
		wcsncpy(curr->name, name, 100);
		curr->bank_roll = bank_roll;
		numOfPlayers++;
		if (numOfPlayers >= 4) {
			wprintf(L"Do you wish to add another player (y,n)? ");
			fseek(stdin, 0, SEEK_END);
			wscanf(L"%1c", &c);
			if (!(c == L'Y' || c == L'y')) {break;}
		}
	}
	return head;
}

// Main game function
int main(int argc, char **argv) {
	player * head;
	// Check inputs
	if (argc == 2 && access(argv[1], R_OK) == 0){
		// Set the console to unicode mode
		_setmode(_fileno(stdout), 0x00040000);
		wprintf(L"Reading file from input...\n");
		head = readInFile(argv[1]);
	} else if(argc == 2 && access(argv[1], R_OK) != 0){
		printf("Cannot read from %s", argv[1]);
		exit(0);
	} else if (argc > 2){
		printf("Usage: %s <player input file>", argv[0]);
		exit(0);
	} else {
		// Set the console to unicode mode
		_setmode(_fileno(stdout), 0x00040000);
		// Get Player Linked List
		head = get_players();
	}

	// Allow the signal handler to work
	signalHead = head;
	player * curr = head;

	// Write the output file on Ctrl-C
	signal(SIGINT, quitHandeler);

	// Initilize char
	wchar_t c = 0;

	// Initilize Deck
	card * deck[52];
	create_deck(deck);
	shuffle_deck(deck);
	wprintf(L"Deck Shuffled, print deck? (y/n): ");
	fseek(stdin, 0, SEEK_END);
	wscanf(L"%1c", &c);
	if (c == L'Y' || c == L'y') {
		print_deck(deck);
		c = 0;
		fseek(stdin, 0, SEEK_END);
		wprintf(L"Press enter to continue");
		while (c != L'\n') { c = fgetwc(stdin); }
		c = 0;
	}

	// Initilize main loop
	curr = head;
	float bet = 0;
	float pot = player_ante(head);
	int roundOne = 1;
	card *card1, *card2, *card3;
	// Run the game
	for(;;) {
		// Iterate through the linked list of people
		while(curr->next != NULL) {
			// Advance to the next person and clear the screen
			curr = curr->next;
			system("cls");

			// Check if this person is in play
			if (curr->bank_roll == 0){
				wprintf(L"%s Does not have enough money to play!\n", curr->name);
				curr->bank_roll = -1;
				goto end;
			} else if (curr->bank_roll < 0){
				goto stop;
			}

			// Print the status bar
			statusBar(head, curr);

			// Get first set of cards
			card1 = nextCard(deck);
			card3 = nextCard(deck);
			// Print cards
			print_card(0, card1);
			print_card(2, card3);
			gotoxy(0, 6);
			if (card1->face == card3->face){
				if (curr->bank_roll < 1){
					wprintf(L"Cards are the same %s forfeits your bankroll", curr->name);
					pot += curr->bank_roll;
					curr->bank_roll = 0;
				} else {
					wprintf(L"Cards are the same %s forfeits $1", curr->name);
					pot += 1;
					curr->bank_roll -= 1;
				}
				goto end;
			}
			wprintf(L"Pot: $%.2f\n", pot);
			wprintf(L"%s's turn, you have $%.2f to bet (enter 0 to forfeit the round):\n",
					curr->name, curr->bank_roll);
			// Get bet
			for(;;) {
				wprintf(L"Bet: ");
				fseek(stdin, 0, SEEK_END);
				wscanf(L"%f", &bet);
				if (roundOne == 1  && bet > 1) {
					wprintf(L"Can bet no more than $1 during first round!\n");
				} else if (bet > curr->bank_roll) {
					wprintf(L"Can't bet more than you have in the bank!\n");
				} else if (bet < 0) {
					wprintf(L"Must bet more than $0!\n");
				} else if (bet == 0) {
					wprintf(L"You forfeit the round!\n");
					goto end;
				} else {break;}
			}
			// Print the final card
			card2 = nextCard(deck);
			print_card(1, card2);

			gotoxy(0, 5);
			// Check if card is inbetween the others
			if ((card1->face < card2->face && card2->face < card3->face) || 
				(card3->face < card2->face && card2->face < card1->face)){
				if (pot > bet){
					wprintf(L"You win your bet!");
					pot -= bet;
					curr->bank_roll += bet;
				} else {
					wprintf(L"You get the whole pot!");
					curr->bank_roll += pot;
					pot = 0;
				}
			}
			// Check if card is eather of the end cards
			else if (card1->face == card2->face || card2->face == card3->face){
				if (curr->bank_roll >= bet*2){
					wprintf(L"Cards are the same! you lose double your bet!");
					pot += bet*2;
					curr->bank_roll -= bet*2;
				} else {
					wprintf(L"Cards are the same! you lose your whole bankroll!");
					pot += curr->bank_roll;
					curr->bank_roll = 0;
				}
			}
			// Check if card is outside range
			else if ((card1->face < card2->face && card2->face > card3->face) || 
					 (card1->face > card2->face && card2->face < card3->face)){
				wprintf(L"You lose your bet to the pot!");
				curr->bank_roll -= bet;
				pot += bet;
			}
			// I'm so sorry
			end:
			gotoxy(0, 14);
			checkWin(head, pot);
			fseek(stdin, 0, SEEK_END);
			wprintf(L"Press enter to continue");
			while (c != L'\n') { c = fgetwc(stdin); }
			stop:
			c = 0;
		}
		// Reset the player pointer for the next round
		curr = head;
		roundOne = -1;
	}
}
