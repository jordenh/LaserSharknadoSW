#include "vga.h"

void setupVga()
{
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, PIXEL_BUFFER_BASE);

	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

	while
		(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	clearScreen(pixel_buffer);

	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);
}

void clearScreen()
{
	printf("Clearing Screen!\n");

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
}

void drawLine(int x0, int y0, int x1, int y1, int color)
{
	/*if (y0 == y1)
	{
		alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, x0, x1, y0, color, 0);
	}
	else if (x0 == x1)
	{
		alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, x0, y0, y1, color, 0);
	}
	else
	{*/
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x0, y0, x1, y1, color, 0);
	//}
}

void printLine(void)
{
	alt_up_char_buffer_string(char_buffer, "LaserSharknado", 40, 30);
}

void drawBox(int x0, int y0, int x1, int y1, int color)
{
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0, y0, x1, y1, color, 0);
}
