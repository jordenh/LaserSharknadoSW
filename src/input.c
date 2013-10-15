#include "input.h"


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
	/* if(key1) {
		moveDownPlayer();
	} else if (key2) {
		moveUpPlayer();
	} else {
		keepPlayerStationary();
	}

	//functionality for keys being pressed.
	if (!key0 && (edgeDetect0 == 0)) {
		edgeDetect0 = 1;
	} else if (key0 && (edgeDetect0 == 1)) {
		edgeDetect0 = 0;
		createBullet(PLAYERBULLET, player.x + PLAYER_WIDTH - 1, player.y + 0.5 * PLAYER_HEIGHT - GUN_OFFSET);
	} */

	/*
	if (!key1 && (edgeDetect1 == 0)) {
		edgeDetect1 = 1;
	} else if (key1 && (edgeDetect1 == 1)) {
		edgeDetect1 = 0;
		//playPlayerDeath();
	}

	if (!key2 && (edgeDetect2 == 0)) {
		edgeDetect2 = 1;
	} else if (key2 && (edgeDetect2 == 1)) {
		edgeDetect2 = 0;
		//updateHighScoreBoard();
		//playTheme();
	} //
*/

	static int testXVal = PLAYER_WIDTH*2;
	static int dir = 0;
	if (!key3 && (edgeDetect3 == 0)) {
		edgeDetect3 = 1;
	} else if (key3 && (edgeDetect3 == 1)) {
		edgeDetect3 = 0;
		if(dir == 0){
			createShark(100, testXVal, 0, (Displacement *)&verticalDisplacementFunctionUp, BOTTOMWALL);
			if(testXVal < SCREEN_WIDTH - NADO_WIDTH*2 - SHARK_WIDTH){
				testXVal += 25;
			} else {
				testXVal -= 25;
				dir = 1;
			}
		} else if(dir == 1){
			createShark(100, testXVal, 0, (Displacement *)&verticalDisplacementFunctionDown, TOPWALL);
			if(testXVal > PLAYER_WIDTH*2){
				testXVal -= 25;
			} else {
				testXVal += 25;
				dir = 0;
			}
		}
		//hitPlayer();
	}

}

void handleSwitchInput(void){
	static char SWInput;
	static short int edgeDetect = 0;
	static char prevSwInput = 0x03;
	static short int scoresShown = 0;
	SWInput = IORD_8DIRECT(SWITCHES_BASE, 0);

	if((SWInput & 0x0F) != (prevSwInput & 0x0F)) {
		updateAudioWithVolume(SWInput);
	}
	prevSwInput = SWInput;

	if ((SWInput & 0x80) != 0) {
		if(scoresShown == 0){
			readHighScoreBoardFromSD();
			drawScore();
		}
		scoresShown = 1;
	} else {
		if(scoresShown == 1){
			clearScore();
			drawInGameInfo();
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

	atariButtons = (IORD_8DIRECT(PROCESSORGPIN_BASE, 0) & 0x0F);
	atariFire = atariButtons & 0x08;
	atariUp = atariButtons & 0x02;
	atariDown = atariButtons & 0x04;
	IOWR_16DIRECT(LEDS_BASE, 0, atariButtons);

	if ((atariFire == 0x00) && (edgeDetect == 0)) {
		edgeDetect = 1;
	} else if ((atariFire != 0x00) && (edgeDetect == 1)) {
		edgeDetect = 0;
		createBullet(PLAYERBULLET, player.x + PLAYER_WIDTH - 1, player.y + 0.5 * PLAYER_HEIGHT - GUN_OFFSET);
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

void delayUntilUPAndButtonPressed(void) {
	char atariButtons;
	char atariFire;
	char keys;
	char key4;
	
	while(1) {
		atariButtons = (IORD_8DIRECT(PROCESSORGPIN_BASE, 0) & 0x0F);
		keys = (IORD_8DIRECT(KEYS_BASE, 0) & 0x0F);
		atariFire = atariButtons & 0x8;
		key4 = keys & 0x8;
		if (atariFire || key4) {
			break;
		}
	}
}

int gameStart(void){
	char atariButtons;
	char atariFire;
	char keys;
	char key4;

	atariButtons = (IORD_8DIRECT(PROCESSORGPIN_BASE, 0) & 0x0F);
	keys = (IORD_8DIRECT(KEYS_BASE, 0) & 0x0F);
	atariFire = atariButtons & 0x8;
	key4 = keys & 0x8;

	if (atariFire || key4) {
		return 1;
	} else {
		return 0;
	}
}
