#ifndef __SHARK_H__
#define __SHARK_H__

#include "bmp.h"

#define SHARK_LENGTH 37
#define SHARK_HEIGHT 20

typedef struct {
	int x;
	int y;
} Shark;

void drawShark(int x, int y);
void moveShark();

#endif
