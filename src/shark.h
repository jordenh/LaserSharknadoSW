#ifndef SHARK_H
#define SHARK_H

//#include "bmp.h"
#include "bullet.h"
#include "audio.h"
//#include "score.h" // TBD - FIX AND INCLUDE!!

#define SHARK_WIDTH 37
#define SHARK_HEIGHT 20
#define NUM_SHARKS 32
#define SHARK_LASER_LOCATION 13

#define LIVE 1
#define RECENTLY_DEAD 2
#define DEAD 3

struct Displacement {
	short int dx;
	short int dy;
	struct Displacement *next;
};

struct Shark {
	int x, prevX;
	int y, prevY;
	int freq, count;
	struct Shark *next;
	struct Shark *prev;
	struct Displacement *displacement;
	short int state;
};

typedef struct Displacement Displacement;
typedef struct Shark Shark;

extern Shark *sharkList;
extern unsigned int sharkCount;
extern Shark *deadSharkList;

void initSharks(void);
void drawShark(Shark *shark);
void eraseShark(Shark *shark);
void moveShark(Shark *shark);
void createShark(int sudoRandomSeed, int x, int y, Displacement *displacement);
void killShark(Shark *shark);
void moveAllSharks(void);
void drawAllSharks(void);
void eraseAllSharks(void);
void cleanupDeadSharks(void);

#endif
