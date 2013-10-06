#include <stdio.h>

#include "collision.h"

Shark *findSharkIfHit(Bullet *bullet) {
	Shark *cursor = sharkList;
	while (cursor != NULL) {
		if (isBulletCollidingWithShark(cursor, bullet) == TRUE) {
			return cursor;
		}
	}
	return NULL;
}

int isBulletCollidingWithShark(Shark *shark, Bullet *bullet) {
	if (bullet->status == SHARKBULLET) {
		// No friendly fire
		return FALSE;
	}
	
	if (bullet->x >= shark->x &&
		bullet->x <= shark->x + SHARK_LENGTH) {
		// Have x region correct
		
		if (bullet->y >= shark->y &&
			bullet->y <= shark->y + SHARK_HEIGHT) {
			// Have y region correct
			
			return TRUE;
		}
	}
	return FALSE;
}

int isBulletCollidingWithPlayer(Player *player, Bullet *bullet) {
	if (bullet->status == PLAYERBULLET) {
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
	while (bulletCursor != NULL && bulletCursor->status == PLAYERBULLET) {
		toKill = findSharkIfHit(bulletCursor);
		if (toKill != NULL) {
			killShark(toKill);
		}
		bulletCursor = bulletCursor->next;
	}
}
