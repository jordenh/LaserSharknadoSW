#ifndef VGA_H
#define VGA_H

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 240

alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev *char_buffer;

void setupVga();
void clearScreen();
void drawLine(int x0, int y0, int x1, int y1, int color);
void printLine();
void drawBox(int x0, int y0, int x1, int y1, int color);

#endif
