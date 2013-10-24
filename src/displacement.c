#include "displacement.h"

Displacement verticalDisplacementFunctionUp[DISPLACEMENT_LENGTH];
Displacement verticalDisplacementFunctionDown[DISPLACEMENT_LENGTH];
Displacement circularDisplacementFunction[DISPLACEMENT_LENGTH];
Displacement arcDisplacementFunction[DISPLACEMENT_LENGTH];
Displacement doNotMove[DISPLACEMENT_LENGTH];

static Displacement right;
static Displacement left;
static Displacement up;
static Displacement down;

//Purpose: Set up the shark displacements so that they can be used
void setupDisplacement(void) {
	right.dx = 1;
	right.dy = 0;

	left.dx = -1;
	left.dy = 0;

	up.dx = 0;
	up.dy = -1;

	down.dx = 0;
	down.dy = 1;

	setupVerticalDisplacementFunction();
	setupCircularDisplacementFunction();
	setupArcDisplacementFunction();
	setupNoDisplcementFunction();

	printf("Setup displacement functions.\n");
}

void setupVerticalDisplacementFunction(void) {
	int i;
	Displacement *ptr;
	for (i = 0; i < DISPLACEMENT_LENGTH; i++) {
		ptr = &verticalDisplacementFunctionUp[i];
		makeUp(ptr);
		ptr->next = &verticalDisplacementFunctionUp[i + 1];

		ptr = &verticalDisplacementFunctionDown[i];
		makeDown(ptr);
		ptr-> next = &verticalDisplacementFunctionDown[i + 1];
	}
	ptr = &verticalDisplacementFunctionUp[DISPLACEMENT_LENGTH - 1];
	ptr->next = verticalDisplacementFunctionDown;
	ptr = &verticalDisplacementFunctionDown[DISPLACEMENT_LENGTH - 1];
	ptr->next = verticalDisplacementFunctionUp;
}

void setupCircularDisplacementFunction(void) {
	int i = 0;
	Displacement *ptr;
	for (i = 0; i < DISPLACEMENT_LENGTH; i++) {
		ptr = &circularDisplacementFunction[i];
		if (i < DISPLACEMENT_LENGTH / 4) {
			// step 1: the sharks starts off going right
			makeRight(ptr);
		} else if (i < DISPLACEMENT_LENGTH / 2) {
			// step 2: the sharks goes down
			makeDown(ptr);
		} else if (i < 3 * DISPLACEMENT_LENGTH / 4) {
			// step 3: the sharks goes left
			makeLeft(ptr);
		} else {
			// step 4: the sharks goes up
			makeUp(ptr);
		}
		ptr->next = &circularDisplacementFunction[i + 1];
	}
	ptr = &circularDisplacementFunction[DISPLACEMENT_LENGTH - 1];
	ptr->next = circularDisplacementFunction;
}

void setupArcDisplacementFunction(void) {
	int i;
	Displacement *ptr;
	for (i = 0; i < DISPLACEMENT_LENGTH; i++) {
		ptr = &arcDisplacementFunction[i];
		if (i < DISPLACEMENT_LENGTH / 4) {
			makeDownRight(ptr);
		} else if (i < DISPLACEMENT_LENGTH / 2) {
			makeDownLeft(ptr);
		} else if (i < 3 * DISPLACEMENT_LENGTH / 4) {
			makeUpRight(ptr);
		} else {
			makeUpLeft(ptr);
		}
		ptr->next = &arcDisplacementFunction[i + 1];
	}
	ptr = &arcDisplacementFunction[DISPLACEMENT_LENGTH - 1];
	ptr->next = arcDisplacementFunction;
}

void setupNoDisplcementFunction(void) {
	int i;
	for (i = 0; i < DISPLACEMENT_LENGTH; i++) {
		doNotMove[i].dx = 0;
		doNotMove[i].dy = 0;
		doNotMove[i].next = doNotMove;
	}
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
