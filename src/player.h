#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "vga.h"
#include "bmp.h"

#define PLAYER_WIDTH 37
#define PLAYER_HEIGHT 20

typedef struct {
	int x, prevX;
	int y, prevY;
} Player;

Player *player;

void initPlayer();
void drawPlayer();
void erasePlayer();
void moveUp();
void moveDown();

#endif
