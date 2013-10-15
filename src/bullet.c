#include "bullet.h"

Bullet *playerBulletList = NULL;
Bullet *sharkBulletList = NULL;
int playerLaserColor;
int sharkLaserColor;

void initBullets() {
	int i = 0;
	for (i = 0; i < NUM_BULLETS; i++) {
		bulletArray[i].type = NOTACTIVE;
		bulletArray[i].next = NULL;
		bulletArray[i].prev = NULL;
	}

	playerLaserColor = convert24BitRgbTo16(0xFF0000);
	sharkLaserColor = convert24BitRgbTo16(0x66FFCC);
}

void createBullet(bullettype type, int x, int y, int sudoRandomSeed) {
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
		newBullet->laserColor = playerLaserColor;
		newBullet->isAimed = 0;
		newBullet->slope = 0;
		playerBulletList = newBullet;
	} else if (type == SHARKBULLET) {
		if (sharkBulletList != NULL) {
			sharkBulletList->prev = newBullet;
		}
		newBullet->next = sharkBulletList;
		newBullet->laserColor = sharkLaserColor;
		newBullet->isAimed = 1;
		//newBullet->isAimed = sudoRandomSeed%2;
		newBullet->slope = (y - (player.y + 0.5*PLAYER_HEIGHT))/(x - (player.x + 0.5*PLAYER_WIDTH));
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
		}
	}
}

void drawAllBullets() {
	int i;
	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].type != NOTACTIVE && bulletArray[i].type != DELETE) {
			drawBullet(&bulletArray[i]);
		}
	}
}

void eraseAllBullets() {
	int i;
	for (i = 0; i < NUM_BULLETS; i++) {
		if (bulletArray[i].type != NOTACTIVE) {
			eraseBullet(&bulletArray[i]);

			if (bulletArray[i].type == DELETE)
				bulletArray[i].type = NOTACTIVE;
		}
	}
}

void drawBullet(Bullet *bullet) {
	int i;
	for (i = 0; i < BULLET_LENGTH; i++) {
		if (bullet != NULL) {
			drawPixelFast(bullet->x + i, bullet->y + i*bullet->slope, bullet->laserColor);
		} else {
			printf("Attempt to draw null bullet.\n");
		}
	}
}

void eraseBullet(Bullet *bullet) {
	int i;

	for (i = 0; i < BULLET_LENGTH; i++) {
		if (bullet != NULL) {
			drawPixelFast(bullet->prevX + i, bullet->prevY + i*bullet->slope, 0x0000);
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

	if (bullet->x >= SCREEN_WIDTH) {
		deleteBullet(bullet);
	}
}

void deleteBullet(Bullet *bullet) {
	eraseBullet(bullet);
	bullet->type = DELETE;
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
	bullet->y = bullet->y - 2*bullet->slope;

	if (bullet->x <= 0) {
		deleteBullet(bullet);
	}
}
