#include "player.h"

void initPlayer() {
	player->x = 10;
	player->y = 90;
	drawPlayer(player);
}

void drawPlayer(Player *player) {
	drawBox(player->x, player->y, player->x + PLAYER_WIDTH, player->y + PLAYER_HEIGHT, 0xFFFF);
}

void erasePlayer(Player *player) {
	drawBox(player->x, player->y, player->x + PLAYER_WIDTH, player->y + PLAYER_HEIGHT, 0x0000);
}

void moveUp(Player *player) {
	if (player->y > 0) {
		erasePlayer(player);
		player->y = player->y - 1;
		drawPlayer(player);
	}
}
void moveDown(Player *player)
{
	if (player->y < SCREEN_HEIGHT - PLAYER_HEIGHT) {
		erasePlayer(player);
		player->y = player->y + 1;
		drawPlayer(player);
	}
}
