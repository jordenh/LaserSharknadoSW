#include "shark.h"
#include "audio.h"
#include "score.h"
#include "displacement.h"

Shark *sharkList = NULL;
Shark *deadSharkList = NULL;
unsigned int sharkCount = 0;
Shark sharkArray[NUM_SHARKS];

void initSharks(void) {
	int i;
	Shark *cursor;
	sharkList = NULL;
	deadSharkList = NULL;
	sharkCount = 0;
	for (i = 0; i < NUM_SHARKS; i++) {
		cursor = &(sharkArray[i]);
		cursor->state = DEAD;
		cursor->next = NULL;
		cursor->prev = NULL;
		cursor->displacement = NULL;
		cursor->entranceCount = 0;
	}
}

Shark *getFreeShark(void) {
	int i;
	for (i = 0; i < NUM_SHARKS; i++) {
		if (sharkArray[i].state == DEAD) {
			return &(sharkArray[i]);
		}
	}
	return NULL;
}

void drawShark(Shark *shark) {
	if (shark != NULL) {
		if(shark->x > (SCREEN_WIDTH + SHARK_WIDTH) || shark->y < (-SHARK_HEIGHT) || shark->y > (SCREEN_WIDTH + SHARK_HEIGHT)) {
		} else {
			drawBmp(sharkBmp, shark->x, shark->y);
		}
	} else {
		printf("Attempt to draw null shark.\n");
	}
}

void eraseShark(Shark *shark){
	if (shark != NULL) {
		eraseBmp(sharkBmp, shark->prevX, shark->prevY);
	} else {
		printf("Attempt to erase null shark.\n");
	}
}

void moveShark(Shark *shark) {
	if (shark == NULL) {
		printf("Attempt to move null shark.\n");
		return;
	}

	shark->prevX = shark->x;
	shark->prevY = shark->y;

	shark->count++;

	if (shark->count >= shark->freq) {
		shark->count = 0;
		createBullet(SHARKBULLET, shark->x + BULLET_LENGTH, shark->y + SHARK_LASER_LOCATION, shark->smart);
	}

	// TODO: get rid of sharks that leave the screen
//	if ((shark->x >= SCREEN_WIDTH) || (shark->x <= -SHARK_WIDTH)) {
//		shark->type = RECENTLYDEAD;
//	} else if ((shark->y >= SCREEN_HEIGHT) || (shark->y <= -SHARK_HEIGHT)) {
//		shark->type = RECENTLYDEAD;
//	}

	if(shark->entranceCount >= ENTRANCESTEPS) {
		Displacement *disp = shark->displacement;
		shark->x += disp->dx;
		shark->y += disp->dy;
		shark->displacement = shark->displacement->next;
	} else {
		shark->entranceCount++;
		if(shark->initialWall == TOPWALL) {
			shark->x += 0;
			shark->y += 1;
		} else if(shark->initialWall == RIGHTWALL) {
			shark->x += -1;
			shark->y += 0;
		} else if(shark->initialWall == BOTTOMWALL) {
			shark->x += 0;
			shark->y += -1;
		}
	}
}

void moveAllSharks(void) {
	Shark *cursor = sharkList;
	while (cursor != NULL) {
		moveShark(cursor);
		cursor = cursor->next;
	}
}

void drawAllSharks(void) {
	Shark *cursor = sharkList;
	while (cursor != NULL) {
		drawShark(cursor);
		cursor = cursor->next;
	}
}

void eraseAllSharks(void) {
	Shark *cursor = sharkList;
	// TODO: erase recently dead list
	while (cursor != NULL) {
		eraseShark(cursor);
		cursor = cursor->next;
	}
}

void createShark(int sudoRandomSeed, int x, int y, Displacement *displacement, unsigned short wall, short smart) {
	if (displacement == NULL) {
		printf("Attempt to create shark with null displacement.\n");
		return;
	}

	//Shark *newShark = malloc(sizeof(Shark));
	Shark *newShark = getFreeShark();
	if (newShark == NULL) {
		return;
	}

	newShark->state = LIVE;
	newShark->smart = smart;
	newShark->initialWall = wall % 3; //% 3 to ensure that value is always between 0-2, even if caller messes up.
	if(newShark->initialWall == TOPWALL) {
		newShark->x = x;
		newShark->y = 0 - SHARK_HEIGHT;
	} else if(newShark->initialWall == BOTTOMWALL) {
		newShark->x = x;
		newShark->y = SCREEN_HEIGHT + SHARK_HEIGHT;
	} else if(newShark->initialWall == RIGHTWALL) {
		newShark->x = SCREEN_WIDTH - (NADO_WIDTH * 1.5) - SHARK_WIDTH;
		newShark->y = y;
	}
	newShark->prevX = newShark->x;
	newShark->prevY = newShark->y;

	newShark->displacement = displacement;
	newShark->prev = NULL;
	if (sharkList == NULL) {
		sharkList = newShark;
		newShark->next = NULL;
	} else {
		sharkList->prev = newShark;
		newShark->next = sharkList;
		sharkList = newShark;
	}
	newShark->freq = (sudoRandomSeed % 20) + 1.5*PLAYER_HEIGHT + 1;
	newShark->count = 0;
	newShark->entranceCount = 0;
	sharkCount++;
}

void killShark(Shark *shark) {
	if (shark == NULL) {
		printf("Attempt to kill null shark.\n");
		return;
	}

	shark->state = RECENTLY_DEAD;

	Shark *previousShark = shark->prev;
	Shark *nextShark = shark->next;
//
//	if (deadSharkList == NULL) {
//		deadSharkList = shark;
//		deadSharkList->next = NULL;
//	}
//	else {
//		deadSharkList->prev = shark;
//		shark->next = deadSharkList;
//		deadSharkList = shark;
//	}

	playSharkDeath();
	// Need to erase now because we free the shark
	eraseShark(shark);
	// TODO: increase player score

	if (previousShark != NULL) {
		// Not killing first shark
		previousShark->next = nextShark;
	} else {
		// Killing first shark
		sharkList = nextShark;
	}

	if (nextShark != NULL) {
		// Not killing last shark
		nextShark->prev = previousShark;
	} else {
		// Killing last shark
	}
	sharkCount--;
	updateCurrentPlayerScore(250);
	drawInGameInfo();
}

void cleanupDeadSharks() {
	int i;
	for (i = 0; i < NUM_SHARKS; i++) {
		if (sharkArray[i].state == RECENTLY_DEAD) {
			sharkArray[i].state = DEAD;
			sharkArray[i].prevX = sharkArray[i].x;
			sharkArray[i].prevY = sharkArray[i].y;
			eraseShark(&(sharkArray[i]));
		}
	}
	return;

//	Shark *cursor = deadSharkList;
//	Shark *next;
//	int i = 0;
//	while (cursor != NULL && i < sharkCount) {
//		// eraseShark uses previous values
//		cursor->prevX = cursor->x;
//		cursor->prevY = cursor->y;
//		eraseShark(cursor);
//		next = cursor->next;
//		//free(cursor);
//		cursor = next;
//		i++;
//	}
//	deadSharkList = NULL;
}
