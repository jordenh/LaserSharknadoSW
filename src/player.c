#include "player.h"

void initPlayer() {
	player.x = 10;
	player.y = 90;
	player.prevX = 0;
	player.prevY = 0;
}

void drawPlayer() {
	drawBmp(playerBmp, player.x, player.y);
}

void erasePlayer() {
	eraseBmp(playerBmp, player.prevX, player.prevY);
}

void moveUpPlayer() {
	if (player.y > 0) {
		player.prevY = player.y;

		player.y = player.y - 2;
	}

	drawBmp(playerBmp, player.x, player.y);
}
void moveDownPlayer()
{
	if (player.y < SCREEN_HEIGHT - PLAYER_HEIGHT) {
		player.prevY = player.y;

		player.y = player.y + 2;
	}

	drawBmp(playerBmp, player.x, player.y);
}

void keepPlayerStationary() {
	player.prevY = player.y;
	drawBmp(playerBmp, player.x, player.y);
}
