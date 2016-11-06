#define SPADE   "♠"
#define CLUB    "♣"
#define HEART   "♥"
#define DIAMOND "♦"

//be sure to use the command: "chcp 65001" before running the code
/*
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
	if(suit == 's')
		strcpy(suitA, SPADE);
	if(suit == 'h')
		strcpy(suitA, HEART);
	if(suit == 'd')
		strcpy(suitA,DIAMOND);
	if(suit == 'c')
		strcpy(suitA, CLUB);
	
	printf("______\n");
	printf("|%s  |\n", faceA);
	printf("|%s   |\n", suitA);
	printf("|   %s|\n", suitA);
	printf("|__%s|\n", faceA);
	return;
}
*/