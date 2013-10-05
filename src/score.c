/* score.c */

#include "score.h"

char * scoreFileName = "scores.txt";

void initScoreBoard(struct scores * gameScores) {
	gameScores->currentPlayerScore = 0;
	gameScores->currentPlayerLives = INITIALLIVES;
	int i;
	/*for (i = 0; i < NUMSCORES; i++){
		//gameScores.highScoreBoardInits[i] = malloc(sizeof(char) * 3);
		//printf("size of gameScore: %x, at %x",(sizeof(char) * 3), (unsigned int)gameScores.highScoreBoardInits[i]);
		if(gameScores->highScoreBoardInits[i] == NULL) {
			printf("Error in mallocing scoreboard Initials space. \n");
		}
	}*/
	if(getHighScoreBoard(gameScores) != -1) {
		return;
	} else {
		defaultHighScoreBoard(gameScores);
		return;
	}

}

void defaultHighScoreBoard(struct scores * gameScores) {
	int i;
	for(i = 0; i < NUMSCORES; i++){
		gameScores->highScoreBoard[i] = 1;
		gameScores->highScoreBoardInits[i] = "XXX";
	}
	return;
}

//returns -1 on error in reading scoreboard, returns 0 on pass.
int getHighScoreBoard(struct scores * gameScores) {
	short int fileHandle = openFile(scoreFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", scoreFileName);
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
			return -1;
		}

		currentScore = 0;
		//use scoreBuffer values (stored in char values) into a meaningful integer, currentScore.
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
	unsigned short i,j,k;
	unsigned short scoreReplaceIndex;
	unsigned short firstDigitFound;
	unsigned short digit;
	short int fileHandle = openFile(scoreFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", scoreFileName);
		return;
	}

	for(i = 0; i < NUMSCORES; i++) {
		if(playerScore > gameScores->highScoreBoard[i]) {
			scoreReplaceIndex = i;
			break; // current index needs to be replaced by
		}
	}
	//if(i != NUMSCORE){
	// THIS SECTION CURRENTLY WRITES EXACTLY WHAT IS IN THE gameScores back into the file!
	//needs to be modded to incorperate new score value!
	if(1){
		for(i = 0; i < NUMSCORES; i++) {
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
	return;

	//if i == NUMSCORES{
	//write 0 - index as was previously, then write current high score, then write index-1 downto 1.
	//} else
	//return, since file is as updated as need be.
}

void updateCurrentPlayerScore(int deltaScore, struct scores * gameScores) {
	gameScores->currentPlayerScore += deltaScore;
	return;
}

int getCurrentPlayerScore(struct scores * gameScores) {
	return gameScores->currentPlayerScore;
}

void setCurrentPlayerLives(struct scores * gameScores, int newNumLives){
	gameScores->currentPlayer = newNumLives;
}

int getCurrentPlayerLives(struct scores * gameScores) {
	return gameScores->currentPlayer;
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
			printf("digit: %d\n", digit);
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

void clearScore(void){
	alt_up_char_buffer_clear(char_buffer);
}

