#include <stdio.h>
#include <stdlib.h>
#include "audio.h"
#include "timer.h"
#include "sd_card.h"
#include "vga.h"
#include "bmp.h"
#include "shark.h"
#include "displacement.h"
#include "collision.h"
#include "io.h"
#include "system.h"
#include "altera_nios2_qsys_irq.h"
#include "sys/alt_irq.h"
#include "score.h"
#include "input.h"
#include "splash.h"
#include "gameEnd.h"
#include "nado.h"

#define SHARKS_PER_LVL 4
#define SMART 1
#define DUMB 0

void sharkSpawner(void);
static int lvlSharkCount = 0;

int init(void) {
	if (openSdCard() == -1) {
		printf("Error: Failed to open sd card\n");
		return -1;
	} else {
		printf("Opened SD card\n");
	}

	initVga();
	parseBmps();
	drawSplashScreen();
	setupAudio();
	setupDisplacement();
	initScoreBoard();

	initBullets();
	initPlayer();
	initSharks();
	initCollision();

	initHardwareTimer();

	swapSplashScreenText();

	return 0;
}

int main() {
	int count = 0;
	int spawnLvl = 0;
	int nadoCounter = 0;
	short int displaySplashScreen = 1;

	if (init() == -1)
		return -1;

	drawAllSharks();
	startHardwareTimer();

	playTheme();

	// main game loop;
	while(1) {
		if (hasHardwareTimerExpired() == 1) {
			startHardwareTimer();

			if (displaySplashScreen) {
				if(gameStart()) {
					clearSplashScreen();
					stopTheme();
					displaySplashScreen = 0;	
					drawInGameInfo();
					nadoCounter = 0;
				}
			} else {
				if (nadoCounter < 2) {
					drawInitialNado(nadoCounter);
					nadoCounter++;
				}
				count++;

				if (sharkCount == 0) {
					spawnLvl = 1;
				}
				if (spawnLvl == 1 && count%15 == 0) {
					sharkSpawner();
					if (sharkCount == 4){
						spawnLvl = 0;
					}
				}

				moveAllSharks();
				drawAllSharks();

				handleKeyInput();
				handleSwitchInput();
				handleAtariInput();

				moveAllBullets();

				cleanupDeadSharks();

				doSharkBulletCollision();
				doPlayerBulletCollision();
				doNadoBulletCollision();
				if(getCurrentPlayerLives() == 0) {
					gameEndSequence();
					displaySplashScreen = 1;
					lvlSharkCount = 0;
					continue;
				}

				drawAllBullets();

				swapBuffers();

				cleanupDeadSharks();
				eraseAllSharks();
				eraseAllBullets();
				erasePlayer();

			}
		}
	}

	freeBmps();
	return 0;
}

void sharkSpawner(void){
	int shotSpeed;
	int sharkX, sharkY;
	int sharkEntrance;
	static int sharkEntranceDefault = 128;

	if (lvlSharkCount == SHARKS_PER_LVL*8) {
		lvlSharkCount = 0;
	}

	sharkEntrance = ((lvlSharkCount%4) + 1) * 49;
	shotSpeed = 20*rand();
	sharkX = (((lvlSharkCount%4) + 1) * SHARK_WIDTH) + PLAYER_WIDTH;
	sharkY = ((lvlSharkCount%4) * PLAYER_HEIGHT) + PLAYER_HEIGHT ;

	if(lvlSharkCount < SHARKS_PER_LVL*1){
		createShark(shotSpeed, sharkX, 0, (Displacement *)&doNotMove, BOTTOMWALL, DUMB, sharkEntrance);
	} else if(lvlSharkCount < SHARKS_PER_LVL*2){
		createShark(shotSpeed, sharkX, 0, (Displacement *)&verticalDisplacementFunctionDown, TOPWALL, DUMB, sharkEntranceDefault);
	} else if(lvlSharkCount < SHARKS_PER_LVL*3){
		createShark(shotSpeed, sharkX, 0, (Displacement *)&circularDisplacementFunction, BOTTOMWALL, DUMB, sharkEntranceDefault);
	} else if(lvlSharkCount < SHARKS_PER_LVL*4){
		createShark(shotSpeed, sharkX, 0, (Displacement *)&arcDisplacementFunction, TOPWALL, DUMB, sharkEntranceDefault);
	} else if(lvlSharkCount < SHARKS_PER_LVL*5){
		createShark(shotSpeed, 0, sharkY, (Displacement *)&doNotMove, RIGHTWALL, SMART, sharkEntrance);
	} else if(lvlSharkCount < SHARKS_PER_LVL*6){
		createShark(shotSpeed, 0, sharkY, (Displacement *)&circularDisplacementFunction, RIGHTWALL, SMART, sharkEntranceDefault);
	} else if(lvlSharkCount < SHARKS_PER_LVL*7){
		createShark(shotSpeed, 0, sharkY, (Displacement *)&arcDisplacementFunction, RIGHTWALL, SMART, sharkEntranceDefault);
	} else if(lvlSharkCount < SHARKS_PER_LVL*8){
		createShark(shotSpeed, 0, sharkY, (Displacement *)&verticalDisplacementFunctionDown, RIGHTWALL, SMART, sharkEntranceDefault);
	}
	lvlSharkCount++;

	return;
}





