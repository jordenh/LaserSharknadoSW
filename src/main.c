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
	int nadoCounter = 0;
	short int displaySplashScreen = 1;

	if (init() == -1)
		return -1;


	//createShark(22, 100, 0, (Displacement *)&doNotMove);
	//createShark(45, 200, 200, (Displacement *)&doNotMove);
	//createShark(22, 100, 0, (Displacement *)&arcDisplacementFunction, TOPWALL);
	//createShark(45, 200, 200, (Displacement *)&arcDisplacementFunction, RIGHTWALL);

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
					createShark(22, 100, 0, (Displacement *)&arcDisplacementFunction, BOTTOMWALL, 0, 30);
					createShark(45, 200, 100, (Displacement *)&verticalDisplacementFunctionUp, RIGHTWALL, 1, 100);
				}
			} else {
				if (nadoCounter < 2) {
					drawInitialNado(nadoCounter);
					nadoCounter++;
				}
				count++;

				moveAllSharks();
				drawAllSharks();
				//drawInGameInfo(); // TBD: in actual game loop, only call this function when an event happens (like score inc/dec, or lives inc/dec)

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
