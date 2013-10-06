#include "shark.h"

void initShark() {
	int i;

	for (i = 0; i < NUM_SHARKS; i++) {
		sharkArray[i].type = DEAD;
	}
}

void createShark(int sudoRandomSeed)
{
	int index = 0;
	while (index < NUM_SHARKS) {
		if (sharkArray[index].type == DEAD)	{
			sharkArray[index].freq = (sudoRandomSeed%10) + PLAYER_HEIGHT + 1;
			sharkArray[index].count = 0;
			sharkArray[index].x = SCREEN_WIDTH - SHARK_WIDTH;
			if (index%2 == 0) {
				sharkArray[index].type = TRAVERSETB;
				sharkArray[index].y = 0;
			} else {
				sharkArray[index].type = TRAVERSEBT;
				sharkArray[index].y = SCREEN_HEIGHT - SHARK_HEIGHT;
			}
			//sharkArray[index].displacement = circularDisplacementFunction;
			break;
		}
		index++;
	}
}


void moveAllSharks() {
	int i;

	for (i = 0; i < NUM_SHARKS; i++) {
		if ((sharkArray[i].type != DEAD) || (sharkArray[i].type != RECENTLYDEAD)) {
			moveShark(&sharkArray[i]);
			drawShark(&sharkArray[i]);
		}
	}
}

void eraseAllSharks(){
	int i;

	for (i = 0; i < NUM_SHARKS; i++) {
		if (sharkArray[i].type != DEAD) {
			eraseShark(&sharkArray[i]);
			if (sharkArray[i].type == RECENTLYDEAD) {
				sharkArray[i].type == DEAD;
			}
		}
	}
}

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

	if (shark->type == TRAVERSEBT) {
		shark->x--;
		shark->y--;
	} else if (shark->type == TRAVERSETB) {
		shark->x--;
		shark->y++;
	}

	if ((shark->x >= SCREEN_WIDTH) || (shark->x <= -SHARK_WIDTH)) {
		shark->type = RECENTLYDEAD;
	} else if ((shark->y >= SCREEN_HEIGHT) || (shark->y <= -SHARK_HEIGHT)) {
		shark->type = RECENTLYDEAD;
	}
	/*
	shark->x += shark->displacement->dx;
	shark->y += shark->displacement->dy;

	shark->displacement = shark->displacement->next;
	*/
}
