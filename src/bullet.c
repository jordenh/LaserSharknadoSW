#include "bullet.h"

void initBullets() {
	int i = 0;

	bulletArray = malloc(NUM_BULLETS*sizeof(Bullet));

	for (i = 0; i < NUM_BULLETS; i++)
		bulletArray[i].status = NOTACTIVE;
}

void createBullet(bulletstatus status) {
	int index = 0;
	while (index < NUM_BULLETS) {
		if (bulletArray[index].status == NOTACTIVE)	{
			bulletArray[index].x = player.x + PLAYER_WIDTH + 1;
			bulletArray[index].y = player.y + 0.5*PLAYER_HEIGHT;
			bulletArray[index].status = PLAYERBULLET;
			drawBullet(&bulletArray[index]);
			break;
		}
		index++;
	}

	//playLaser();
}

void moveAllBullets() {
	int i;

	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].status == PLAYERBULLET) {
			moveBulletRight(&bulletArray[i]);
		} else if (bulletArray[i].status == SHARKBULLET) {
			moveBulletLeft(&bulletArray[i]);
		}
	}
}

void drawAllBullets() {
	int i;

	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].status != NOTACTIVE) {
			drawBullet(&bulletArray[i]);
		}
	}
}

void eraseAllBullets() {
	int i;

	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].status != NOTACTIVE) {
			eraseBullet(&bulletArray[i]);
		}
	}
}

void drawBullet(Bullet *bullet) {
	int i;

	for (i = 0; i < NUM_BULLETS; i++) {
		drawPixel(bullet->x + i, bullet->y, 0xFF00);
	}
}

void eraseBullet(Bullet *bullet) {
	int i;
	for (i = 0; i < NUM_BULLETS; i++) {
		drawPixel(bullet->prevX + i, bullet->prevY, 0x0000);
	}
}

void moveBulletRight(Bullet *bullet) {
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x + 1;
	if (bullet->x >= SCREEN_WIDTH) {
		bullet->status = NOTACTIVE;
		return;
	}
}

void moveBulletLeft(Bullet *bullet) {
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x - 1;
	if (bullet->x >= SCREEN_WIDTH) {
		bullet->status = NOTACTIVE;
		return;
	}
}


