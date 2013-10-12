#include "nado.h"

int nadoDrawX = SCREEN_WIDTH - NADO_WIDTH - 24;
int nadoDrawY = 0;

void initNado() {
}

void drawNado(BMP *nado) {
	drawBmp(nado, nadoDrawX, nadoDrawY);
}

void drawCommonNado(void) {
	drawNado(cnadoBmp);
}

void drawANado(void) {
	drawNado(pnadoaBmp);
}

void eraseANado(void) {
	drawNado(nnadoaBmp);
}

void drawBNado(void) {
	drawNado(pnadobBmp);
}

void eraseBNado(void) {
	drawNado(nnadobBmp);
}
