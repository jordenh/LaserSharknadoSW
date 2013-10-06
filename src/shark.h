#ifndef SHARK_H
#define SHARK_H

#include "bmp.h"
#include "displacement.h"
#include "bullet.h"

#define SHARK_WIDTH 37
#define SHARK_HEIGHT 20
#define NUM_SHARKS 4
#define SHARK_LASER_LOCATION 13

//struct Displacement;

typedef enum {
	TRAVERSEBT,
	TRAVERSETB,
	RECENTLYDEAD,
	DEAD
} sharktype;

typedef struct {
	int x, prevX;
	int y, prevY;
	int freq, count;
	//struct Displacement *displacement;
	sharktype type;
} Shark;

Shark sharkArray[NUM_SHARKS];

void initShark();
void createShark(int sudoRandomSeed);
void moveAllSharks();
void eraseAllSharks();
void drawShark(Shark *shark);
void eraseShark(Shark *shark);
void moveShark(Shark *shark);

#endif
