#ifndef STUBS_H
#define STUBS_H

#include "displacement.h"

#define RIGHT 1
#define LEFT -1
#define TRUE 1
#define FALSE 0
#define SHARK_LENGTH 37
#define SHARK_HEIGHT 20

// Forward declaration
struct Displacement;

// STUB
struct Shark {
	int x;
	int y;
	struct Displacement *displacement;
};

typedef struct Shark Shark;

#endif
