#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "shark.h"

#define DISPLACEMENT_LENGTH 128

void setupDisplacement(void);
void setToRandomDisplacement(Shark *shark);

extern Displacement verticalDisplacementFunctionUp[DISPLACEMENT_LENGTH];
extern Displacement verticalDisplacementFunctionDown[DISPLACEMENT_LENGTH];
extern Displacement circularDisplacementFunction[DISPLACEMENT_LENGTH];
extern Displacement arcDisplacementFunction[DISPLACEMENT_LENGTH];
extern Displacement doNotMove[DISPLACEMENT_LENGTH];

#endif
