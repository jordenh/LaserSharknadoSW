#include "gameEnd.h"

//purpose: This function is used whenever the player dies and first updates the score board with 
//		player score information, then allows the player to view the score screen,
//		then reinitializes the game and displays the splash screen while playing music
void gameEndSequence(void) {
	updateHighScoreBoard();

	clearScreen();

	readHighScoreBoardFromSD();
	drawScore();
	delayUntilUPAndButtonPressed();

	clearScreen();

	initPlayer();
	setCurrentPlayerLives(3);
	setCurrentPlayerScore(0);


	initSharks();
	//DO ALL SHARK CLEARING/INITIALIZATION STUFF
	initBullets();
	//DO ALL BULLET CLEARING/INITIALIZATION STUFF

	drawSplashAfterDeath();

	playTheme();
}

