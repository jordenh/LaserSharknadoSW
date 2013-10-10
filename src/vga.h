#ifndef VGA_H
#define VGA_H

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 240

alt_up_pixel_buffer_dma_dev* pixel_buffer;
unsigned int pixel_buffer_addr1;
unsigned int pixel_buffer_addr2;
alt_up_char_buffer_dev *char_buffer;

void initVga();
void clearScreen();
void drawPixel(int x, int y, int color);
void drawPixelFast(unsigned int x, unsigned int y, unsigned int color);
void drawLine(int x0, int y0, int x1, int y1, int color);
void printLine();
void drawBox(int x0, int y0, int x1, int y1, int color);
int convert24BitRgbTo16(unsigned int rgb24bit);

#endif
