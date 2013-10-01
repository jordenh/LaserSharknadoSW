#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

alt_up_pixel_buffer_dma_dev* setUp(void);
void clearScreen(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void drawLine(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x0, int y0, int x1, int y1, int color);
void printLine(void);
void drawBox(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x0, int y0, int x1, int y1, int color);

#endif
