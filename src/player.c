#include "player.h"

void initPlayer() {
	player.x = 10;
	player.y = 90;
	drawPlayer(player);
}

void drawPlayer() {
	drawBox(player.x, player.y, player.x + PLAYER_WIDTH, player.y + PLAYER_HEIGHT, 0xFFFF);
}

void erasePlayer() {
	drawBox(player.x, player.y, player.x + PLAYER_WIDTH, player.y + PLAYER_HEIGHT, 0x0000);
}

void moveUpPlayer() {
	if (player.y > 0) {
		erasePlayer(player);
		player.y = player.y - 1;
		drawPlayer(player);
	}
}
void moveDownPlayer()
{
	if (player.y < SCREEN_HEIGHT - PLAYER_HEIGHT) {
		erasePlayer(player);
		player.y = player.y + 1;
		drawPlayer(player);
	}
}
