#include "audio.h"

// preprocessor directives used to support both the basic and enhanced interrupt apis
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void playSoundISR(void* isr_context);
#else
static void playSoundISR(void* isr_context, alt_u32 id);
#endif

// Constants used to open devices
const char *CONFIG_NAME = "/dev/audio_and_video_config_0";
const char *AUDIO_NAME = "/dev/audio_0";

// Pointers to the devices
alt_up_audio_dev *audio = NULL;
alt_up_av_config_dev *config = NULL;

int DEBUG = 0;

volatile short int status;
volatile short int loaded;

volatile unsigned int *playCursor;
unsigned int audioFileWordLength;
unsigned int playedWords;

struct audioInfo laser;
struct audioInfo playerDeath;
struct audioInfo sharkDeath;
struct audioInfo theme;

static unsigned int playBuffer[PLAYBUFFER_LEN];
struct audioInfo *sounds[4];
unsigned short int numSounds = 4;

volatile int somethingForIrq;

void setupAudio()
{
	bool error = false;

	// Initialize audio video config core
	config = (alt_up_av_config_dev *)alt_up_av_config_open_dev(CONFIG_NAME);
	if (config == NULL) {
		printf("Error: audio video config could not be opened.\n");
		error = true;
	}
	while (!alt_up_av_config_read_ready(config)) {
	}

	// Initialize audio core
	audio = (alt_up_audio_dev *)alt_up_audio_open_dev(AUDIO_NAME);
	if (config == NULL) {
		printf("Error: audio codec could not be opened.\n");
		error = true;
	} else if (DEBUG == 1) {
		printf("Successfully opened audio codec.\n");
	}

	// Initialize the audio interrupt
    int interruptStatus = setupAudioInterrupt(audio, somethingForIrq);
    if (interruptStatus < 0) {
		printf("Error: audio interrupt could not be setup.\n");
		error = true;
	} else if (DEBUG == 1) {
		printf("Successfully setup audio interrupts.\n");
	}

	// Load sound effects
    loadLaser();
    loadPlayerDeath();
    loadSharkDeath();
    loadTheme();
    printf("finished loading sound buffers.\n");

    sounds[0] = &laser;
    sounds[1] = &sharkDeath;
    sounds[2] = &playerDeath;
    sounds[3] = &theme;

	// Not playing a sound effect yet
	status = NONE;

	if (DEBUG == 1 && error == false) {
		printf("Successfully setup sound.\n");
	}
}

int setupAudioInterrupt(alt_up_audio_dev *audio, volatile int somethingForIrq)
{
    // Disable both audio interrupts before setting them up
    // otherwise you get stuck in them when they are setup
    alt_up_audio_disable_read_interrupt(audio);
    alt_up_audio_disable_write_interrupt(audio);

    void *irqInt = (void*)&somethingForIrq;

	// Supporting both interupt apis
	#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
	return alt_ic_isr_register(AUDIO_0_IRQ_INTERRUPT_CONTROLLER_ID, AUDIO_0_IRQ, playSoundISR, irqInt, 0x0);
	#else
	return alt_irq_register(AUDIO_0_IRQ, irqInt, playSoundISR);
	#endif
}

void loadLaser() {
    laser.bufferLength = 38384;
	readWavFile("laserii.wav", &laser);
}

void loadPlayerDeath() {
	playerDeath.bufferLength = 0x0000DAFF / 2;
	readWavFile("pdie.wav", &playerDeath);
}

void loadSharkDeath() {
	sharkDeath.bufferLength = 0x0000DAFF / 2;
	readWavFile("sdie.wav", &sharkDeath);
}

void loadTheme() {
	theme.bufferLength = 0x00063E00 / 2;
	readWavFile("theme.wav", &theme);
}

// 'Plays' a number of words from the play buffer by writing them into the audio FIFOs
void playAudioMono(int length) {
	if (DEBUG == 1) {
		printf("Playing audio.\n");
	}
	alt_up_audio_write_fifo(audio, (unsigned int *)playBuffer, length, ALT_UP_AUDIO_LEFT);
	alt_up_audio_write_fifo(audio, (unsigned int *)playBuffer, length, ALT_UP_AUDIO_RIGHT);
}

// Initializes an audioInfo struct with a wav file
void readWavFile(char *wavFileName, struct audioInfo *info) {
	info->mainBuffer = malloc(sizeof(unsigned int) * info->bufferLength);
	info->volumeBuffer = malloc(sizeof(unsigned int) * info->bufferLength);
	if (info->mainBuffer == NULL || info->volumeBuffer == NULL) {
		printf("Error: insufficient memory to load audio file into memory.\n");
	}

	short int fileHandle = openFile(wavFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", wavFileName);
		return;
	}

	// Move the read cursor past the wav file header
	readPastWavHeader(fileHandle);

	// Fill the main and volume buffers
	unsigned int i;
	unsigned int word;
	for (i = 0; i < info->bufferLength; i++) {
		word = readWord(fileHandle);
		info->mainBuffer[i] = word;
		info->volumeBuffer[i] = word;
		//printf("0x%x ", (int)word > 0 ? word : -1 * word);
	}
	//printf("reached EOF\n");

	closeFile(fileHandle);
	return;
}

