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
	//updateHighScoreBoard(); // only here for testing - this should actually be called each time the player gets hit.

	parseBmps();
	initBullets();
	initPlayer();

	setHardwareTimerPeriod(CLOCK_FREQ/30);
	return 0;
}

int main() {
	if (init() == -1)
		return -1;

	setHardwareTimerPeriod(CLOCK_FREQ / 30);

	createShark(100, 0, (Displacement *)&doNotMove);
	createShark(200, 200, (Displacement *)&doNotMove);

	drawAllSharks();
	startHardwareTimer();

	int count = 0;
	printf("%d: count \n", count);
	// main game loop;
	while(1) {
		if (hasHardwareTimerExpired() == 1) {
			startHardwareTimer();
			count++;

			moveAllSharks();
			drawAllSharks();
			drawInGameInfo(); // TBD: in actual game loop, only call this function when an event happens (like score inc/dec, or lives inc/dec)

			handleKeyInput(); // CURRENTLY ONLY FOR KEY3 - need to uncomment in order to have all. laggy.
			handleSwitchInput();
			handleAtariInput();

			moveAllBullets();

			cleanupDeadSharks();

			if (count % 2 == 0) doSharkBulletCollision();
			if (count % 2 == 1) doPlayerBulletCollision();

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
