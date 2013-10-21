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

	// Initialize the pixel-perfect collision lookup table for sharks
	for (y = 0; y < SHARK_HEIGHT; y++) {
		yOffset = SHARK_WIDTH * y;
		for (x = 0; x < SHARK_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (*cursor != 0) {
				relativeSharkHit[x][y] = TRUE;
			} else {
				relativeSharkHit[x][y] = FALSE;
			}
		}
	}

	// Initialize the pixel-perfect collision lookup table for the player
	colourArray = playerBmp->color;
	for (y = 0; y < PLAYER_HEIGHT; y++) {
		yOffset = PLAYER_WIDTH * y;
		for (x = 0; x < PLAYER_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			//printf("r: 0x%x; g: 0x%x; b: 0x%x\n", cursor->r, cursor->g, cursor->b);

			if (*cursor != 0) {
				relativePlayerHit[x][y] = TRUE;
			} else {
				relativePlayerHit[x][y] = FALSE;
			}
		}
	}

	// Initialize the pixel-perfect collision lookup table for the tornado
	// Done in multiple parts as multiple bitmaps make up the tornado
	colourArray = cnadoBmp->color;
	for (y = 0; y < NADO_HEIGHT; y++) {
		yOffset = NADO_WIDTH * y;
		for (x = 0; x < NADO_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (*cursor != 0) {
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
			if (*cursor != 0) {
				relativeNadoHit[x][y] = TRUE;
			}
		}
	}

	colourArray = pnadobBmp->color;
	for (y = 0; y < NADO_HEIGHT; y++) {
		yOffset = NADO_WIDTH * y;
		for (x = 0; x < NADO_WIDTH; x++) {
			cursor = &colourArray[yOffset + x];
			if (*cursor != 0) {
				relativeNadoHit[x][y] = TRUE;
			}
		}
	}

	// Attempted tornado collision bugfix
	bool shouldCollideForRestOfRow = false;
	for (y = 0; y < NADO_HEIGHT; y++) {
		yOffset = NADO_HEIGHT;
		shouldCollideForRestOfRow = false;
		for (x = 0; x < NADO_WIDTH; x++) {
			if (relativeNadoHit[x][y] == 1 || shouldCollideForRestOfRow == true) {
				shouldCollideForRestOfRow = true;
				relativeNadoHit[x][y] = 1;
			}
		}
	}
}

// Returns a pointer to the shark the bullet argument is colliding with, NULL otherwise
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

// Determines if a bullet and a shark are colliding
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

	// Check x region first
	if (bullet->x >= shark->x &&
		bullet->x <= shark->x + SHARK_WIDTH) {

		// Then check y region
		if (bullet->y >= shark->y &&
			bullet->y <= shark->y + SHARK_HEIGHT) {
			// Then compute relative x and y and check the pixel-perfect collision lookup table for sharks
			int yRelative = bullet->y - shark->y;
			int xRelative = bullet->x - shark->x;
			return relativeSharkHit[xRelative][yRelative];
		}
	}
	return FALSE;
}

// Determines if a bullet and the player are colliding
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

	// Check x region first
	if (bullet->x >= player->x &&
		bullet->x <= player->x + PLAYER_WIDTH) {

		// Then check y region
		if (bullet->y >= player->y &&
			bullet->y <= player->y + PLAYER_HEIGHT) {
			// Compute relative x and y, use them to check the pixel-perfect collision lookup table for the player
			int yRelative = bullet->y - player->y;
			int xRelative = bullet->x - player->x;
			return relativePlayerHit[xRelative][yRelative];
		}
	}
	return FALSE;
}

// Does not work properly
short int isBulletCollidingWithNado(Bullet *bullet) {
	if (bullet->x >= nadoDrawX) {
		int xRelative = bullet->x - nadoDrawX;
		int yRelative = bullet->y;
		return relativeNadoHit[xRelative][yRelative];
	}
	/*if (bullet->x >= nadoDrawX - 6) {
		int xRelative = bullet->x - nadoDrawX + 4;
		int yRelative = bullet->y;
		return relativeNadoHit[xRelative][yRelative];
	} */
	return FALSE;
}

// Performs all shark-bullet collision
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

// Performs all player-bullet collision
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

// Performs all tornado-bullet collision
void doNadoBulletCollision(void) {
	Bullet *bulletCursor = playerBulletList;
	Bullet *nextBullet = playerBulletList->next;
	int i = 0;
	while (bulletCursor != NULL
			&& bulletCursor->type == PLAYERBULLET
			&& i < NUM_BULLETS) {
		nextBullet = bulletCursor->next;
		if (isBulletCollidingWithNado(bulletCursor) == TRUE) {
			deleteBullet(bulletCursor);
		}
		i++;
		bulletCursor = nextBullet;
	}
}
