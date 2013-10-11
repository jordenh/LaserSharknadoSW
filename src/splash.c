#include "splash.h"

void drawSplashScreen() {
	int loadXPosition, loadYPosition;

	loadXPosition = (SCREEN_WIDTH - loadBmp->infoheader.width)/2;
	loadYPosition = SCREEN_HEIGHT - 8*loadBmp->infoheader.height;

	drawBmp(splashBmp, 0, 0);
	drawBmp(loadBmp, loadXPosition, loadYPosition);

	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
}

void swapSplashScreenText() {
	int pressXPosition, pressYPosition;

	pressXPosition = (SCREEN_WIDTH - pressBmp->infoheader.width)/2;
	pressYPosition = SCREEN_HEIGHT - 8*pressBmp->infoheader.height;

	drawBmp(splashBmp, 0, 0);
	drawBmp(pressBmp, pressXPosition, pressYPosition);

	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
}

void clearSplashScreen() {
	clearScreen();

	freeSplash();
}
