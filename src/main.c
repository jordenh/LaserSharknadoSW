/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "audio.h"
#include "timer.h"
#include "sd_card.h"
#include "vga.h"
#include "bmp.h"
#include "io.h"
#include "system.h"
#include "altera_nios2_qsys_irq.h"
#include "sys/alt_irq.h"
#include "displacement.h"
#include "collision.h"
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
	short int debounce = 0;
	char SWInput;
	char scoreInitials[4];// = malloc(sizeof(char) * NUMINITIALS);
	scoreInitials[3] = '\0';
	printf("scoreInitials got addr: %x\n", scoreInitials);
	char keyInput;
	char key2;
	char atariButtons;
	char atariUp;
	char atariDown;
	char atariFire;
	int j,k;

	setHardwareTimerPeriod(CLOCK_FREQ / 30);
	startHardwareTimer();

	// main game loop;
	while(1) {
		if (hasHardwareTimerExpired() == 1) {
			startHardwareTimer();
			count++;
			if (count % 30 == 0) {
				printf("%i: Timer has expired\n", count / 30);
			}

			SWInput = IORD_8DIRECT(switches, 0);
			keyInput = IORD_8DIRECT(keys, 0);
			key2 = keyInput / 4;
			key2 = keyInput & 0x0001;
			atariButtons = (IORD_8DIRECT(atariInput, 0) & 0x0F);
			atariFire = atariButtons & 0x08;
			atariUp = atariButtons & 0x02;
			atariDown = atariButtons & 0x04;
			IOWR_16DIRECT(leds, 0, atariButtons);

			//if ((key2 == 0x01) && debounce == 0) {
			if ((atariFire == 0x00) && debounce == 0) {
				debounce = 1;
			//} else if ((key2 == 0x00) && debounce == 1){
			} else if ((atariFire != 0x00) && debounce == 1){
				debounce = 0;

				int index = 0;
				while (index < NUM_BULLETS) {
					if (bulletArray[index].status == NOTACTIVE) {
						bulletArray[index].x = player.x + PLAYER_WIDTH + 1;
						bulletArray[index].y = player.y + 0.5 * PLAYER_HEIGHT;
						bulletArray[index].status = PLAYERBULLET;
						drawBullet(&bulletArray[index]);
						break;
					}
					index++;
				}

				playLaser();
			}

			if (count % 2 == 0) {
				i = 0;
				for (i = 0; i < NUM_BULLETS; i++) {
					if (bulletArray[i].status != NOTACTIVE) {
						moveRight(&bulletArray[i]);
					}
				}
			}

			if (atariUp != 0x00) {
				//printf("Key 0");
				moveUp(&player);
			}
			if (atariDown != 0x00) {
				//printf("Key 1");
				moveDown(&player);
			}
			if ((SWInput & 0x80) != 0) {
				for(i = 0; i < NUMSCORES; i++) {
					scoreInitials[0] = gameScores.highScoreBoardInits[i][0];
					scoreInitials[2] = gameScores.highScoreBoardInits[i][2];
					scoreInitials[2] = gameScores.highScoreBoardInits[i][2];
					printf("scoreInitials are: %s", scoreInitials);
					alt_up_char_buffer_string(char_buffer,scoreInitials , 40, 30 + i*5);
				}
			}

		}
	}

	return 0;
}
