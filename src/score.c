/* score.c */

#include "score.h"

char * scoreFileName = "scores.txt";

void initScoreBoard(struct scores gameScores) {
	gameScores.currentPlayerScore = 0;
	int i;
	for (i = 0; i < NUMSCORES; i++){
		gameScores.highScoreBoardInits[i] = malloc(sizeof(char) * 3);
		if(gameScores.highScoreBoardInits[i] == NULL) {
			printf("Error in mallocing scoreboard Initials space. \n");
		}
	}
	getHighScoreBoard(gameScores);
}

void getHighScoreBoard(struct scores gameScores) {
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
	return;
}

void updateHighScoreBoard(struct scores gameScores) {
	int playerScore = getCurrentPlayerScore(gameScores);
	unsigned short i;
	for(i = 0; i < NUMSCORES; i++) {
		if(playerScore > gameScores.highScoreBoard[i]) {

		}
	}
}

void updateCurrentPlayerScore(int deltaScore, struct scores gameScores) {

}

int getCurrentPlayerScore(struct scores gameScores) {
	return -1;
}
