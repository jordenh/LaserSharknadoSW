#ifndef __SD_CARD_H__
#define __SD_CARD_H__

#include "Altera_UP_SD_Card_Avalon_Interface.h"

alt_up_sd_card_dev *sdDev;

int openSdCard();
short int openFile(char *fileName);
int closeFile(short int fh);
unsigned char readByte(short int fh);
short int readWord(short int fh);
int readDWord(short int fh);
unsigned int getWavFileLength(char *fileName);
void readPastWavHeader(short int handle);

#endif
