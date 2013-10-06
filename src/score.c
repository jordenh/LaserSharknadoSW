/* score.c */

#include "score.h"

char * scoreFileName = "scores.txt";
struct scores * gameScores;

//set all gameScores values to initial values - read all high score board info from SD card.
void initScoreBoard(void) {
	alt_up_char_buffer_clear(char_buffer); // clear screen
	gameScores = malloc(sizeof(struct scores));
	printf("gameScoresTemp is at: %x\n", (unsigned int)gameScores);

	gameScores->currentPlayerScore = 333; // TBD - set to 0 - testing code by setting to other values.
	gameScores->currentPlayerLives = INITIALLIVES;
	readHighScoreBoardFromSD();
}

//set high score values to defaults - used if reading the SD card fails.s
void defaultHighScoreBoard(void) {
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
//function updates all gameScores' variables so that reading them have the most up to date values
int readHighScoreBoardFromSD(void) {
	gameScores->currentScoreBoardCorrupt = 0;

	short int fileHandle = openFile(scoreFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", scoreFileName);
		defaultHighScoreBoard();
		gameScores->currentScoreBoardCorrupt = 1;
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
			defaultHighScoreBoard();
			gameScores->currentScoreBoardCorrupt = 1;
			return -1;
		}

		//clear score buffer - which is used in order to convert unknown number of chars into an int.
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
			defaultHighScoreBoard();
			gameScores->currentScoreBoardCorrupt = 1;
			return -1;
		}

		//use scoreBuffer values (stored in char values) into a meaningful integer, currentScore.
		currentScore = 0;
		for(k = 0; k < numDigits; k++) {
			currentScore += (scoreBuffer[k] - '0') * pow(10,(numDigits - 1 - k));
		}
		gameScores->highScoreBoard[i] = currentScore;
	}

	closeFile(fileHandle);
	return 0;
}

//function writes to the SD card, and places current players score into the high scores if
//it exceeds any of the current high scores.
//function returns if the highScore file doesnt open in memory, or if the currentScoreBoard is corrupt.
void updateHighScoreBoard(void) {
	int playerScore = getCurrentPlayerScore();
	unsigned short i,j;
	short scoreReplaceIndex = -1;
	unsigned short firstDigitFound = 0;
	unsigned short digit;
	short int fileHandle = openFile(scoreFileName);
	if (fileHandle == -1 || gameScores->currentScoreBoardCorrupt == 1) {
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

				char * newInitials = malloc(3*sizeof(char));
				*newInitials = '-';
				*(newInitials + 1) = '-';
				*(newInitials + 2) = '-';
				obtainUserInitials(newInitials);
				for(j = 0; j < NUMINITIALS; j++){
					alt_up_sd_card_write(fileHandle, newInitials[j]);
				}
				free(newInitials);

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

	//reload new written data back into game memory.
	readHighScoreBoardFromSD();

	return;
}

//get user input for initials if they are on the high score board
void obtainUserInitials(char * initials){

	int numInitials = 3;
	unsigned int charChoice = 0;
	int i;
	char keyInput;
	short int edgeDetect0 = 0;
	short int edgeDetect1 = 0;
	short int edgeDetect2 = 0;
	keyInput = IORD_8DIRECT(KEYS_BASE, 0);
	char key0 = keyInput & 0x01;
	char key1 = keyInput & 0x02;
	char key2 = keyInput & 0x04;
	int xPos = 20;
	int yPos = 20;

	//0x41 + x(0-25) is alphabet in ascii.
	alt_up_char_buffer_string(char_buffer, "NEW HIGH SCORE!!!", xPos, yPos-3);
	alt_up_char_buffer_string(char_buffer, "use key1 to go up, key2 to go down,", xPos, yPos-2);
	alt_up_char_buffer_string(char_buffer, "and key0 to lock in choice:", xPos, yPos-1);
	for(i = 0; i < numInitials; i++) {
		charChoice = 0;
		while(1) {
			keyInput = IORD_8DIRECT(KEYS_BASE, 0);
			key0 = keyInput & 0x01;
			key1 = keyInput & 0x02;
			key2 = keyInput & 0x04;

			alt_up_char_buffer_string(char_buffer, initials, xPos, yPos);
			if (!key0 && (edgeDetect0 == 0)) {
				edgeDetect0 = 1;
			} else if (key0 && (edgeDetect0 == 1)) {
				edgeDetect0 = 0;
				break;
			}
			if (!key1 && (edgeDetect1 == 0)) {
				edgeDetect1 = 1;
			} else if (key1 && (edgeDetect1 == 1)) {
				edgeDetect1 = 0;
				if(charChoice < 25){
					charChoice++;
				} else {
					charChoice = 0;
				}
			}
			if (!key2 && (edgeDetect2 == 0)) {
				edgeDetect2 = 1;
			} else if (key2 && (edgeDetect2 == 1)) {
				edgeDetect2 = 0;
				if(charChoice > 0) {
					charChoice--;
				} else {
					charChoice = 25;
				}

			}
			initials[i] = 0x41 + charChoice;
		}
	}

	alt_up_char_buffer_clear(char_buffer); // clear screen

	printf("initials read as: %c%c%c", initials[0], initials[1], initials[2]);

	return;
}


//increment player score by deltaScore
void updateCurrentPlayerScore(int deltaScore) {
	gameScores->currentPlayerScore += deltaScore;
	return;
}

//accessor: get score
int getCurrentPlayerScore() {
	return gameScores->currentPlayerScore;
}

//mutator: set lives to newNumLives
void setCurrentPlayerLives(int newNumLives){
	gameScores->currentPlayerLives = newNumLives;
}

//accessor: get lives
int getCurrentPlayerLives(void) {
	return gameScores->currentPlayerLives;
}

//purpose: overlay current scoreboard values onto the screen.
void drawScore(void){
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
void drawInGameInfo(void) {
	char livesLeft[2] = {' ', '\0'};
	livesLeft[0] = (getCurrentPlayerLives() + '0');
	int xPos = 1;
	int yPos = 1;
	int i;
	int digit = 0;
	int firstDigitFound = 0;
	int currentScore = getCurrentPlayerScore();
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

	} else if(livesLeft[0] == '0') {
		alt_up_char_buffer_string(char_buffer, "Lives: DEAD" , xPos, yPos);
	} else {
		alt_up_char_buffer_string(char_buffer, "Lives: " , xPos, yPos);
		alt_up_char_buffer_string(char_buffer, livesLeft , xPos + 7, yPos);
	}
	alt_up_char_buffer_string(char_buffer, "Score: " , xPos, yPos+1);
	alt_up_char_buffer_string(char_buffer, currentCharScore , xPos + 7, yPos+1);

	return;
}
