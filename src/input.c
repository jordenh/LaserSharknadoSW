
#include "input.h"

#define switches (volatile char *) 0x1001060
#define leds (char *) 0x1001070
#define keys (volatile char *) 0x1001080
#define atariInput (volatile char *) 0x10010b0

void handleKeyInput(void){
	static char keyInput;
	static short int edgeDetect0 = 0;
	static short int edgeDetect1 = 0;
	static short int edgeDetect2 = 0;
	static short int edgeDetect3 = 0;

	keyInput = IORD_8DIRECT(KEYS_BASE, 0);
	char key0 = keyInput & 0x01;
	char key1 = keyInput & 0x02;
	char key2 = keyInput & 0x04;
	char key3 = keyInput & 0x08;

	//functionality for keys being held
	if(key1) {
		moveDownPlayer();
	} else if (key2) {
		moveUpPlayer();
	} else {
		drawPlayer();
	}

	//functionality for keys being pressed.
	if (!key0 && (edgeDetect0 == 0)) {
		edgeDetect0 = 1;
	} else if (key0 && (edgeDetect0 == 1)) {
		edgeDetect0 = 0;
		createBullet(PLAYERBULLET);
	}

	if (!key1 && (edgeDetect1 == 0)) {
		edgeDetect1 = 1;
	} else if (key1 && (edgeDetect1 == 1)) {
		edgeDetect1 = 0;
		/*if(getCurrentPlayerLives() != 0){
			setCurrentPlayerLives(getCurrentPlayerLives() - 1);
		} else {
			//ONLY FOR TESTING btw.
			setCurrentPlayerLives(getCurrentPlayerLives() + 1);
		}
		playPlayerDeath();*/
	}

	if (!key2 && (edgeDetect2 == 0)) {
		edgeDetect2 = 1;
	} else if (key2 && (edgeDetect2 == 1)) {
		edgeDetect2 = 0;
		//updateHighScoreBoard();
		//playTheme();
	}

	if (!key3 && (edgeDetect3 == 0)) {
		edgeDetect3 = 1;
	} else if (key3 && (edgeDetect3 == 1)) {
		edgeDetect3 = 0;
		//updateCurrentPlayerScore(250);
		//playSharkDeath();
	}
}

void handleSwitchInput(void){
	static char SWInput;
	static short int scoresShown = 0;
	SWInput = IORD_8DIRECT(switches, 0);

	if ((SWInput & 0x80) != 0) {
		if(scoresShown == 0){
			readHighScoreBoardFromSD();
			drawScore();
		}
		scoresShown = 1;
	} else {
		if(scoresShown == 1){
			clearScore();
		}
		scoresShown = 0;
	}

}

void handleAtariInput(void){
	static char atariButtons;
	static char atariUp;
	static char atariDown;
	static char atariFire;
	static short int edgeDetect = 0;

	atariButtons = (IORD_8DIRECT(atariInput, 0) & 0x0F);
	atariFire = atariButtons & 0x08;
	atariUp = atariButtons & 0x02;
	atariDown = atariButtons & 0x04;
	IOWR_16DIRECT(leds, 0, atariButtons);

	if ((atariFire == 0x00) && (edgeDetect == 0)) {
		edgeDetect = 1;
	} else if ((atariFire != 0x00) && (edgeDetect == 1)) {
		edgeDetect = 0;
		createBullet(PLAYERBULLET);
	}

	//movements - no edge detect
	if (atariUp != 0x00) {
		moveUpPlayer();
	} else if (atariDown != 0x00) {
		moveDownPlayer();
	} else {
		keepPlayerStationary();
	}
}

int startGame() {
	char atariButtons;
	char atariFire;

	atariButtons = (IORD_8DIRECT(atariInput, 0) & 0x0F);
	atariFire = atariButtons & 0x08;
	
	if (atariFire == 1) {
		return 1;
	} else {
		return 0;
	}
}
