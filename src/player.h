#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "vga.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

#define PLAYER_WIDTH 37
#define PLAYER_HEIGHT 20

typedef struct {
	int x;
	int y;
} Player;

Player player;

void initPlayer();
void drawPlayer();
void erasePlayer();
void moveUpPlayer();
void moveDownPlayer();

#endif
