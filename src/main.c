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
#include "altera_up_avalon_character_lcd.h"
#include "audio.h"
#include "timer.h"
#include "sd_card.h"
#include "vga.h"
#include "io.h"
#include "system.h"
#include "altera_nios2_qsys_irq.h"
#include "sys/alt_irq.h"
#include "displacement.h"
#include "collision.h"

#define switches (volatile char *) 0x1001060
#define leds (char *) 0x1001070
#define keys (volatile char *) 0x1001080

int main()
{
	// Mandatory setup code for peripherals
	// SD card must be opened before audio is setup
	openSdCard();
	setupAudio();
	setupDisplacement();
	setupVga();

	//***

	alt_up_character_lcd_dev * char_lcd_dev;
	// open the Character LCD port
	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");//("/dev/Char_LCD_16x2");
	if ( char_lcd_dev == NULL)
		printf ("Error: could not open character LCD device\n");
	else
		printf ("Opened character LCD device\n");
	/* Initialize the character display */
	alt_up_character_lcd_init (char_lcd_dev);
	/* Write "Welcome to" in the first row */
	alt_up_character_lcd_string(char_lcd_dev, "Welcome to");
	/* Write "the DE2 board" in the second row */
	char second_row[] = "the DE2 board\0";
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
	alt_up_character_lcd_string(char_lcd_dev, second_row);

	printf("hello, world!\n");

	// initialise the player
	Player *player;
	player->x = 10;
	player->y = 90;
	drawPlayer(player);

	// initialise array of bullets
	Bullet *bulletArray;
	bulletArray = malloc(NUM_BULLETS*sizeof(Bullet));

	int i = 0;
	for (i = 0; i < NUM_BULLETS; i++)
	{
		bulletArray[i].status = NOTACTIVE;
	}

	int count = 0;
	short int debounce = 0;
	char keyInput;
	char key2;

	setHardwareTimerPeriod(CLOCK_FREQ/30);
	startHardwareTimer();

	// main game loop;
	while(1)
	{
		if (hasHardwareTimerExpired() == 1)
		{
			startHardwareTimer();
			count++;
			if (count%30 == 0)
			{
				printf("%i: Timer has expired\n", count/30);
			}

			keyInput = IORD_8DIRECT(keys, 0);
			key2 = keyInput/4;
			key2 = keyInput & 0x0001;

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
				i = 0;
				for (i = 0; i < NUM_BULLETS; i++) {
					if (bulletArray[i].status != NOTACTIVE) {
						moveRight(&bulletArray[i]);
					}
				}
			}

			if (keyInput == 0x02) {
				printf("Key 0");
				moveUp(player);
			}
			if (keyInput == 0x04) {
				printf("Key 1");
				moveDown(player);
			}
		}
	}

	return 0;
}
