void shuffle_deck(card* deck[]) {
	int i, j, randNum;
	srand((int)time(0));
	card* tmpDeck[52];
	tmpDeck[0] = deck[0];
	for (i = 1; i < 52; i++) {
		randNum = (rand() % (i + 1));
		if (randNum == i) {
			tmpDeck[i] = deck[i];
			card_insertAfter(tmpDeck[randNum - 1], tmpDeck[randNum]);
		}
		else {
			for (j = i - 1; j >= randNum; j--) {
				tmpDeck[j + 1] = tmpDeck[j];
			}
			tmpDeck[randNum] = deck[i];
			if (randNum == 0) {
				tmpDeck[randNum] = deck[i];
				tmpDeck[randNum]->listP = tmpDeck[randNum + 1];
			}
			else {
				card_insertAfter(tmpDeck[randNum - 1], tmpDeck[randNum]);
			}
		}
	}
	tmpDeck[51]->listP = NULL;
	for (i = 0; i < 52; i++) {
		deck[i] = tmpDeck[i];
	}
}
