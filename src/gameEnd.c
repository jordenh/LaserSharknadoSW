#include "gameEnd.h"

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

	drawSplashScreen();
	swapSplashScreenText();
	//delayUntilUPAndButtonPressed();

	//clearScreen();
}

