#include "bullet.h"

void initBullets() {
	int i = 0;

	bulletArray = malloc(NUM_BULLETS*sizeof(Bullet));

	for (i = 0; i < NUM_BULLETS; i++)
		bulletArray[i].status = NOTACTIVE;
}

void drawBullet(Bullet *bullet) {
	drawLine(bullet->x, bullet->y, bullet->x + BULLET_LENGTH, bullet->y, 0xFF00);
}

void eraseBullet(Bullet *bullet) {
	drawLine(bullet->x, bullet->y, bullet->x + BULLET_LENGTH, bullet->y, 0x0000);
}

void moveRight(Bullet *bullet) {
	eraseBullet(bullet);
	bullet->x = bullet->x + 1;
	if (bullet->x >= SCREEN_WIDTH) {
		bullet->status = NOTACTIVE;
		return;
	}
	drawBullet(bullet);
}


