#include "bullet.h"

Bullet *playerBulletList = NULL;
Bullet *sharkBulletList = NULL;

void initBullets() {
	int i = 0;
	for (i = 0; i < NUM_BULLETS; i++) {
		bulletArray[i].type = NOTACTIVE;
		bulletArray[i].next = NULL;
	}
}

void createBullet(bullettype type, int x, int y) {
	int index = 0;
	Bullet *activeBullet;
	Bullet *newBullet;
	while (index < NUM_BULLETS) {
		if (bulletArray[index].type == NOTACTIVE)	{
			bulletArray[index].x = x;
			bulletArray[index].y = y;
			bulletArray[index].type = type;
			drawBullet(&bulletArray[index]);

			activeBullet->next = &bulletArray[index];
			// TODO REMOVE - separation of concerns
			newBullet = &bulletArray[index];
			while (index < NUM_BULLETS) {
				index++;
				if (bulletArray[index].type == type) {
					newBullet->next = &bulletArray[index];
					break;
				}
			}
			// Last active bullet in the array if you get here
			newBullet->next = NULL;
		} else if (bulletArray[index].type == type){
			activeBullet = &bulletArray[index];
		}
		index++;
	}

	if (type == PLAYERBULLET) {
		if ((unsigned int)playerBulletList > (unsigned int)newBullet
				|| playerBulletList == NULL) {
			playerBulletList = newBullet;
		}
	} else {
		if ((unsigned int)sharkBulletList > (unsigned int)newBullet
				|| playerBulletList == NULL) {
			sharkBulletList = newBullet;
		}
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
		bullet->next = NULL;
		return;
	}
}

void moveBulletLeft(Bullet *bullet) {
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x - 2;

	if ((bullet->x >= SCREEN_WIDTH) || (bullet->x <= -BULLET_LENGTH - 1)) {
		bullet->type = NOTACTIVE;
		bullet->next = NULL;
		// TODO: restructure list ie make doubly linked
		return;
	}
}
