#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "vga.h"
#include "bmp.h"
#include "score.h"

#define PLAYER_WIDTH 28
#define PLAYER_HEIGHT 27

typedef struct {
	int x, prevX;
	int y, prevY;
} Player;

Player player;

void initPlayer();
void drawPlayer();
void erasePlayer();
void moveUpPlayer();
void moveDownPlayer();
void keepPlayerStationary();
void hitPlayer();
void killPlayer();

#endif
