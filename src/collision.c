#include <stdio.h>

#include "collision.h"

#define TRUE 1
#define FALSE 0

short int relativeSharkHit[SHARK_WIDTH][SHARK_HEIGHT];
short int relativePlayerHit[PLAYER_WIDTH][PLAYER_HEIGHT];
short int relativeNadoHit[NADO_WIDTH][NADO_HEIGHT];

void initCollision(void) {
	short int *colourArray;
	short int *cursor;
	colourArray = sharkBmp->color;
	int x, y;
	int yOffset;

	for (y = 0; y < SHARK_HEIGHT; y++) {
		yOffset = SHARK_WIDTH * y;
		for (x = 0; x < SHARK_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (*cursor > 0) {
				relativeSharkHit[x][y] = TRUE;
			} else {
				relativeSharkHit[x][y] = FALSE;
			}
		}
	}

	colourArray = playerBmp->color;
	for (y = 0; y < PLAYER_HEIGHT; y++) {
		yOffset = PLAYER_WIDTH * y;
		for (x = 0; x < PLAYER_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			//printf("r: 0x%x; g: 0x%x; b: 0x%x\n", cursor->r, cursor->g, cursor->b);
			if (*cursor > 0) {
				relativePlayerHit[x][y] = TRUE;
			} else {
				relativePlayerHit[x][y] = FALSE;
			}
		}
	}

	colourArray = cnadoBmp->color;
	for (y = 0; y < NADO_HEIGHT; y++) {
		yOffset = NADO_WIDTH * y;
		for (x = 0; x < NADO_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (*cursor > 0) {
				relativeNadoHit[x][y] = TRUE;
			} else {
				relativeNadoHit[x][y] = FALSE;
			}
		}
	}

	colourArray = pnadoaBmp->color;
	for (y = 0; y < NADO_HEIGHT; y++) {
		yOffset = NADO_WIDTH * y;
		for (x = 0; x < NADO_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (*cursor > 0) {
				relativeNadoHit[x][y] = TRUE;
			}
		}
	}

	colourArray = pnadobBmp->color;
	for (y = 0; y < NADO_HEIGHT; y++) {
		yOffset = NADO_WIDTH * y;
		for (x = 0; x < NADO_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (*cursor > 0) {
				relativeNadoHit[x][y] = TRUE;
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

short int isBulletCollidingWithNado(Bullet *bullet) {
	if (bullet->x >= SCREEN_WIDTH - NADO_WIDTH) {
		int xRelative = bullet->x - (SCREEN_WIDTH - NADO_WIDTH);
		int yRelative = bullet->y;
		return relativeNadoHit[xRelative][yRelative];
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
			deleteBullet(bulletCursor);
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
			deleteBullet(bulletCursor);
			break;
		}
		i++;
		bulletCursor = bulletCursor->next;
	}
}

void doNadoBulletCollision(void) {
	Bullet *bulletCursor = playerBulletList;
	int i = 0;
	while (bulletCursor != NULL
			&& bulletCursor->type == PLAYERBULLET
			&& i < NUM_BULLETS) {
		if (isBulletCollidingWithNado(bulletCursor) == TRUE) {
			deleteBullet(bulletCursor);
		}
	}
}
