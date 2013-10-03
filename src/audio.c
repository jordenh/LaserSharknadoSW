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
static void playSoundISR(void* isr_context);
#else
static void playSoundISR(void* isr_context, alt_u32 id);
#endif

//#include "audio_up_hack.h"

const char *CONFIG_NAME = "/dev/audio_and_video_config_0"; // may need to change to verilog version

const char *AUDIO_NAME = "/dev/audio_0"; // may need to change to verilog version
alt_up_audio_dev *audio = NULL;

alt_up_av_config_dev *config = NULL;

int DEBUG = 0;
int toneLength = 122;
unsigned int tone[122];

volatile unsigned int *playCursor;
volatile short int status;

unsigned int *laserBuffer;
unsigned int laserBufferLength;
unsigned int laserFileWordLength;

unsigned int *playerDeathBuffer;
unsigned int playerDeathBufferLength;
unsigned int playerDeathFileWordLength;

unsigned int *sharkDeathBuffer;
unsigned int sharkDeathBufferLength;
unsigned int sharkDeathFileWordLength;

unsigned int *themeBuffer;
unsigned int themeBufferLength;
unsigned int themeFileWordLength;

volatile int somethingForIrq;

unsigned int **getActiveBuffer(void);
int getActiveBufferLength(void);
int getActiveFileWordLength(void);
int setupAudioInterrupt(alt_up_audio_dev *audio, volatile int somethingForIrq);
void playAudio(unsigned int *leftBuffer, int leftLength, unsigned int *rightBuffer, int rightLength);

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

	laserFileWordLength = 38384;
	printf("File Length is: %x\n", laserFileWordLength);
	unsigned int **ptrToLaserBuffer = &laserBuffer;
	readWavFile("laserii.wav", laserFileWordLength, ptrToLaserBuffer);

	// Need to setup playerDeathSound
	//playerDeathFileWordLength = 0x0000DAFF/ 2;
	//unsigned int **ptrToPlayerDeathBuffer = &playerDeathBuffer; // initialize other buffers yo?
	//readWavFile("pdie.wav", playerDeathFileWordLength, ptrToPlayerDeathBuffer);

	// Need to setup sharkDeathSound
	//sharkDeathFileWordLength = 0x00006FFF / 2;
	//unsigned int **ptrToSharkDeathBuffer = &sharkDeathBuffer;
	//readWavFile("sdie.wav", sharkDeathFileWordLength, ptrToSharkDeathBuffer);

	//themeFileWordLength = 0x00063E00 / 2;
	//unsigned int **ptrToThemeBuffer = &themeBuffer;
	//readWavFile("theme.wav", themeFileWordLength, ptrToThemeBuffer);

	status = NONE;

	if (DEBUG == 1 && error == false) {
		printf("Successfully setup sound.\n");
	}
}

int setupAudioInterrupt(alt_up_audio_dev *audio, volatile int somethingForIrq)
{
    // Need to disable both audio interrupts before setting them up
    // otherwise you get stuck in them when they are setup
    alt_up_audio_disable_read_interrupt(audio);
    alt_up_audio_disable_write_interrupt(audio);

    void *irqInt = (void*)&somethingForIrq;

	#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
	return alt_ic_isr_register(AUDIO_0_IRQ_INTERRUPT_CONTROLLER_ID, AUDIO_0_IRQ, playSoundISR, irqInt, 0x0);
	#else
	return alt_irq_register(AUDIO_0_IRQ, irqInt, playSoundISR);
	#endif
}

void playAudioMono(int length) {
	if (DEBUG == 1) {
		printf("Playing audio.\n");
	}
	int left = alt_up_audio_write_fifo(audio, laserBuffer, length, ALT_UP_AUDIO_LEFT);
	int right = alt_up_audio_write_fifo(audio, laserBuffer, length, ALT_UP_AUDIO_RIGHT);
	left++;
	right++;
	//playAudio(buffer, length, buffer, length);
}

