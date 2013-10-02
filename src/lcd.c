#include "lcd.h"

int init_lcd() {
	// open the Character LCD port
	char_lcd_dev = alt_up_character_lcd_open_dev("/dev/character_lcd_0");

	if(char_lcd_dev == NULL) {
		return -1;
	}

	/* Initialize the character display */
	alt_up_character_lcd_init (char_lcd_dev);

	return 0;
}
