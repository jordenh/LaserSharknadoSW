#include <stdio.h>
#include "displacement.h"

Displacement verticalDisplacementFunctionUp[DISPLACEMENT_LENGTH];
Displacement verticalDisplacementFunctionDown[DISPLACEMENT_LENGTH];
Displacement circularDisplacementFunction[DISPLACEMENT_LENGTH];
Displacement arcDisplacementFunction[DISPLACEMENT_LENGTH];

void setupHorizontalDisplacementFunction(void);
void setupCircularDisplacementFunction(void);
void setupArcDisplacementFunction(void);

static Displacement right;
static Displacement left;
static Displacement up;
static Displacement down;

void makeRight(Displacement *disp);
void makeLeft(Displacement *disp);
void makeUp(Displacement *disp);
void makeDown(Displacement *disp);
void makeDownRight(Displacement *disp);
void makeDownLeft(Displacement *disp);
void makeUpRight(Displacement *disp);
void makeUpLeft(Displacement *disp);

void setupDisplacement(void) {
	right.dx = 1;
	right.dy = 0;

	left.dx = -1;
	left.dy = 0;

	up.dx = 0;
	up.dy = -1;

	down.dx = 0;
	up.dy = 1;

	setupHorizontalDisplacementFunction();
	setupCircularDisplacementFunction();
	setupArcDisplacementFunction();

	printf("Setup displacement functions.\n");
}

void setupHorizontalDisplacementFunction(void) {
	int i;
	Displacement *ptr;
	for (i = 0; i < DISPLACEMENT_LENGTH; i++) {
		ptr = &verticalDisplacementFunctionUp[i];
		makeUp(ptr);
		ptr->next = &verticalDisplacementFunctionUp[i + 1];

		ptr = &verticalDisplacementFunctionDown[i];
		makeUp(ptr);
		ptr-> next = &verticalDisplacementFunctionDown[i + 1];
	}
	ptr = &verticalDisplacementFunctionUp[DISPLACEMENT_LENGTH];
	ptr->next = verticalDisplacementFunctionUp;
	ptr = &verticalDisplacementFunctionDown[DISPLACEMENT_LENGTH];
	ptr->next = verticalDisplacementFunctionDown;
}

void setupCircularDisplacementFunction(void) {
	int i = 0;
	Displacement *ptr;
	for (i = 0; i < DISPLACEMENT_LENGTH; i++) {
		ptr = &circularDisplacementFunction[i];
		if (i < DISPLACEMENT_LENGTH / 4) {
			makeLeft(ptr);
		} else if (i < DISPLACEMENT_LENGTH / 2) {
			makeDown(ptr);
		} else if (i < 3 * DISPLACEMENT_LENGTH / 4) {
			makeRight(ptr);
		} else {
			makeUp(ptr);
		}
		ptr->next = &circularDisplacementFunction[i + 1];
	}
	ptr = &circularDisplacementFunction[DISPLACEMENT_LENGTH];
	ptr->next = circularDisplacementFunction;
}

void setupArcDisplacementFunction(void) {
	int i;
	Displacement *ptr;
	for (i = 0; i < DISPLACEMENT_LENGTH; i++) {
		ptr = &arcDisplacementFunction[i];
		if (i < DISPLACEMENT_LENGTH / 2) {
			makeDownLeft(ptr);
		} else {
			makeDownRight(ptr);
		}
		ptr->next = &arcDisplacementFunction[i + 1];
	}
	ptr = &arcDisplacementFunction[DISPLACEMENT_LENGTH];
	ptr->next = arcDisplacementFunction;
}

void makeRight(Displacement *disp) {
	disp->dx = right.dx;
	disp->dy = right.dy;
}
void makeLeft(Displacement *disp) {
	disp->dx = left.dx;
	disp->dy = left.dy;
}
void makeUp(Displacement *disp) {
	disp->dx = up.dx;
	disp->dy = up.dy;
}

void makeDown(Displacement *disp) {
	disp->dx = down.dx;
	disp->dy = down.dy;
}

void makeDownRight(Displacement *disp) {
	disp->dx = right.dx;
	disp->dy = down.dy;
}

void makeDownLeft(Displacement *disp) {
	disp->dx = left.dx;
	disp->dy = down.dy;
}

void makeUpRight(Displacement *disp) {
	disp->dx = right.dx;
	disp->dy = up.dy;
}

void makeUpLeft(Displacement *disp) {
	disp->dx = left.dx;
	disp->dy = up.dy;
}

void setToRandomDisplacement(Shark *shark) {
	srand(time(NULL));
	int r = rand();
	short int select = r % 4;
	Displacement *ptr;
	switch (select) {
	case 0:
		ptr = verticalDisplacementFunctionUp;
		break;
	case 1:
		ptr = verticalDisplacementFunctionDown;
		break;
	case 2:
		ptr = circularDisplacementFunction;
		break;
	case 3:
		ptr = arcDisplacementFunction;
		break;
	}
	shark->displacement = ptr;
}