void playAudio(unsigned int *leftBuffer, int leftLength, unsigned int *rightBuffer, int rightLength) {
	int leftWritten = alt_up_audio_write_fifo(audio, leftBuffer, leftLength, ALT_UP_AUDIO_LEFT);
	if (DEBUG == 1) {
		printf("Wrote %d to left audio FIFO. with value\n", leftWritten);
	}

	int rightWritten = alt_up_audio_write_fifo(audio, rightBuffer, rightLength, ALT_UP_AUDIO_RIGHT);
	if (DEBUG == 1) {
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

void readWavFile(char *wavFileName, unsigned int fileWordLength, unsigned int **buffer) {
	unsigned int *tempBuffer = malloc(fileWordLength * 2);
	if (tempBuffer == NULL) {
		printf("Error: insufficient memory to load audio file into memory.\n");
	}

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
		tempBuffer[i++] = word;
		word = readWord(fileHandle);
	}
	printf("reached EOF\n");

	closeFile(fileHandle);
	(*buffer) = tempBuffer;
	return;
}

// Plays laser once, using interrupts
void playLaser(void) {
	if (DEBUG == 1) {
		printf("Playing laser via interrupt.\n");
	}

	status = LASER;
	playCursor = laserBuffer;
	alt_up_audio_enable_write_interrupt(audio);
}

void playPlayerDeath(void) {
	status = PLAYER_DEATH;
	playCursor = playerDeathBuffer;
	alt_up_audio_enable_write_interrupt(audio);
}

void playSharkDeath(void) {
	status = SHARK_DEATH;
	playCursor = sharkDeathBuffer;
	alt_up_audio_enable_write_interrupt(audio);
}

void playTheme(void) {
	status = THEME;
	playCursor = themeBuffer;
	alt_up_audio_enable_write_interrupt(audio);
}

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void playSoundISR(void* isr_context) {
#else
static void playSoundISR(void* isr_context, alt_u32 id) {
#endif
	if (DEBUG == 1) {
		printf("Entering playSoundISR.\n");
	}
	if (status == NONE) {
		alt_up_audio_disable_write_interrupt(audio);
		return;
	}
	int len;
	unsigned int free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
	if (free > 1) {
		int activeFileWordLength = getActiveFileWordLength();
		unsigned int **ptrToActiveBuffer = getActiveBuffer();
		unsigned int *activeBuffer = *ptrToActiveBuffer;
		activeBuffer = laserBuffer;
		if (activeBuffer != laserBuffer &&
			activeBuffer != playerDeathBuffer &&
			activeBuffer != sharkDeathBuffer &&
			activeBuffer != themeBuffer) {
			printf("Error: invalid buffer state.\n");
		}
		if (activeFileWordLength == FAIL ||
				ptrToActiveBuffer == 0) {
			printf("Error: invalid audio state.\n");
		}
		if ((int)playCursor + free >= (int)(activeBuffer) + (2 * activeFileWordLength)) {
			// Last chunk to play
			len = activeFileWordLength - free;
			alt_up_audio_disable_write_interrupt(audio);
			status = NONE;
			if (DEBUG == 1) {
				printf("done playing audio.\n");
			}
		} else {
			len = free;
		}
		playAudioMono(len);
		playCursor += len;
		if (status == NONE) {
			playCursor = NULL;
		}
	} else {
		// Interrupt should not be triggered if there is no space
		alt_up_audio_disable_write_interrupt(audio);
	}
}

unsigned int **getActiveBuffer(void) {
	switch(status) {
	case LASER:
		return (unsigned int **)(&laserBuffer);
	case PLAYER_DEATH:
		return (unsigned int **)(&playerDeathBuffer);
	case SHARK_DEATH:
		return (unsigned int **)(&sharkDeathBuffer);
	case THEME:
		return (unsigned int **)(&sharkDeathBuffer);
	}
	return (unsigned int **)0;
}

int getActiveBufferLength(void) {
	switch(status) {
	case LASER:
		return (int)laserBufferLength;
	case PLAYER_DEATH:
		return (int)playerDeathBufferLength;
	case SHARK_DEATH:
		return (int)(sharkDeathBufferLength);
	case THEME:
		return (int)(themeBufferLength);
	}
	return FAIL;
}

int getActiveFileWordLength(void) {
	switch(status) {
	case LASER:
		return laserFileWordLength;
	case PLAYER_DEATH:
		return playerDeathFileWordLength;
	case SHARK_DEATH:
		return sharkDeathFileWordLength;
	case THEME:
		return themeFileWordLength;
	}
	printf("Error: unable to get active file word length.\n");
	return FAIL;
}
