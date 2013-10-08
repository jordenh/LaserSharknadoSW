#include "bullet.h"

Bullet *playerBulletList = NULL;
Bullet *sharkBulletList = NULL;

void initBullets() {
	int i = 0;
	for (i = 0; i < NUM_BULLETS; i++) {
		bulletArray[i].type = NOTACTIVE;
		bulletArray[i].next = NULL;
		bulletArray[i].prev = NULL;
	}
}

void createBullet(bullettype type, int x, int y) {
	int index = 0;
	Bullet *activeBullet;
	Bullet *newBullet;
	while (index < NUM_BULLETS) {
		if (bulletArray[index].type == NOTACTIVE)	{
			newBullet = &(bulletArray[index]);
			newBullet->x = x;
			newBullet->y = y;
			newBullet->prevX = x;
			newBullet->prevY = y;
			newBullet->type = type;
			newBullet->next = NULL;
			newBullet->prev = NULL;

			drawBullet(newBullet);

			if (activeBullet != NULL) {
				if (activeBullet->next != NULL) {
					(activeBullet->next)->prev = newBullet;
				}
				newBullet->next = activeBullet->next;
				activeBullet->next = newBullet;
				newBullet->prev = activeBullet;
			}

			break;
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
		Bullet *nextBullet = bullet->next;
		Bullet *prevBullet = bullet->prev;
		bullet->next = NULL;
		bullet->prev = NULL;

		if (nextBullet != NULL) {
			nextBullet->prev = prevBullet;
		}
		if (prevBullet != NULL) {
			prevBullet->next = nextBullet;
		}
	}
}

void moveBulletLeft(Bullet *bullet) {
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x - 2;

	if ((bullet->x >= SCREEN_WIDTH) || (bullet->x <= -BULLET_LENGTH - 1)) {
		bullet->type = NOTACTIVE;
		Bullet *nextBullet = bullet->next;
		Bullet *prevBullet = bullet->prev;
		bullet->next = NULL;
		bullet->prev = NULL;

		if (nextBullet != NULL) {
			nextBullet->prev = prevBullet;
		}
		if (prevBullet != NULL) {
			prevBullet->next = nextBullet;
		}
	}
}
