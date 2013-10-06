/* score.c */

#include "score.h"

char * scoreFileName = "scores.txt";

void initScoreBoard(struct scores * gameScores) {
	gameScores->currentPlayerScore = 125; // TBD - set to 0 - testing code by setting to other values.
	gameScores->currentPlayerLives = INITIALLIVES;
	getHighScoreBoard(gameScores);
	/*int i;
	for (i = 0; i < NUMSCORES; i++){
		//gameScores.highScoreBoardInits[i] = malloc(sizeof(char) * 3);
		//printf("size of gameScore: %x, at %x",(sizeof(char) * 3), (unsigned int)gameScores.highScoreBoardInits[i]);
		if(gameScores->highScoreBoardInits[i] == NULL) {
			printf("Error in mallocing scoreboard Initials space. \n");
		}
	}*/

}

void defaultHighScoreBoard(struct scores * gameScores) {
	int i,j;
	for(i = 0; i < NUMSCORES; i++) {
		gameScores->highScoreBoard[i] = 1;
		for(j = 0; j < NUMINITIALS; j++) {
			gameScores->highScoreBoardInits[i][j] = 'X';
		}
	}
	return;
}

//returns -1 on error in reading scoreboard (and also defaults all current values), returns 0 on success.
int getHighScoreBoard(struct scores * gameScores) {
	short int fileHandle = openFile(scoreFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", scoreFileName);
		defaultHighScoreBoard(gameScores);
		return -1;
	}

	char readValue;
	int currentScore;
	int numDigits;
	int scoreBuffer[MAXSCOREDIGITS];
	int i,j,k;
	for(i = 0; i < NUMSCORES; i++) {
		for(j = 0; j < NUMINITIALS; j++){
			readValue = readByte(fileHandle);
			gameScores->highScoreBoardInits[i][j] = readValue;
		}
		readValue = readByte(fileHandle);

		if(readValue != 0x20) {
			closeFile(fileHandle);
			printf("Error within %s - file not initialized to proper format. Scoreboard defaulted.", scoreFileName);
			defaultHighScoreBoard(gameScores);
			return -1;
		}
		//printf("test: %c%c%c\n", gameScores.highScoreBoardInits[i][0],gameScores.highScoreBoardInits[i][1],gameScores.highScoreBoardInits[i][2]);

		//clear score buffer
		for(k = 0; k < MAXSCOREDIGITS; k++){
			scoreBuffer[k] = -1;
		}

		numDigits = 0;
		while((readValue = readByte(fileHandle)) != 0x20 && numDigits < MAXSCOREDIGITS) { //loop until space found
			scoreBuffer[numDigits] = readValue;
			numDigits++;
		}
		if(readValue != 0x20) {
			closeFile(fileHandle);
			printf("Error within %s - file not initialized to proper format. Scoreboard defaulted.", scoreFileName);
			defaultHighScoreBoard(gameScores);
			return -1;
		}

		//use scoreBuffer values (stored in char values) into a meaningful integer, currentScore.
		currentScore = 0;
		for(k = 0; k < numDigits; k++) {
			currentScore += (scoreBuffer[k] - '0') * pow(10,(numDigits - 1 - k));
		}
		gameScores->highScoreBoard[i] = currentScore;

		//printf("score: %d\n", currentScore);
	}
	for(i = 0; i < NUMSCORES; i++){
		printf("test: %c%c%c\n", gameScores->highScoreBoardInits[i][0],gameScores->highScoreBoardInits[i][1],gameScores->highScoreBoardInits[i][2]);
		printf("score: %d\n", gameScores->highScoreBoard[i]);
	}
	closeFile(fileHandle);
	return 0;
}

