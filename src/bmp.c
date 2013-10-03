#include "bmp.h"

BMP* parseBmp (char *fileName) {
	int i, j, k;
	int pixels, rowOffset, offset;
	short int fh;
	BMP *bmp;

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

	for(i = 0; i < bmp->infoheader.height; i++) {
		rowOffset = i*bmp->infoheader.width;
		for(j = 0; j < bmp->infoheader.width; j++ ){
			offset = pixels - rowOffset - j - 1;

			(bmp->rgb + offset)->b = (readByte(fh) & 0xF1) >> 3;
			(bmp->rgb + offset)->g = (readByte(fh) & 0xFC) >> 2;
			(bmp->rgb + offset)->r = (readByte(fh) & 0xF1) >> 3;
		}

		if((BYTES_PER_PIXEL*bmp->infoheader.width) % 4 != 0) {
			for (k = 0; k <  (4 - ((BYTES_PER_PIXEL*bmp->infoheader.width) % 4)); k++) {
				readByte(fh);
			}
		}
	}

	closeFile(fh);

	return bmp;
}

int drawBmp (char *fileName, int x, int y) {
	int i,j;
	int color;
	BMP *bmp = parseBmp(fileName);

	for(i = 0; i < bmp->infoheader.height; i++) {
		for(j = 0; j < bmp->infoheader.width; j++){
			color = ((bmp->rgb + i*bmp->infoheader.width +j)->r << 11) | ((bmp->rgb + i*bmp->infoheader.width +j)->g << 5) | (bmp->rgb + i*bmp->infoheader.width +j)->b;
			drawLine(x + j, y + i, x + j, y + i, color);
		}
	}

	return 0;
}
