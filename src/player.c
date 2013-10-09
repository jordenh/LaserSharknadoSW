#include "player.h"
#include "audio.h"

void initPlayer() {
	player.x = 10;
	player.y = 90;
	player.prevX = 10;
	player.prevY = 90;
}

void drawPlayer() {
	drawBmp(playerBmp, player.x, player.y);
}

void erasePlayer() {
	eraseBmp(playerBmp, player.prevX, player.prevY);
}

void moveUpPlayer() {
	player.prevY = player.y;

	if (player.y > 0)
		player.y = player.y - 2;

	drawBmp(playerBmp, player.x, player.y);
}
void moveDownPlayer()
{
	player.prevY = player.y;

	if (player.y < SCREEN_HEIGHT - PLAYER_HEIGHT)
		player.y = player.y + 2;

	drawBmp(playerBmp, player.x, player.y);
}

void keepPlayerStationary() {
	player.prevY = player.y;
	drawBmp(playerBmp, player.x, player.y);
}

void hitPlayer() {
	int playerLives = getCurrentPlayerLives();
	if (playerLives > 1) {
		setCurrentPlayerLives(playerLives - 1);
	} else {
		killPlayer();
	}
	drawInGameInfo();
}

void killPlayer() {
	playPlayerDeath();
	updateHighScoreBoard();
	// Some game over stuff
}

