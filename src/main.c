#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
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

#define switches (volatile char *) 0x1001060
#define leds (char *) 0x1001070
#define keys (volatile char *) 0x1001080
#define atariInput (volatile char *) 0x10010b0

int init(void) {
	if (openSdCard() == -1) {
		printf("Error: Failed to open sd card\n");
		return -1;
	} else {
		printf("Opened SD card\n");
	}

	if (init_lcd() == -1) {
		printf("Error: could not open character LCD device\n");
		return -1;
	} else {
		printf("LCD Initialized\n");
	}

	initVga();
	setupAudio();
	setupDisplacement();
	initScoreBoard();
	updateHighScoreBoard(); // only here for testing - this should actually be called each time the player gets hit.

	parseBmps();
	initBullets();
	initPlayer();

	setHardwareTimerPeriod(CLOCK_FREQ/30);
	return 0;
}

int main() {
	if (init() == -1)
		return -1;

	// Initialize the player
	Player player;
	player.x = 10;
	player.y = 90;
	drawPlayer(&player);

	// initialize array of bullets
	Bullet bulletArray[NUM_BULLETS];
	//bulletArray = malloc(NUM_BULLETS * sizeof(Bullet));
	//printf("size of bulletArray %x, at %x", (NUM_BULLETS * sizeof(Bullet)), (unsigned int)bulletArray);

	int i = 0;
	for (i = 0; i < NUM_BULLETS; i++) {
		bulletArray[i].status = NOTACTIVE;
	}

	int count = 0;
	short int edgeDetect = 0;
	short int debounce = 0;
	char SWInput;
	short int scoresShown = 0;

	char keyInput;
	int position = 0;

	char atariButtons;
	char atariUp;
	char atariDown;
	char atariFire;
	int j,k;

	setHardwareTimerPeriod(CLOCK_FREQ / 30);

	createShark(100, 0, &doNotMove);

	drawAllSharks();
	startHardwareTimer();

	// main game loop;
	while(1) {
		if (hasHardwareTimerExpired() == 1) {
			startHardwareTimer();
			count++;

			keyInput = IORD_8DIRECT(KEYS_BASE, 0);
			SWInput = IORD_8DIRECT(switches, 0);
			atariButtons = (IORD_8DIRECT(atariInput, 0) & 0x0F);
			atariFire = atariButtons & 0x08;
			atariUp = atariButtons & 0x02;
			atariDown = atariButtons & 0x04;
			IOWR_16DIRECT(leds, 0, atariButtons);

			if (count%30 == 0)
				printf("%i: Timer has expired\n", count/30);

			moveAllSharks();
			drawAllSharks();

			handleKeyInput();
			handleSwitchInput();
			handleAtariInput();

			/*if ((atariFire == 0x00 || keyInput & 0x01) && (edgeDetect == 0)) {
				edgeDetect = 1;
			} else if ((atariFire != 0x00 || keyInput & 0x01) && (edgeDetect == 1)) {
				edgeDetect = 0;
				createBullet(PLAYERBULLET);
			}*/

			if (count%2 == 0) {
				moveAllBullets();
			}

			/*//movements
			if (atariUp != 0x00) {
				moveUpPlayer();
			} else if (atariDown != 0x00) {
				moveDownPlayer();
			} else {
				drawPlayer();
			}
			//score screen
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
			}*/
			drawInGameInfo(); // TBD: in actual game loop, only call this function when an event happens (like score inc/dec, or lives inc/dec)
			/*//random sounds for testing
			if((keyInput & 0x02) != 0x00){
				if(getCurrentPlayerLives() != 0){
					setCurrentPlayerLives(getCurrentPlayerLives() - 1);
				} else {
					//ONLY FOR TESTING btw.
					setCurrentPlayerLives(getCurrentPlayerLives() + 1);
				}
				playPlayerDeath();
			}
			if((keyInput & 0x04) != 0x00){
				updateHighScoreBoard();
				playTheme();
			}
			if((keyInput & 0x08) != 0x00){
				updateCurrentPlayerScore(250);
				playSharkDeath();
			}*/


			if (count % 2 == 0) doSharkBulletCollision();

			drawAllBullets();

			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
			while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
			cleanupDeadSharks();
			eraseAllSharks();
			eraseAllBullets();
			erasePlayer();
		}
	}

	freeBmps();
	return 0;
}
