#include "bullet.h"

void initBullets() {
	int i = 0;

	for (i = 0; i < NUM_BULLETS; i++)
		bulletArray[i].type = NOTACTIVE;
}

void createBullet(bullettype status, int x, int y) {
	int index = 0;
	while (index < NUM_BULLETS) {
		if (bulletArray[index].type == NOTACTIVE)	{
			bulletArray[index].x = x;
			bulletArray[index].y = y;
			bulletArray[index].type = status;
			drawBullet(&bulletArray[index]);
			break;
		}
		index++;
	}

	playLaser();
}

void moveAllBullets() {
	int i;

	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].type == PLAYERBULLET) {
			moveBulletRight(&bulletArray[i]);
		} else if (bulletArray[i].type == SHARKBULLET) {
			moveBulletLeft(&bulletArray[i]);
		}
	}
}

void drawAllBullets() {
	int i;

	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].type != NOTACTIVE) {
			drawBullet(&bulletArray[i]);
		}
	}
}

void eraseAllBullets() {
	int i;

	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].type != NOTACTIVE) {
			eraseBullet(&bulletArray[i]);
		}
	}
}

void drawBullet(Bullet *bullet) {
	int i;

	for (i = 0; i < BULLET_LENGTH; i++) {
		drawPixel(bullet->x + i, bullet->y, convert24BitRgbTo16(0xFF000C));
	}
}

void eraseBullet(Bullet *bullet) {
	int i;
	for (i = 0; i < BULLET_LENGTH; i++) {
		drawPixel(bullet->prevX + i, bullet->prevY, 0x0000);
	}
}

void moveBulletRight(Bullet *bullet) {
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x + 2;
	if ((bullet->x >= SCREEN_WIDTH) || (bullet->x <= -BULLET_LENGTH - 1)) {
		bullet->type = NOTACTIVE;
		return;
	}
}

void moveBulletLeft(Bullet *bullet) {
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x - 2;
	if ((bullet->x >= SCREEN_WIDTH) || (bullet->x <= -BULLET_LENGTH - 1)) {
		bullet->type = NOTACTIVE;
		return;
	}
}
