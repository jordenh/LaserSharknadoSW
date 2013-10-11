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
	int index;
	Bullet *newBullet = NULL;
	for (index = 0; index < NUM_BULLETS; index++) {
		if (bulletArray[index].type == NOTACTIVE)	{
			newBullet = &(bulletArray[index]);
			newBullet->x = x;
			newBullet->y = y;
			newBullet->prevX = x;
			newBullet->prevY = y;
			newBullet->type = type;

			break;
		}
	}

	if (type == PLAYERBULLET) {
		if (playerBulletList != NULL) {
			playerBulletList->prev = newBullet;
		}
		newBullet->next = playerBulletList;
		playerBulletList = newBullet;
	} else {
		if (sharkBulletList != NULL) {
			sharkBulletList->prev = newBullet;
		}
		newBullet->next = sharkBulletList;
		sharkBulletList = newBullet;
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
		} else if (bulletArray[i].type == DELETED || bulletArray[i].type == ONEMORE) {
			bulletArray[i].prevX = bulletArray[i].x;
			bulletArray[i].prevY = bulletArray[i].y;
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
		if (bullet != NULL) {
			drawPixel(bullet->x + i, bullet->y, convert24BitRgbTo16(0xFF000C));
		} else {
			printf("Attempt to draw null bullet.\n");
		}
	}
}

void eraseBullet(Bullet *bullet) {
	int i;
	for (i = 0; i < BULLET_LENGTH; i++) {
		if (bullet != NULL) {
			drawPixel(bullet->prevX + i, bullet->prevY, 0x0000);
			bullet->type = bullet->type == DELETED ? ONEMORE : bullet->type;
			bullet->type = bullet->type == ONEMORE ? NOTACTIVE : bullet->type;
		} else {
			printf("Attempt to draw null bullet.\n");
		}
	}
}

void moveBulletRight(Bullet *bullet) {
	if (bullet == NULL) {
		printf("Attempt to move null bullet right.\n");
		return;
	}

	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x + 2;

	if ((bullet->x >= SCREEN_WIDTH) || (bullet->x <= -BULLET_LENGTH - 1)) {
		deleteBullet(bullet);
	}
}

void deleteBullet(Bullet *bullet) {
	eraseBullet(bullet);
	bullet->type = DELETED;
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;
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

void moveBulletLeft(Bullet *bullet) {
	if (bullet == NULL) {
		printf("Attempt to move null bullet left.\n");
		return;
	}

	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x - 2;

	if ((bullet->x >= SCREEN_WIDTH) || (bullet->x <= -BULLET_LENGTH - 1)) {
		deleteBullet(bullet);
	}
}
