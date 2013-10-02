#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "stubs.h"

#define DISPLACEMENT_LENGTH 128

struct Displacement {
	short int dx;
	short int dy;
	struct Displacement *next;
};

typedef struct Displacement Displacement;

// Forward declaration
struct Shark;

void setupDisplacement(void);
void setToRandomDisplacement(struct Shark *shark);

extern Displacement verticalDisplacementFunctionUp[DISPLACEMENT_LENGTH];
extern Displacement verticalDisplacementFunctionDown[DISPLACEMENT_LENGTH];
extern Displacement circularDisplacementFunction[DISPLACEMENT_LENGTH];
extern Displacement arcDisplacementFunction[DISPLACEMENT_LENGTH];

#endif
