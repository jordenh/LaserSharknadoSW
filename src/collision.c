#include <stdio.h>

#include "collision.h"

#define TRUE 1
#define FALSE 0

Shark *findSharkIfHit(Bullet *bullet) {
	Shark *cursor = sharkList;
	int i = 0;
	while (cursor != NULL
			&& i < sharkCount) {
		if (isBulletCollidingWithShark(cursor, bullet) == TRUE) {
			return cursor;
		}
		cursor = cursor->next;
		i++;
	}
	return NULL;
}

int isBulletCollidingWithShark(Shark *shark, Bullet *bullet) {
	if (bullet->type == SHARKBULLET) {
		// No friendly fire
		return FALSE;
	}
	
	if (bullet->x >= shark->x &&
		bullet->x <= shark->x + SHARK_WIDTH) {
		// Have x region correct
		//printf("x-hit\n");
		
		if (bullet->y >= shark->y &&
			bullet->y <= shark->y + SHARK_HEIGHT) {
			// Have y region correct
			//printf("y-hit\n");
			return TRUE;
		}
	}
	return FALSE;
}

int isBulletCollidingWithPlayer(Player *player, Bullet *bullet) {
	if (bullet->type == PLAYERBULLET) {
		// No friendly fire
		return FALSE;
	}

	if (bullet->x >= player->x &&
		bullet->x <= player->x + PLAYER_WIDTH) {
		// Have x region
		
		if (bullet->y >= player->y &&
			bullet->y <= player->y + PLAYER_HEIGHT) {
			// Have y
			
			return TRUE;
		}
	}
	return FALSE;
}

void doSharkBulletCollision(void) {
	Bullet *bulletCursor = playerBulletList;
	Shark *toKill = NULL;
	int i = 0;
	while (bulletCursor != NULL
			&& bulletCursor->type == PLAYERBULLET
			&& i < NUM_BULLETS) {
		toKill = findSharkIfHit(bulletCursor);
		if (toKill != NULL) {
			killShark(toKill);
		}
		bulletCursor = bulletCursor->next;
		i++;
	}
}

void doPlayerBulletCollision(void) {
	Bullet *bulletCursor = sharkBulletList;
	int i = 0;
	while (bulletCursor != NULL
			&& bulletCursor->type == SHARKBULLET
			&& i < NUM_BULLETS) {
		if (isBulletCollidingWithPlayer(&player, bulletCursor) == TRUE) {
			killPlayer();
			break;
		}
	}
}
