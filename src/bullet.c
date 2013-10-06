#include "bullet.h"

Bullet *playerBulletList = NULL;
Bullet *sharkBulletList = NULL;

void initBullets() {
	int i = 0;

	for (i = 0; i < NUM_BULLETS; i++) {
		bulletArray[i].status = NOTACTIVE;
		bulletArray[i].next = NULL;
	}
}

void createBullet(bulletstatus status) {
	int index = 0;
	Bullet *activeBullet;
	Bullet *newBullet;
	while (index < NUM_BULLETS) {
		if (bulletArray[index].status == NOTACTIVE)	{
			bulletArray[index].x = player.x + PLAYER_WIDTH + 1;
			bulletArray[index].y = player.y + 0.5 * PLAYER_HEIGHT;
			bulletArray[index].status = status;

			activeBullet->next = &bulletArray[index];
			// TODO REMOVE - separation of concerns
			drawBullet(&bulletArray[index]);
			newBullet = &bulletArray[index];
			while (index < NUM_BULLETS) {
				index++;
				if (bulletArray[index].status == status) {
					newBullet->next = &bulletArray[index];
					break;
				}
			}
			// Last active bullet in the array if you get here
			newBullet->next = NULL;
		} else if (bulletArray[index].status == status){
			activeBullet = &bulletArray[index];
		}
		index++;
	}

	if (status == PLAYERBULLET) {
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
	if (bullet->x >= SCREEN_WIDTH) {
		bullet->status = NOTACTIVE;
		bullet->next = NULL;
		return;
	}
}

void moveBulletLeft(Bullet *bullet) {
	bullet->prevX = bullet->x;
	bullet->prevY = bullet->y;

	bullet->x = bullet->x - 2;
	if (bullet->x >= SCREEN_WIDTH) {
		bullet->status = NOTACTIVE;
		bullet->next = NULL;
		return;
	}
}
