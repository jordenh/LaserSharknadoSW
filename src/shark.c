#include "shark.h"

void drawShark(Shark *shark,int x, int y) {
	shark->x = x;
	shark->y = y;

	drawBmp(sharkBmp, x, y);
}

void eraseShark(Shark *shark){
	eraseBmp(sharkBmp, shark->prevX, shark->prevY);
}

void moveShark(Shark *shark, int x, int y) {
	shark->prevX = shark->x;
	shark->prevY = shark->y;
	shark->x = x;
	shark->y = y;
	drawBmp(sharkBmp, x, y);
}
