#include<stdio.h>
#include<math.h>
#include<string.h>
#include "deck_functions.h"
#include "print_functions.h"

void print_card(char suit, int face){				//prints given card
	char suitA[4];
	char faceA[] = "2 ";	
	if(face == 11)
		faceA[0] = 'J';
	if(face == 12)
		faceA[0] = 'Q';
	if(face == 13)
		faceA[0] = 'K';
	if(face == 14)
		faceA[0] = 'A';
	else
		faceA[0] = (char)face;
	if(suit == 's')
		strcpy(suitA, SPADE);
	if(suit == 'h')
		strcpy(suitA, HEART);
	if(suit == 'd')
		strcpy(suitA,DIAMOND);
	if(suit == 'c')
		strcpy(suitA, CLUB);
	
	printf("+-----+\n");
	printf("|%s%s  |\n", faceA, suitA);
	printf("|     |\n");
	printf("|     |\n");
	printf("|  %s%s|\n", faceA, suitA);
	printf("+-----+\n");
	return;
}

int main(void){
	print_card('c', 13);
	return 0;
}