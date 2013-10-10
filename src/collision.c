#include <stdio.h>

#include "collision.h"

#define TRUE 1
#define FALSE 0

int relativeSharkHit[SHARK_WIDTH][SHARK_HEIGHT];
int relativePlayerHit[PLAYER_WIDTH][PLAYER_HEIGHT];

void initCollision(void) {
	RGB *colourArray;
	RGB *cursor;
	colourArray = playerBmp->rgb;
	int x, y;
	int yOffset;

	for (y = 0; y < SHARK_HEIGHT; y++) {
		yOffset = SHARK_WIDTH * y;
		for (x = 0; x < SHARK_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (cursor->r > 0 &&
				cursor->g > 0 &&
				cursor->b > 0) {
				relativeSharkHit[x][y] = TRUE;
			} else {
				relativeSharkHit[x][y] = FALSE;
			}
		}
	}

	for (y = 0; y < PLAYER_HEIGHT; y++) {
		yOffset = PLAYER_WIDTH * y;
		for (x = 0; x < PLAYER_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (cursor->r > 0 &&
				cursor->g > 0 &&
				cursor->b > 0) {
				relativePlayerHit[x][y] = TRUE;
			} else {
				relativePlayerHit[x][y] = FALSE;
			}
		}
	}
}

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
	if (bullet == NULL || shark == NULL) {
		printf("Attempt to do player/bullet collision with null member.\n");
		return FALSE;
	}

	if (bullet->type == SHARKBULLET) {
		// No friendly fire
		printf("Error: sharkbullet present in player bullet list.\n");
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
			int yRelative = bullet->y - shark->y;
			int xRelative = bullet->x - shark->x;
			return relativeSharkHit[xRelative][yRelative];
		}
	}
	//printf("Miss\n");
	return FALSE;
}

int isBulletCollidingWithPlayer(Player *player, Bullet *bullet) {
	if (player == NULL || bullet == NULL) {
		printf("Attempt to do player/bullet collision with null member.\n");
		return FALSE;
	}

	if (bullet->type == PLAYERBULLET) {
		// No friendly fire
		printf("Error: player bullet in shark bullet list.\n");
		return FALSE;
	}

	if (bullet->x >= player->x &&
		bullet->x <= player->x + PLAYER_WIDTH) {
		// Have x region
		
		if (bullet->y >= player->y &&
			bullet->y <= player->y + PLAYER_HEIGHT) {
			// Have y
			int yRelative = bullet->y - player->y;
			int xRelative = bullet->x - player->x;
			return relativePlayerHit[xRelative][yRelative];
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
			hitPlayer();
			break;
		}
		i++;
		bulletCursor = bulletCursor->next;
	}
}
