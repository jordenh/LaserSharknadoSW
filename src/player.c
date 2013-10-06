#include "player.h"
#include "audio.h"

void initPlayer() {
	player.x = 10;
	player.y = 90;
	player.prevX = 0;
	player.prevY = 0;
	drawBmp(playerBmp, player.x, player.y);
}

void drawPlayer() {
	drawBmp(playerBmp, player.x, player.y);
}

void erasePlayer() {
	eraseBmp(playerBmp, player.prevX, player.prevY);
}

void moveUpPlayer() {
	if (player.y > 0) {
		player.prevX = player.x;
		player.prevY = player.y;

		player.y = player.y - 1;
		drawBmp(playerBmp, player.x, player.y);
	}
}
void moveDownPlayer()
{
	if (player.y < SCREEN_HEIGHT - PLAYER_HEIGHT) {
		player.prevX = player.x;
		player.prevY = player.y;

		player.y = player.y + 1;
		drawBmp(playerBmp, player.x, player.y);
	}
}

void killPlayer() {
	playPlayerDeath();
	// Some game over stuff
}
