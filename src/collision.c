#include <stdio.h>

#include "collision.h"

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
