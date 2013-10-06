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

volatile short int status;
volatile short int loaded;

volatile unsigned int *playCursor;
static unsigned int *audioBuffer;
unsigned int audioFileWordLength;
unsigned int playedWords;

static unsigned int *laserBuffer = NULL;
static unsigned int *playerDeathBuffer = NULL;
static unsigned int *sharkDeathBuffer = NULL;
static unsigned int *themeBuffer = NULL;

volatile int somethingForIrq;

unsigned int **getActiveBuffer(void);
int getActiveBufferLength(void);
int getActiveFileWordLength(void);
int setupAudioInterrupt(alt_up_audio_dev *audio, volatile int somethingForIrq);
void playAudio(unsigned int *leftBuffer, int leftLength, unsigned int *rightBuffer, int rightLength);
void loadLaser(void);
void loadPlayerDeath(void);
void loadSharkDeath(void);
void loadTheme(void);

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
    int interruptStatus = setupAudioInterrupt(audio, somethingForIrq);

    if (interruptStatus < 0) {
		printf("Error: audio interrupt could not be setup.\n");
		error = true;
	} else if (DEBUG == 1) {
		printf("Successfully setup audio interrupts.\n");
	}

    loadLaser();
    loadPlayerDeath();
    loadSharkDeath();
    loadTheme();

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

void loadLaser() {
	audioFileWordLength = 38384;
	if (laserBuffer == NULL) {
		readWavFile("laserii.wav", audioFileWordLength);
		laserBuffer = audioBuffer;
	}
	audioBuffer = laserBuffer;
	loaded = LASER;
}

void loadPlayerDeath() {
	audioFileWordLength = 0x0000DAFF / 2;
	if (playerDeathBuffer == NULL) {
		readWavFile("pdie.wav", audioFileWordLength);
		playerDeathBuffer = audioBuffer;
	}
	audioBuffer = playerDeathBuffer;
	loaded = PLAYER_DEATH;
}

void loadSharkDeath() {
	audioFileWordLength = 0x0000DAFF / 2;
	if (sharkDeathBuffer == NULL) {
		readWavFile("sdie.wav", audioFileWordLength);
		sharkDeathBuffer = audioBuffer;
	}
	audioBuffer = sharkDeathBuffer;
	loaded = SHARK_DEATH;
}

void loadTheme() {
	audioFileWordLength = 0x00063E00 / 2;
	if (themeBuffer == NULL) {
		readWavFile("theme.wav", audioFileWordLength);
		themeBuffer = audioBuffer;
	}
	audioBuffer = themeBuffer;
	loaded = THEME;
}

void playAudioMono(int length) {
	if (DEBUG == 1) {
		printf("Playing audio.\n");
	}
	int left = alt_up_audio_write_fifo(audio, (unsigned int *)playCursor, length, ALT_UP_AUDIO_LEFT);
	int right = alt_up_audio_write_fifo(audio, (unsigned int *)playCursor, length, ALT_UP_AUDIO_RIGHT);
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

void readWavFile(char *wavFileName, unsigned int fileWordLength) {
	if (audioBuffer != NULL) {
		//free(audioBuffer);
	}
	audioBuffer = malloc(sizeof(unsigned int) * fileWordLength);
	if (audioBuffer == NULL) {
		printf("Error: insufficient memory to load audio file into memory.\n");
	}

	short int fileHandle = openFile(wavFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", wavFileName);
		return;
	}

	readPastWavHeader(fileHandle);

	unsigned int i;
	unsigned int word = readWord(fileHandle);
	//printf("first word is %x\n", word);
	for (i = 0; i < fileWordLength; i++) {
		audioBuffer[i] = word;
		word = readWord(fileHandle);
		//printf("0x%x ", (int)word > 0 ? word : -1 * word);
	}
	//printf("reached EOF\n");

	closeFile(fileHandle);
	return;
}

int i;
// Plays laser once, using interrupts
void playLaser(void) {
	if (DEBUG == 1) {
		printf("Playing laser via interrupt.\n");
	}
	i = 0;
	loadLaser();
	status = LASER;
	playCursor = audioBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

void playPlayerDeath(void) {
	loadPlayerDeath();
	status = PLAYER_DEATH;
	playCursor = audioBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

void playSharkDeath(void) {
	loadSharkDeath();
	status = SHARK_DEATH;
	playCursor = audioBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

void playTheme(void) {
	loadTheme();
	status = THEME;
	playCursor = audioBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void playSoundISR(void* isr_context) {
#else
static void playSoundISR(void* isr_context, alt_u32 id) {
#endif
	if (status == NONE) {
		alt_up_audio_disable_write_interrupt(audio);
		return;
	}
	int len;
	unsigned int free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_LEFT);
	unsigned end = (unsigned)(audioBuffer) + (2 * audioFileWordLength);
	if (free >= 1) {
		if (((int)playCursor + free >= end) ||
			 (playedWords + free) >= audioFileWordLength) {
			// Last chunk to play
			len = end - (int)playCursor;
			alt_up_audio_disable_write_interrupt(audio);
		} else {
			len = free;
		}
		len = len > free ? free : len;
		playAudioMono(len);
		playedWords += len;
		playCursor += len;
	} else {
		// Interrupt should not be triggered if there is no space
		alt_up_audio_disable_write_interrupt(audio);
	}
	i++;
}
