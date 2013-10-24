#ifndef SHARK_H
#define SHARK_H

//#include "bmp.h"
#include "bullet.h"
#include "audio.h"
#include "nado.h"
//#include "score.h" // TBD - FIX AND INCLUDE!!

#define SHARK_WIDTH 37
#define SHARK_HEIGHT 20
#define NUM_SHARKS 32
#define SHARK_LASER_LOCATION 13

#define LIVE 1
#define RECENTLY_DEAD 2
#define DEAD 3
#define TOPWALL 0
#define RIGHTWALL 1
#define BOTTOMWALL 2

struct Displacement {
	short int dx;
	short int dy;
	struct Displacement *next;
};

struct Shark {
	int x, prevX; // current and previous x location
	int y, prevY; // current and previous y location
	int freq, count; // frequency of bullet fire & count of steps since last bullet
	short smart; // flag for if the shark shoots smart bullets
	struct Shark *next; // pointers for the linked list of active sharks
	struct Shark *prev;
	struct Displacement *displacement; // current displacement
	unsigned short initialWall; // which wall the shark started at
	unsigned short entranceCount; // number of steps taken in from the initial wall
	unsigned short entranceSteps; // number of steps that shark should take in before starting the pattern
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
void createShark(int sudoRandomSeed, int x, int y, Displacement *displacement, unsigned short wall, short smart, unsigned short entranceStep);
void killShark(Shark *shark);
void moveAllSharks(void);
void drawAllSharks(void);
void eraseAllSharks(void);
void cleanupDeadSharks(void);

#endif
