#include <stdio.h>

#include "collision.h"

#include "player.h"
#include "stubs.h"
//#include "bullet.h"
//#include "shark.h"

int isBulletCollidingWithShark(Shark *shark, Bullet *bullet) {
	if (bullet->direction == LEFT) {
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
	if (bullet->direction == RIGHT) {
		// No friendly fire
		return FALSE;
	}

	if (bullet->x >= player->x &&
		bullet->x <= player->x + PLAYER_LENGTH) {
		// Have x region
		
		if (bullet->y >= player->y &&
			bullet->y <= player->y + PLAYER_HEIGHT) {
			// Have y
			
			return TRUE;
		}
	}
	return FALSE;
}
