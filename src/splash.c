#include "splash.h"

void drawSplashScreen() {
	int loadXPosition, loadYPosition;

	loadXPosition = (SCREEN_WIDTH - loadBmp->infoheader.width)/2;
	loadYPosition = SCREEN_HEIGHT - 8*loadBmp->infoheader.height;

	clearScreen();
	drawBmp(splashBmp, 0, 0);
	drawBmp(loadBmp, loadXPosition, loadYPosition);

	swapBuffers();
}

void swapSplashScreenText() {
	int pressXPosition, pressYPosition;

	pressXPosition = (SCREEN_WIDTH - pressBmp->infoheader.width)/2;
	pressYPosition = SCREEN_HEIGHT - 8*pressBmp->infoheader.height;

	drawBmp(splashBmp, 0, 0);
	drawBmp(pressBmp, pressXPosition, pressYPosition);

	swapBuffers();
}

void clearSplashScreen() {
	clearScreen();

	//freeSplash();
}
