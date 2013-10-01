#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"
#include "timer.h"
#include "audio.h"
#include "sd_card.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include "alt_types.h"
#include "sys/alt_irq.h"

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void playLaserInterrupt(void* isr_context);
#else
static void playLaserInterrupt(void* isr_context, alt_u32 id);
#endif

//#include "audio_up_hack.h"

const char *CONFIG_NAME = "/dev/audio_and_video_config_0"; // may need to change to verilog version

const char *AUDIO_NAME = "/dev/audio_0"; // may need to change to verilog version
alt_up_audio_dev *audio = NULL;

alt_up_av_config_dev *config = NULL;

int DEBUG = 0;
int toneLength = 122;
unsigned int tone[122];

unsigned int *laserBuffer;
volatile unsigned int *laserCursor;
unsigned int laserBufferLength;
unsigned int laserFileWordLength;

volatile int somethingForIrq;

void setupAudio()
{
	bool error = false;
	config = (alt_up_av_config_dev *)alt_up_av_config_open_dev(CONFIG_NAME);
	if (config == NULL) {
		printf("Error: audio video config could not be opened.\n");
		error = true;
	}
	while (!alt_up_av_config_read_ready(config)) {
	}

	audio = (alt_up_audio_dev *)alt_up_audio_open_dev(AUDIO_NAME);
	if (config == NULL) {
		printf("Error: audio codec could not be opened.\n");
		error = true;
	} else if (DEBUG == 1) {
		printf("Successfully opened audio codec.\n");
	}

	int amp = 10000000;
	//printf("Amp is %d\n", amp);
	int i;
	for (i = 0; i < toneLength; i++) {
		tone[i] = amp * sin((float)i * 3.141592 / 122.0);//* (PI / 8.0));
    }
    int interruptStatus = setupAudioInterrupt(audio, somethingForIrq);

    if (interruptStatus < 0) {
		printf("Error: audio interrupt could not be setup.\n");
		error = true;
	} else if (DEBUG == 1) {
		printf("Successfully setup audio interrupts.\n");
	}

	laserFileWordLength = 38384;//laserii//(getWavFileLength("laseri.wav") / 2);// laser i = 26200;/*looked in hex file *///
	printf("File Length is: %x\n", laserFileWordLength);
	readWavFile("laserii.wav", laserFileWordLength, laserBuffer);

	if (DEBUG == 1 && error == false) {
		printf("Successfully setup sound.\n");
	}
}

int setupAudioInterrupt(alt_up_audio_dev **audio, volatile int somethingForIrq)
{
    // Need to disable both audio interrupts before setting them up
    // otherwise you get stuck in them when they are setup
    alt_up_audio_disable_read_interrupt(audio);
    alt_up_audio_disable_write_interrupt(audio);

    void *irqInt = (void*)&somethingForIrq;

    return alt_ic_isr_register(AUDIO_0_IRQ_INTERRUPT_CONTROLLER_ID, AUDIO_0_IRQ, playLaserInterrupt, irqInt, 0x0);
}

void playAudioMono(unsigned int *buffer, int length) {
	alt_up_audio_write_fifo(audio, buffer, length, ALT_UP_AUDIO_LEFT);
	alt_up_audio_write_fifo(audio, buffer, length, ALT_UP_AUDIO_RIGHT);
}

void playAudio(unsigned int *leftBuffer, int leftLength, unsigned int *rightBuffer, int rightLength) {
	int leftWritten = alt_up_audio_write_fifo(audio, leftBuffer, leftLength, ALT_UP_AUDIO_LEFT);
	int rightWritten = alt_up_audio_write_fifo(audio, rightBuffer, rightLength, ALT_UP_AUDIO_RIGHT);

	if (DEBUG == 1) {
		printf("Wrote %d to left audio FIFO. with value\n", leftWritten);
		printf("Wrote %d to right audio FIFO.\n", rightWritten);
	}
}

void audioTest()
{
	setupAudio();
	printf("Starting audio test\n");
	int length = 8;
	unsigned int right [length];
	unsigned int left [length];
	int i;

	for (i = 0; i < length; i++) {
		right[i] = (i * i * i) & 0x0000FFFF;
		left[i] = right[i];
	}

	unsigned int *rightBuffer = right;
	unsigned int *leftBuffer = left;

	//sin (param*PI/180); for degrees, it uses radians

	int free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
	printf("%d words free in right FIFO\n", free);

	for (;;) {
		free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
		//printf("%d words free in right FIFO\n", free);
		if (free >= length) {
			//printf("Playing audio\n");
			playAudio(leftBuffer, length, rightBuffer, length);
		}
	}
}


void testTone(void)
{
	unsigned int *toneBuffer = tone;
	//int toneLen = 360;
	int free, len;
	int wrap = 0;

	for (;;) {
		free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
		if (free > 1) {
			if ((int)toneBuffer + free >= toneLength) {
				// Wrap around
				len = toneLength - free;
				wrap = 1;
			} else {
				len = free;
				wrap = 0;
			}
			playAudio(toneBuffer, len, toneBuffer, len);
			toneBuffer = wrap == 1 ? tone : toneBuffer + len;
		}
	}
}

void playLaser1(void) {
	unsigned int fileWordLength = 38384;//laserii//(getWavFileLength("laseri.wav") / 2);// laser i = 26200;/*looked in hex file *///
	printf("File Length is: %x\n", fileWordLength);
	readWavFile("laserii.wav", fileWordLength, laserBuffer);

	int free, len;
	int wrap = 0;
	unsigned int *cursor = laserBuffer;
	for (;;) {
		free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
		if (free > 1) {
			if ((int)cursor + free >= (int)laserBuffer + (2 * fileWordLength)) {
				// Wrap around
				len = fileWordLength - free;
				wrap = 1;
			} else {
				len = free;
				wrap = 0;
			}
			playAudio(cursor, len, cursor, len);
			cursor = wrap == 1 ? laserBuffer : cursor + len;
		}
	}
}

void readWavFile(char *wavFileName, unsigned int fileWordLength, unsigned int *buffer) {
	laserBuffer = malloc(fileWordLength * 2); //words are 2 bytes // this line should be changed

	short int fileHandle = openFile(wavFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", wavFileName);
		return;
	}

	readPastWavHeader(fileHandle);

	unsigned int i = 0;
	unsigned int word = readWord(fileHandle);
	printf("first word is %x\n", word);
	while (i < fileWordLength) {
		laserBuffer[i++] = word;
		word = readWord(fileHandle);
	}
	printf("reached EOF\n");

	closeFile(fileHandle);
	return;
}

// Plays laser once, using interrupts
void playLaser(void) {
	if (DEBUG == 1) {
		printf("Playing laser via interrupt.\n");
	}

	laserCursor = laserBuffer;
	alt_up_audio_enable_write_interrupt(audio);
}

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void playLaserInterrupt(void* isr_context) {
#else
static void playLaserInterrupt(void* isr_context, alt_u32 id) {
#endif
	int len;
	unsigned int free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
	if (free > 1) {
		if ((int)laserCursor + free >= (int)laserBuffer + (2 * laserFileWordLength)) {
			// Last chunk to play
			len = laserFileWordLength - free;
			alt_up_audio_disable_write_interrupt(audio);
		} else {
			len = free;
		}
		playAudioMono((unsigned int *)laserCursor, len);
		laserCursor += len;
	}
}
