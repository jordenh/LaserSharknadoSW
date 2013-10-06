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

#define switches (volatile char *) 0x1001060
#define leds (char *) 0x1001070
#define keys (volatile char *) 0x1001080
#define atariInput (volatile char *) 0x10010b0

int init(struct scores * gameScores) {
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
	initScoreBoard(gameScores);

	parseBmps();
	initBullets();
	initPlayer();

	setHardwareTimerPeriod(CLOCK_FREQ/30);
	return 0;
}

int main() {
	struct scores gameScores;

	if (init(&gameScores) == -1)
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

	for(i = 0; i < NUMSCORES; i++){
				printf("test: %c%c%c\n", gameScores.highScoreBoardInits[i][0],gameScores.highScoreBoardInits[i][1],gameScores.highScoreBoardInits[i][2]);
				printf("score: %d\n", gameScores.highScoreBoard[i]);
			}

	int count = 0;
	short int edgeDetect = 0;
	short int debounce = 0;
	char SWInput;
	char scoreInitials[15];// = malloc(sizeof(char) * NUMINITIALS); // scores wont exceed 10, and names are 3
	scoreInitials[3] = '\0';
	short int scoresShown = 0;
	printf("scoreInitials got addr: %x\n", (unsigned int)scoreInitials);
	char keyInput;
	int position = 0;

	char atariButtons;
	char atariUp;
	char atariDown;
	char atariFire;
	int j,k;

	setHardwareTimerPeriod(CLOCK_FREQ / 30);

	createShark(100, 0, &circularDisplacementFunction);

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

			if ((atariFire == 0x00 || keyInput & 0x01) && (edgeDetect == 0)) {
				edgeDetect = 1;
			} else if ((atariFire != 0x00 || keyInput & 0x01) && (edgeDetect == 1)) {
				edgeDetect = 0;
				createBullet(PLAYERBULLET);
			}

			if (count%2 == 0) {
				moveAllBullets();
			}

			if (atariUp != 0x00) {
				moveUpPlayer();
			} else if (atariDown != 0x00) {
				moveDownPlayer();
			} else {
				drawPlayer();
			}
			if ((SWInput & 0x80) != 0) {
				if(scoresShown == 0){
					for(i = 0; i < NUMSCORES; i++) {
						scoreInitials[0] = gameScores.highScoreBoardInits[i][0];
						scoreInitials[1] = gameScores.highScoreBoardInits[i][1];
						scoreInitials[2] = gameScores.highScoreBoardInits[i][2];

						//for(j = 0; j < )

						alt_up_char_buffer_string(char_buffer,scoreInitials , 40, 30 + i);
					}
				}
				scoresShown = 1;
			} else {
				if(scoresShown == 1){
					alt_up_char_buffer_clear(char_buffer);
				}
				scoresShown = 0;
			}
			if((keyInput & 0x02) != 0x00){
				playPlayerDeath();
			}
			if((keyInput & 0x04) != 0x00){
				playTheme();
			}
			if((keyInput & 0x08) != 0x00){
				playSharkDeath();
			}

			doSharkBulletCollision();

			drawAllBullets();

			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
			while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

			eraseAllSharks();
			eraseAllBullets();
			erasePlayer();
		}
	}

	freeBmps();
	return 0;
}
