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

#define switches (volatile char *) 0x1001060
#define leds (char *) 0x1001070

int init() {
	if(openSdCard() == -1 ) {
		printf("Error: Failed to open sd card\n");
		return -1;
	} else {
		printf("Opened SD card\n");
	}

	if(init_lcd() == -1 ) {
		printf("Error: could not open character LCD device\n");
		return -1;
	} else {
		printf("LCD Initialized\n");
	}

	initVga();
	setupAudio();
	setupDisplacement();
	parseBmps();
	initBullets();
	initPlayer();

	setHardwareTimerPeriod(CLOCK_FREQ/30);
	return 0;
}

int main() {
	int count = 0;
	short int debounce = 0;
	char keyInput;
	char key2;
	int i;
	int position = 0;

	if(init() == -1)
		return -1;

	Shark *shark1 = malloc(sizeof(Shark));

	drawShark(shark1, 100, 0);

	startHardwareTimer();

	// main game loop;
	while(1) {
		if (hasHardwareTimerExpired() == 1) {
			startHardwareTimer();
			count++;

			if (count%30 == 0)
				printf("%i: Timer has expired\n", count/30);

			moveShark(shark1, 100, position++);

			keyInput = IORD_8DIRECT(KEYS_BASE, 0);
			key2 = (keyInput/4) & 0x0001;

			if ((key2 == 0x01) && debounce == 0) {
				debounce = 1;
			} else if ((key2 == 0x00) && debounce == 1){
				debounce = 0;

				int index = 0;
				while (index < NUM_BULLETS)
				{
					if (bulletArray[index].status == NOTACTIVE)
					{
						bulletArray[index].x = player->x + PLAYER_WIDTH + 1;
						bulletArray[index].y = player->y + 0.5*PLAYER_HEIGHT;
						bulletArray[index].status = PLAYERBULLET;
						drawBullet(&bulletArray[index]);
						break;
					}
					index++;
				}

				playLaser();
			}

			if (count%2 == 0) {
				for (i = 0; i < NUM_BULLETS; i++) {
					if (bulletArray[i].status != NOTACTIVE) {
						moveRight(&bulletArray[i]);
					}
				}
			}

			if (keyInput == 0x02) {
				moveUp();
			} else if (keyInput == 0x04) {
				moveDown();
			} else {
				drawPlayer();
			}

			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
			while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

			eraseShark(shark1);
			erasePlayer();
		}
	}

	freeBmps();
	return 0;
}
