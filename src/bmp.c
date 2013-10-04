#include "bmp.h"

void parseBmp (char *fileName, BMP *bmp) {
	int i, j, k;
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
	bmp->rgb = malloc(BYTES_PER_PIXEL * pixels);
	printf("size of bmp->rgb %x", bmp->rgb);

	for(i = 0; i < bmp->infoheader.height; i++) {
		rowOffset = i*bmp->infoheader.width;
		for(j = 0; j < bmp->infoheader.width; j++ ){
			offset = pixels - rowOffset - j - 1;

			(bmp->rgb + offset)->b = (readByte(fh) & 0xF1) >> 3;
			(bmp->rgb + offset)->g = (readByte(fh) & 0xFC) >> 2;
			(bmp->rgb + offset)->r = (readByte(fh) & 0xF1) >> 3;

			//Filter out the pink pixels
			if (((bmp->rgb + offset)->b == 0x1E) && ((bmp->rgb + offset)->g == 0) && ((bmp->rgb + offset)->r == 0x1E)) {
				(bmp->rgb + offset)->b = 0x0;
				(bmp->rgb + offset)->r = 0x0;
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
	sharkBmp = malloc(sizeof(BMP));
	playerBmp = malloc(sizeof(BMP));

	//parseBmp("splash.bmp", splashBmp);
	parseBmp("shark.bmp", sharkBmp);
	parseBmp("player.bmp", playerBmp);
}

void freeBmps(){
	free(splashBmp->rgb);
	free(playerBmp->rgb);
	free(sharkBmp->rgb);

	free(splashBmp);
	free(sharkBmp);
	free(playerBmp);
}

void drawBmp (BMP *bmp, int x, int y) {
	int i,j;
	int color;

	for(i = 0; i < bmp->infoheader.height; i++) {
		for(j = 0; j < bmp->infoheader.width; j++){
			color = ((bmp->rgb + i*bmp->infoheader.width +j)->r << 11) | ((bmp->rgb + i*bmp->infoheader.width +j)->g << 5) | (bmp->rgb + i*bmp->infoheader.width +j)->b;
			drawPixel(x + j, y + i, color);
		}
	}
}

void eraseBmp (BMP *bmp, int x, int y) {
	int i, j;

	for(i = 0; i < bmp->infoheader.height; i++) {
			for(j = 0; j < bmp->infoheader.width; j++){
				drawPixel(x + j, y + i, 0);
			}
	}
}