void updateHighScoreBoard(struct scores * gameScores) {
	int playerScore = getCurrentPlayerScore(gameScores);
	unsigned short i,j;
	short scoreReplaceIndex = -1;
	unsigned short firstDigitFound = 0;
	unsigned short digit;
	short int fileHandle = openFile(scoreFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", scoreFileName);
		return;
	}

	//determine if playerScore is better than any high scores
	for(i = 0; i < NUMSCORES; i++) {
		if(playerScore > gameScores->highScoreBoard[i]) {
			printf("I found a score, %d, that is higher than other scores. Index %d\n", playerScore, i);
			scoreReplaceIndex = i;
			break; // current index needs to be replaced by
		}
	}

	//if playerScore is better than a high score, re-write to the SD card, placing currentPlayerScore in the file.
	if(scoreReplaceIndex != -1) {
		int loopCount = NUMSCORES;
		for(i = 0; i < loopCount; i++) {
			if(i == scoreReplaceIndex){
				printf("replacing score code at index %d\n", i);
				char newName[3] = {'Y', 'O', 'U'};
				for(j = 0; j < NUMINITIALS; j++){
					alt_up_sd_card_write(fileHandle, newName[j]);
				}
				alt_up_sd_card_write(fileHandle, ' ');
				firstDigitFound = 0;
				for(j = 0; j < MAXSCOREDIGITS; j++){
					digit = ((int)(gameScores->currentPlayerScore / pow(10,(MAXSCOREDIGITS - 1 - j)))%10);
					if(digit != 0 && firstDigitFound == 0){
						firstDigitFound = 1;
					}
					if(firstDigitFound == 1){
						alt_up_sd_card_write(fileHandle, (digit + '0'));
					}
				}
				alt_up_sd_card_write(fileHandle, ' ');
				loopCount--;
			}

			printf("writing high scores, in main loop, i = %d\n", i);
			//write scores that are currently saved as high scores back to SD card in correct position.
			for(j = 0; j < NUMINITIALS; j++){
				alt_up_sd_card_write(fileHandle, gameScores->highScoreBoardInits[i][j]);
			}
			alt_up_sd_card_write(fileHandle, ' ');
			firstDigitFound = 0;
			for(j = 0; j < MAXSCOREDIGITS; j++){
				digit = ((int)(gameScores->highScoreBoard[i] / pow(10,(MAXSCOREDIGITS - 1 - j)))%10);
				if(digit != 0 && firstDigitFound == 0){
					firstDigitFound = 1;
				}
				if(firstDigitFound == 1){
					alt_up_sd_card_write(fileHandle, (digit + '0'));
				}
			}
			alt_up_sd_card_write(fileHandle, ' ');
		}
	}

	closeFile(fileHandle);

	getHighScoreBoard(gameScores); //reload new written data back into game memory.

	return;
}

void updateCurrentPlayerScore(struct scores * gameScores, int deltaScore) {
	gameScores->currentPlayerScore += deltaScore;
	return;
}

int getCurrentPlayerScore(struct scores * gameScores) {
	return gameScores->currentPlayerScore;
}

void setCurrentPlayerLives(struct scores * gameScores, int newNumLives){
	gameScores->currentPlayerLives = newNumLives;
}

int getCurrentPlayerLives(struct scores * gameScores) {
	return gameScores->currentPlayerLives;
}

void drawScore(struct scores * gameScores){
	char scoreValues[15];// = malloc(sizeof(char) * NUMINITIALS); // scores wont exceed 10, and names are 3
	int i,j;
	int digit;
	int firstNonZeroDig;
	int xPos = 35;
	int ypos = 25;

	alt_up_char_buffer_string(char_buffer, "**HIGH SCORES**" , xPos, ypos - 2);

	for(i = 0; i < NUMSCORES; i++) {
		scoreValues[0] = gameScores->highScoreBoardInits[i][0];
		scoreValues[1] = gameScores->highScoreBoardInits[i][1];
		scoreValues[2] = gameScores->highScoreBoardInits[i][2];

		scoreValues[3] = ' ';

		firstNonZeroDig = 0;
		for(j = 0; j < MAXSCOREDIGITS; j++){
			digit = ((int)(gameScores->highScoreBoard[i] / pow(10,(MAXSCOREDIGITS - 1 - j))) % 10);
			//printf("digit: %d\n", digit);
			if(digit == 0 && firstNonZeroDig == 0){
				scoreValues[4+j] = ' ';
			} else {
				firstNonZeroDig = 1;
				scoreValues[4+j] = (digit + '0');
			}
		}
		scoreValues[4+MAXSCOREDIGITS] = '\0';

		alt_up_char_buffer_string(char_buffer, scoreValues , xPos, ypos + i);
	}
}

void clearScore(void) {
	alt_up_char_buffer_clear(char_buffer);
}

//draws current lives and playerScore
void drawInGameInfo(struct scores * gameScores) {
	char livesLeft[2] = {' ', '\0'};
	livesLeft[0] = (getCurrentPlayerLives(gameScores) + '0');
	int xPos = 1;
	int yPos = 1;
	int i;
	int digit = 0;
	int firstDigitFound = 0;
	int currentScore = getCurrentPlayerScore(gameScores);
	char currentCharScore[MAXSCOREDIGITS + 1];
	currentCharScore[MAXSCOREDIGITS] = '\0'; //end of string

	//populate currentCharScore;
	for(i = 0; i < MAXSCOREDIGITS; i++) {
		digit = ((int)(currentScore / pow(10,(MAXSCOREDIGITS - 1 - i)))%10);
		currentCharScore[i] = '0';
		if(digit != 0 && firstDigitFound == 0) {
			firstDigitFound = 1;
		}
		if(firstDigitFound == 1 || i == (MAXSCOREDIGITS - 1)) {
			currentCharScore[i] = (digit + '0');
		}
	}

	//print info to screen
	if(livesLeft[0] == '1') {
		alt_up_char_buffer_string(char_buffer, "Lives: YOLO" , xPos, yPos);

	} else {
		alt_up_char_buffer_string(char_buffer, "Lives: " , xPos, yPos);
		alt_up_char_buffer_string(char_buffer, livesLeft , xPos + 7, yPos);
	}
	alt_up_char_buffer_string(char_buffer, "Score: " , xPos, yPos+1);
	alt_up_char_buffer_string(char_buffer, currentCharScore , xPos + 7, yPos+1);

	return;
}
