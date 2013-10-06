#include "shark.h"

Shark *sharkList = NULL;
unsigned int sharkCount = 0;

void drawShark(Shark *shark) {
	drawBmp(sharkBmp, shark->x, shark->y);
}

void eraseShark(Shark *shark){
	eraseBmp(sharkBmp, shark->prevX, shark->prevY);
}

void moveShark(Shark *shark) {
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
	while (cursor != NULL) {
		eraseShark(cursor);
		cursor = cursor->next;
	}
}

void createShark(int x, int y, struct Displacement *displacement) {
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
	sharkCount++;
}

void killShark(Shark *shark) {
	Shark *previousShark = shark->prev;
	Shark *nextShark = shark->next;
	// Need to earase now because we free the shark
	eraseShark(shark);

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
	free(shark);
}
