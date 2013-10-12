#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"
#include "timer.h"
#include "sd_card.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include "alt_types.h"
#include "sys/alt_irq.h"

#define PI 3.14159265359
#define NONE 0
#define LASER 1
#define PLAYER_DEATH 2
#define SHARK_DEATH 3
#define THEME 4
#define FAIL -1
#define PLAYBUFFER_LEN 128

struct audioInfo{
	unsigned int *mainBuffer;
	unsigned int *volumeBuffer;
	unsigned int bufferLength;
	unsigned int playedWords;
	bool active;
	volatile unsigned int *playCursor;
};

void setupAudio(void);
void audioTest(void);
void runAudioDiagnostics(void);
void testTone(void);
void playLaser1(void);
void readWavFile(char *wavFileName, unsigned int fileWordLength);
void playAudioMono(int length);
void playLaser(void);
void playPlayerDeath(void);
void playSharkDeath(void);
void playTheme(void);
void playPlayerDeath(void);
unsigned int **getActiveBuffer(void);
int getActiveBufferLength(void);
int getActiveFileWordLength(void);
int setupAudioInterrupt(alt_up_audio_dev *audio, volatile int somethingForIrq);
void playAudio(unsigned int *leftBuffer, int leftLength, unsigned int *rightBuffer, int rightLength);
void loadLaser(void);
void loadPlayerDeath(void);
void loadSharkDeath(void);
void loadTheme(void);
void updateAudioWithVolume(char switchValues);
void changeBufferVolume(struct audioInfo, char switchValues);
void stopTheme(void);

#endif
