#include "splash.h"

void drawSplashScreen() {
	drawBmp(splashBmp, 0, 0);

	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
}

void clearSplashScreen() {
	clearScreen();

	freeSplash();
}
