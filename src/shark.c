#include "shark.h"
#include "audio.h"

Shark *sharkList = NULL;
Shark *deadSharkList = NULL;
unsigned int sharkCount = 0;

void drawShark(Shark *shark) {
	drawBmp(sharkBmp, shark->x, shark->y);
}

void eraseShark(Shark *shark){
	eraseBmp(sharkBmp, shark->prevX, shark->prevY);
}

void moveShark(Shark *shark) {
	shark->prevX = shark->x;
	shark->prevY = shark->y;

	shark->count++;

	if (shark->count >= shark->freq) {
		shark->count = 0;
		createBullet(SHARKBULLET, shark->x + BULLET_LENGTH, shark->y + SHARK_LASER_LOCATION);
	}

	// TODO: get rid of sharks that leave the screen
//	if ((shark->x >= SCREEN_WIDTH) || (shark->x <= -SHARK_WIDTH)) {
//		shark->type = RECENTLYDEAD;
//	} else if ((shark->y >= SCREEN_HEIGHT) || (shark->y <= -SHARK_HEIGHT)) {
//		shark->type = RECENTLYDEAD;
//	}

	Displacement *disp = shark->displacement;
	shark->x += disp->dx;
	shark->y += disp->dy;
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

void createShark(int sudoRandomSeed, int x, int y, struct Displacement *displacement) {
	Shark *newShark = malloc(sizeof(Shark));
	newShark->x = x;
	newShark->y = y;
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
	newShark->freq = (sudoRandomSeed % 10) + PLAYER_HEIGHT + 1;
	newShark->count = 0;
	sharkCount++;
}

void killShark(Shark *shark) {
	Shark *previousShark = shark->prev;
	Shark *nextShark = shark->next;

	if (deadSharkList == NULL) {
		deadSharkList = shark;
		shark->next == NULL;
	}
	else {
		deadSharkList->prev = shark;
		shark->next = deadSharkList;
		deadSharkList = shark;
	}

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
}

void cleanupDeadSharks() {
	Shark *cursor = deadSharkList;
	Shark *next;
	while (cursor != NULL) {
		// eraseShark uses previous values
		cursor->prevX = cursor->x;
		cursor->prevY = cursor->y;
		eraseShark(cursor);
		next = cursor->next;
		free(cursor);
		cursor = next;
	}
	deadSharkList = NULL;
}
