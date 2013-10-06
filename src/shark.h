#ifndef __SHARK_H__
#define __SHARK_H__

#include "bmp.h"

#define SHARK_LENGTH 37
#define SHARK_HEIGHT 20

struct Displacement {
	short int dx;
	short int dy;
	struct Displacement *next;
};

struct Shark {
	int x, prevX;
	int y, prevY;
	struct Shark *next;
	struct Shark *prev;
	struct Displacement *displacement;
};

typedef struct Displacement Displacement;
typedef struct Shark Shark;

extern Shark *sharkList;

void drawShark(Shark *shark);
void eraseShark(Shark *shark);
void moveShark(Shark *shark);
void createShark(int x, int y, struct Displacement *displacement);
void killShark(Shark *shark);
void moveAllSharks(void);
void drawAllSharks(void);
void eraseAllSharks(void);

#endif
