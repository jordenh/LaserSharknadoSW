#include "bmp.h"

void parseBmp (char *fileName, BMP *bmp) {
	int i, j, k;
	char b, g, r;
	int pixels, rowOffset, offset;
	short int fh;

	fh = openFile(fileName);

	bmp->header.type = readWord(fh);
	bmp->header.size = readDWord(fh);
	bmp->header.reserved1 = readWord(fh);
	bmp->header.reserved2 = readWord(fh);
	bmp->header.offset = readDWord(fh);

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

	parseBmp("splash.bmp", splashBmp);
	parseBmp("loadtx.bmp", loadBmp);
	parseBmp("press.bmp", pressBmp);
	parseBmp("shark.bmp", sharkBmp);
	parseBmp("player.bmp", playerBmp);
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
		offset = i * bmp->infoheader.width;
		for(j = 0; j < bmp->infoheader.width; j++){
			if(bmp->color[offset + j] == 0 || x + j >= SCREEN_WIDTH || y + i >= SCREEN_HEIGHT || x + j <= 0 || y + i <= 0)
				continue;

			if(x + j <= 0 || y + i <= 0){
				printf("in drawBmp, should not be here...\n");
			}

			drawPixelFast(x + j, y + i, bmp->color[offset +j]);
		}
	}
}

void eraseBmp (BMP *bmp, int x, int y) {
	drawBox(x, y, x  + bmp->infoheader.width, y + bmp->infoheader.height, 0);
}
