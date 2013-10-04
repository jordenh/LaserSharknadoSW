/* score.c */

#include "score.h"

struct scores gameScores;
static char * scoreFileName = "scores.txt";

void initScoreBoard(void) {
	gameScores.currentPlayerScore = 0;
	int i;
	for (i = 0; i < NUMSCORES; i++){
		gameScores.highScoreBoardInits[i] = malloc(sizeof(char) * 3);
	}
	getHighScoreBoard();
}

void getHighScoreBoard(void) {
	short int fileHandle = openFile(scoreFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", scoreFileName);
		return;
	}

	char readValue;
	int currentScore;
	int numDigits;
	int scoreBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
	int i,j,k;
	for(i = 0; i < NUMSCORES; i++) {
		for(j = 0; j < NUMINITIALS; j++){
			readValue = readByte(fileHandle);
			gameScores.highScoreBoardInits[i][j] = readValue;
		}
		readValue = readByte(fileHandle);
		/*
		 * if(readValue != 0x20) {
		 * 		return ERROR;
		 * }
		 */
		//printf("test: %c%c%c\n", gameScores.highScoreBoardInits[i][0],gameScores.highScoreBoardInits[i][1],gameScores.highScoreBoardInits[i][2]);

		for(k = 0; k < 10; k++){
			scoreBuffer[k] = 0;
		}
		k = 0;
		numDigits = 0;
		while((readValue = readByte(fileHandle)) != 0x20) { //loop until space found
			scoreBuffer[k] = readValue;
			numDigits++;
			k++;
		}
		currentScore = 0;
		for(k = 0; k < numDigits; k++) {
			currentScore += (scoreBuffer[k] - '0') * pow(10,(numDigits - 1 - k));
		}
		gameScores.highScoreBoard[i] = currentScore;

		//printf("score: %d\n", currentScore);
	}

	closeFile(fileHandle);
}

void updateHighScoreBoard(void) {
	int maxScore = -1;
	int playerScore = getCurrentPlayerScore();
	unsigned short i;
	for(i = 0; i < NUMSCORES; i++) {
		if(playerScore > gameScores.highScoreBoard[i]) {

		}
	}
}

void updateCurrentPlayerScore(int deltaScore) {

}

int getCurrentPlayerScore(void) {

}
