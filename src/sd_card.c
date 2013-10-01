#include <stdio.h>
#include <stdlib.h>
#include "sd_card.h"

int openSdCard(){
	sdDev = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

	if(sdDev != NULL && alt_up_sd_card_is_FAT16()){
		return 0;
	} else {
		return -1;
	}
}

short int openFile(char *fileName) {
	return alt_up_sd_card_fopen(fileName, 0);
}

int closeFile(short int fh) {
	return alt_up_sd_card_fclose(fh);
}

unsigned char readByte(short int fh) {
	return (unsigned char) alt_up_sd_card_read(fh);
}
short int readWord(short int fh) {
	short int byte1, byte2;

	byte1 = alt_up_sd_card_read(fh);
	byte2 = alt_up_sd_card_read(fh);

	if (byte1 == -1 || byte2 == -1)
		return -1;

	return ((unsigned short int)byte2 << 8) | ((unsigned short int)byte1 & 0x00FF);
}

int readDWord(short int fh) {
	short int byte1, byte2, byte3, byte4;

	byte1 = alt_up_sd_card_read(fh);
	byte2 = alt_up_sd_card_read(fh);
	byte3 = alt_up_sd_card_read(fh);
	byte4 = alt_up_sd_card_read(fh);

	if (byte1 == -1 || byte2 == -1 || byte3 == -1 || byte4 == -1)
			return -1;

	return ((unsigned short int)byte4 << 24) | ((unsigned short int)byte3 << 16) | ((unsigned short int)byte2 << 8) | (unsigned short int)byte1;
}

unsigned int getWavFileLength(char *fileName) { // TBD: This function MUST be fixed - it currently returns an incorrect value, becuase - vals dont always mean it's done.
	unsigned int fileLength = 0;

	short int fileHandle = openFile(fileName);
	if (fileHandle == -1) {
		printf("Error occurred, unable to open file in 'getFileLength' with name: %s", fileName);
	}

	readPastWavHeader(fileHandle); // to get wav file length, dont bypass header i think..

	short int wordRead = readWord(fileHandle);
	//unsigned char firstByte = 0x0000FFFF | wordRead;
	//unsigned char secondByte = 0x0000FFFF | (wordRead >> 8);
	while ((short int)wordRead >= 0) {
		//printf("%c", (unsigned char)byteRead);
		fileLength += 2;
		wordRead = readWord(fileHandle);
	}
	if ((short int)wordRead <= -1) {
		printf("Error reading bytes from %s\n", fileName);
	}

	closeFile(fileHandle);
	return fileLength;
}

void readPastWavHeader(short int handle) {
	int i;
	for (i = 0; i < 44; i++) {
		readByte(handle);
	}
}