// Begins playing a sound associated with the audioInfo struct passed in
void playSound(struct audioInfo *info) {
	//printf("Playing sound with info at 0x%x.\n", info);
	info->active = true;
	info->playedWords = 0;
	info->playCursor = info->volumeBuffer;
	alt_up_audio_enable_write_interrupt(audio);
}

// Used by other modules to play the laser sound effect
void playLaser(void) {
	playSound(&laser);
}

// Used by other modules to play the player death sound effect
void playPlayerDeath(void) {
	playSound(&playerDeath);
}

// Used by other modules to play the shark death sound effect
void playSharkDeath(void) {
	playSound(&sharkDeath);
}

// Used by other modules to play the theme
void playTheme(void) {
	playSound(&theme);
	status = THEME;
	if (theme.active != true) {
		printf("Error: theme was not set to active in play theme.\n");
	}
}

// Stops the theme playing
void stopTheme(void) {
	status = NONE;
}

void clearPlayBuffer(void) {
	int i;
	for (i = 0; i < PLAYBUFFER_LEN; i++) {
		playBuffer[i] = 0;
	}
}

// Adds the contents of the argument buffer to the play buffer (superposition)
void addToPlayBuffer(unsigned int *buffer, unsigned int len) {
	int i;
	for (i = 0; i < len; i++) {
		playBuffer[i] += buffer[i];
	}
}

// Adds a potion of a sound's volume buffer to the play buffer
// Limited by the free space in the audio FIFOs
void addChunkToPlayBuffer(struct audioInfo *info, unsigned int free) {
	unsigned int len, i;
	unsigned int end = (unsigned)(info->volumeBuffer) + (2 * info->bufferLength);
	if (((int)info->playCursor + free >= end) ||
		 (info->playedWords + free) >= info->bufferLength) {
		// Last chunk to play
		len = end - (int)playCursor; // should probably be info->playCursor, but things work
		if (status == THEME && info == &theme) {
			info->playCursor = info->volumeBuffer;
			info->playedWords = 0;
		} else {
			info->active = false;
		}
	} else {
		len = free;
	}
	len = len > free ? free : len;
	info->playedWords += len;
	for (i = 0; i < len; i++) {
		playBuffer[i] += info->playCursor[i];
	}
	info->playCursor += len;
}

// ISR used to write to the FIFOs
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void playSoundISR(void* isr_context) {
#else
static void playSoundISR(void* isr_context, alt_u32 id) {
#endif
	int i;
	unsigned int free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_LEFT);
	bool atLeastOneActive = false;

	clearPlayBuffer();

	if (free >= 1) {
		// Load the play buffer
		for (i = 0; i < numSounds; i++) {
			if (sounds[i]->active == true) {
				//printf("Sounds %d is active.\n", i);
				atLeastOneActive = true;
				addChunkToPlayBuffer(sounds[i], free);
			}
		}
		// Write to the FIFOs
		playAudioMono(free);
	}

	if (atLeastOneActive == false) {
		//printf("Disabling audio interrupt.\n");
		alt_up_audio_disable_write_interrupt(audio);
	}
}

// Change the volume by refilling the volume buffer of an audioInfo struct
// Based on the values read from DE2 switches
void changeBufferVolume(struct audioInfo *currentAudioInfo, char switchValues) {
	int i;
	short volumeKeys = switchValues & 0x0F;
	if(volumeKeys == 0x00 || volumeKeys == 0x01) {
		short shiftLength = 2 - volumeKeys;
		for(i = 0; i < currentAudioInfo->bufferLength; i++) {
			*(currentAudioInfo->volumeBuffer + i) = *(currentAudioInfo->mainBuffer + i) >> shiftLength;
		}
	} else if(volumeKeys == 0x07 || volumeKeys == 0x0F) {
		short shiftLength = (volumeKeys % 7) + 1;
		for(i = 0; i < currentAudioInfo->bufferLength; i++) {
			*(currentAudioInfo->volumeBuffer + i) = *(currentAudioInfo->mainBuffer + i) << shiftLength;
		}
	} else if(volumeKeys == 0x03) {
		for(i = 0; i < currentAudioInfo->bufferLength; i++) {
			*(currentAudioInfo->volumeBuffer + i) = *(currentAudioInfo->mainBuffer + i);
		}
	} else {
		for(i = 0; i < currentAudioInfo->bufferLength; i++) {
			*(currentAudioInfo->volumeBuffer + i) = 0; //mute on all other combos
		}
	}
}

// Update the volumes for all sound effects
void updateAudioWithVolume(char switchValues) {
	int i = 0;
	for (i = 0; i < numSounds; i++) {
		if (sounds[i]->active == true) {
			changeBufferVolume(sounds[i], switchValues);
		}
	}
}
