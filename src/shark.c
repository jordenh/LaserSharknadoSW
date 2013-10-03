#include "shark.h"

void drawShark(Shark *shark,int x, int y) {
	shark->x = x;
	shark->y = y;

	drawBmp(sharkBmp, x, y);
}

void eraseShark(Shark *shark){
	eraseBmp(sharkBmp, shark->x, shark->y);
}

void moveShark(Shark *shark, int x, int y) {
	eraseShark(shark);

	shark->x = x;
	shark->y = y;
	drawBmp(sharkBmp, x, y);
}
