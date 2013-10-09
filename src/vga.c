#include "vga.h"

void initVga()
{
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (320 * 240 * 2);

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	// Set the 2nd buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	clearScreen();
}

void clearScreen()
{
	alt_up_char_buffer_clear(char_buffer);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);
}

void drawPixel(int x, int y, int color) {
	alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
}

void drawLine(int x0, int y0, int x1, int y1, int color)
{
	if (y0 == y1)
	{
		alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, x0, x1, y0, color, 1);
	}
	else if (x0 == x1)
	{
		alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, x0, y0, y1, color, 1);
	}
	else
	{
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x0, y0, x1, y1, color, 1);
	}
}

void printLine(void)
{
	alt_up_char_buffer_string(char_buffer, "LaserSharknado", 40, 30);
}

void drawBox(int x0, int y0, int x1, int y1, int color)
{
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0, y0, x1, y1, color, 1);
}

int convert24BitRgbTo16(unsigned int rgb24bit)
{
	unsigned int R8bit = (rgb24bit >> 16) & 0xFF;
	unsigned int G8bit = (rgb24bit >> 8)  & 0xFF;
	unsigned int B8bit = (rgb24bit)	      & 0xFF;
	unsigned int R5bit = R8bit >> 3 & 0x1F;
	unsigned int G6bit = G8bit >> 2 & 0x3F;
	unsigned int B5bit = B8bit >> 3 & 0x1F;
	return (R5bit << 11 | G6bit << 5 | B5bit);
}
