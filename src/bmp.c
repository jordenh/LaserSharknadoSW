#include "bmp.h"

void parseBmp (char *fileName, BMP *bmp) {
	int i, j, k;
	char b, g, r;
	int pixels, rowOffset, offset;
	short int fh;

	fh = openFile(fileName);

	//Parse BMP header
	bmp->header.type = readWord(fh);
	bmp->header.size = readDWord(fh);
	bmp->header.reserved1 = readWord(fh);
	bmp->header.reserved2 = readWord(fh);
	bmp->header.offset = readDWord(fh);

	//Parse BMP info header
	bmp->infoheader.size = readDWord(fh);
	bmp->infoheader.width = readDWord(fh);
	bmp->infoheader.height = readDWord(fh);
	bmp->infoheader.planes = readWord(fh);
	bmp->infoheader.bits = readWord(fh);
	bmp->infoheader.compression = readDWord(fh);
	bmp->infoheader.imagesize = readDWord(fh);
	bmp->infoheader.xresolution = readDWord(fh);
	bmp->infoheader.yresolution = readDWord(fh);
	bmp->infoheader.ncolors = readDWord(fh);
	bmp->infoheader.importantcolors = readDWord(fh);

	pixels = bmp->infoheader.width * bmp->infoheader.height;
	bmp->color = malloc(BYTES_PER_PIXEL * pixels);

	for(i = 0; i < bmp->infoheader.height; i++) {
		rowOffset = i*bmp->infoheader.width;
		for(j = 0; j < bmp->infoheader.width; j++ ){
			offset = pixels - rowOffset - j - 1;

			/*
			 *RGB values are stored in reverse (BGR) in the .bmp file
			 *BMP is saved as 24 bit RGB, pixel buffer dma takes 16 bit RGB values
			 */
			b = (readByte(fh) & 0xF1) >> 3;
			g = (readByte(fh) & 0xFC) >> 2;
			r = (readByte(fh) & 0xF1) >> 3;

			//Filter out the pink pixels
			if(b == 0x1E && g == 0 && r == 0x1E) {
				bmp->color[offset] = 0x0;
			} else {
				bmp->color[offset] = (r << 11) | (g << 5) | b;
			}
		}

		//Each pixel row is padded so the amount of pixels is a multiple of 4
		if((BYTES_PER_PIXEL*bmp->infoheader.width) % 4 != 0) {
			for (k = 0; k <  (4 - ((BYTES_PER_PIXEL*bmp->infoheader.width) % 4)); k++) {
				readByte(fh);
			}
		}
	}

	closeFile(fh);
}

void parseBmps() {
	splashBmp = malloc(sizeof(BMP));
	loadBmp = malloc(sizeof(BMP));
	pressBmp = malloc(sizeof(BMP));
	sharkBmp = malloc(sizeof(BMP));
	playerBmp = malloc(sizeof(BMP));

	cnadoBmp = malloc(sizeof(BMP));
	pnadoaBmp = malloc(sizeof(BMP));
	pnadobBmp = malloc(sizeof(BMP));

	parseBmp("splash.bmp", splashBmp);
	parseBmp("loadtx.bmp", loadBmp);
	parseBmp("press.bmp", pressBmp);
	parseBmp("shark.bmp", sharkBmp);
	parseBmp("player.bmp", playerBmp);

	parseBmp("cnado.bmp", cnadoBmp);
	parseBmp("pnadoa.bmp", pnadoaBmp);
	parseBmp("pnadob.bmp", pnadobBmp);
}

void freeBmps(){
	free(playerBmp->color);
	free(sharkBmp->color);

	free(sharkBmp);
	free(playerBmp);
}

void freeSplash() {
	free(splashBmp->color);
	free(splashBmp);
}

void drawBmp (BMP *bmp, int x, int y) {
	int i,j;
	int offset;

	for(i = 0; i < bmp->infoheader.height; i++) {
		// drawPixelFast does not check bounds, instead bounds are checked in drawBmp
		if(y + i < SCREEN_HEIGHT && y + i > 0) {
			offset = i * bmp->infoheader.width;

			for(j = 0; j < bmp->infoheader.width; j++){
				if(bmp->color[offset + j] == 0 || x + j >= SCREEN_WIDTH || x + j <= 0)
					continue;

				drawPixelFast(x + j, y + i, bmp->color[offset +j]);
			}
		}
	}
}

void eraseBmp (BMP *bmp, int x, int y) {
	int i,j;
	int offset;

	for(i = 0; i < bmp->infoheader.height; i++) {
		if(y + i < SCREEN_HEIGHT && y + i > 0) {
			offset = i * bmp->infoheader.width;

			for(j = 0; j < bmp->infoheader.width; j++){
				if(x + j >= SCREEN_WIDTH || x + j <= 0)
					continue;

				drawPixelFast(x + j, y + i, 0);
			}
		}
	}
}
