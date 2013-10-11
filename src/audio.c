#include "audio.h"


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
static unsigned int *audioVolumeBuffer;
unsigned int audioFileWordLength;
unsigned int playedWords;

struct audioInfo laser;
struct audioInfo playerDeath;
struct audioInfo sharkDeath;
struct audioInfo theme;

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
    int interruptStatus = setupAudioInterrupt(audio, somethingForIrq);

    if (interruptStatus < 0) {
		printf("Error: audio interrupt could not be setup.\n");
		error = true;
	} else if (DEBUG == 1) {
		printf("Successfully setup audio interrupts.\n");
	}

    laser.mainBuffer = NULL;
    laser.volumeBuffer = NULL;
    loadLaser();
    playerDeath.mainBuffer = NULL;
    playerDeath.volumeBuffer = NULL;
    loadPlayerDeath();
    sharkDeath.mainBuffer = NULL;
    sharkDeath.volumeBuffer = NULL;
    loadSharkDeath();
    theme.mainBuffer = NULL;
    theme.volumeBuffer = NULL;
    loadTheme();
    printf("finised loading sound buffers.\n");

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
    laser.bufferLength = 38384;
	audioFileWordLength = laser.bufferLength;
	if (laser.mainBuffer == NULL || laser.volumeBuffer == NULL) {
		readWavFile("laserii.wav", laser.bufferLength);
		//laserBuffer = audioBuffer;
		laser.mainBuffer = audioBuffer;
		laser.volumeBuffer = audioVolumeBuffer;
	}
	//audioBuffer = laserBuffer;
	audioBuffer = laser.mainBuffer;
	audioVolumeBuffer = laser.volumeBuffer;
	loaded = LASER;
}

void loadPlayerDeath() {
	playerDeath.bufferLength = 0x0000DAFF / 2;
	audioFileWordLength = playerDeath.bufferLength;
	if (playerDeath.mainBuffer == NULL || playerDeath.volumeBuffer == NULL) {
		readWavFile("pdie.wav", playerDeath.bufferLength);
		//playerDeathBuffer = audioBuffer;
		playerDeath.mainBuffer = audioBuffer;
		playerDeath.volumeBuffer = audioVolumeBuffer;
	}
	//audioBuffer = playerDeathBuffer;
	audioBuffer = playerDeath.mainBuffer;
	audioVolumeBuffer = playerDeath.volumeBuffer;
	loaded = PLAYER_DEATH;
}

void loadSharkDeath() {
	sharkDeath.bufferLength = 0x0000DAFF / 2;
	audioFileWordLength = sharkDeath.bufferLength;
	if (sharkDeath.mainBuffer == NULL || sharkDeath.volumeBuffer == NULL) {
		readWavFile("sdie.wav", sharkDeath.bufferLength);
		//sharkDeathBuffer = audioBuffer;
		sharkDeath.mainBuffer = audioBuffer;
		sharkDeath.volumeBuffer = audioVolumeBuffer;
	}
	//audioBuffer = sharkDeathBuffer;
	audioBuffer = sharkDeath.mainBuffer;
	audioVolumeBuffer = sharkDeath.volumeBuffer;
	loaded = SHARK_DEATH;
}

void loadTheme() {
	theme.bufferLength = 0x00063E00 / 2;
	audioFileWordLength = theme.bufferLength;
	if (theme.mainBuffer == NULL || theme.volumeBuffer == NULL) {
		readWavFile("theme.wav", theme.bufferLength);
		//themeBuffer = audioBuffer;
		theme.mainBuffer = audioBuffer;
		theme.volumeBuffer = audioVolumeBuffer;
	}
	//audioBuffer = themeBuffer;
	audioBuffer = theme.mainBuffer;
	audioVolumeBuffer = theme.volumeBuffer;
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
	audioBuffer = malloc(sizeof(unsigned int) * fileWordLength);
	audioVolumeBuffer = malloc(sizeof(unsigned int) * fileWordLength);
	if (audioBuffer == NULL || audioVolumeBuffer == NULL) {
		printf("Error: insufficient memory to load audio file into memory.\n");
	}

	short int fileHandle = openFile(wavFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", wavFileName);
		return;
	}

	readPastWavHeader(fileHandle);

	unsigned int i;
	unsigned int word;
	for (i = 0; i < fileWordLength; i++) {
		word = readWord(fileHandle);
		audioBuffer[i] = word;
		audioVolumeBuffer[i] = word;
		//printf("0x%x ", (int)word > 0 ? word : -1 * word);
	}
	//printf("reached EOF\n");

	closeFile(fileHandle);
	return;
}

// Plays laser once, using interrupts
void playLaser(void) {
	if (DEBUG == 1) {
		printf("Playing laser via interrupt.\n");
	}
	loadLaser();
	status = LASER;
	playCursor = audioVolumeBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

void playPlayerDeath(void) {
	loadPlayerDeath();
	status = PLAYER_DEATH;
	playCursor = audioVolumeBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

void playSharkDeath(void) {
	loadSharkDeath();
	status = SHARK_DEATH;
	playCursor = audioVolumeBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

void playTheme(void) {
	loadTheme();
	status = THEME;
	playCursor = audioVolumeBuffer;
	playedWords = 0;
	alt_up_audio_enable_write_interrupt(audio);
}

void stopTheme(void) {
	status = NONE;
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
	unsigned end = (unsigned)(audioVolumeBuffer) + (2 * audioFileWordLength);
	if (free >= 1) {
		if (((int)playCursor + free >= end) ||
			 (playedWords + free) >= audioFileWordLength) {
			// Last chunk to play
			len = end - (int)playCursor;
			if (status == THEME) {
				loadTheme();
				playCursor = audioVolumeBuffer;
				playedWords = 0;
			} else {
				alt_up_audio_disable_write_interrupt(audio);
			}
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
}

void updateAudioWithVolume(char switchValues) {

	if (laser.mainBuffer != NULL && laser.volumeBuffer != NULL){
		changeBufferVolume(laser, switchValues);
	}
	if (playerDeath.mainBuffer != NULL && playerDeath.volumeBuffer != NULL){
		changeBufferVolume(playerDeath, switchValues);
	}
	if (sharkDeath.mainBuffer != NULL && sharkDeath.volumeBuffer != NULL) {
		changeBufferVolume(sharkDeath, switchValues);
	}
	if (theme.mainBuffer != NULL && theme.volumeBuffer != NULL) {
		changeBufferVolume(theme, switchValues);
	}

	return;
}

void changeBufferVolume(struct audioInfo currentAudioInfo, char switchValues) {
	int i;
	short volumeKeys = switchValues & 0x0F;
	if(volumeKeys == 0x00 || volumeKeys == 0x01) {
		short shiftLength = 2 - volumeKeys;
		for(i = 0; i < currentAudioInfo.bufferLength; i++) {
			*(currentAudioInfo.volumeBuffer + i) = *(currentAudioInfo.mainBuffer + i) >> shiftLength;
		}
	} else if(volumeKeys == 0x07 || volumeKeys == 0x0F) {
		short shiftLength = (volumeKeys % 7) + 1;
		for(i = 0; i < currentAudioInfo.bufferLength; i++) {
			*(currentAudioInfo.volumeBuffer + i) = *(currentAudioInfo.mainBuffer + i) << shiftLength;
		}
	} else if(volumeKeys == 0x03) {
		for(i = 0; i < currentAudioInfo.bufferLength; i++) {
			*(currentAudioInfo.volumeBuffer + i) = *(currentAudioInfo.mainBuffer + i);
		}
	} else {
		for(i = 0; i < currentAudioInfo.bufferLength; i++) {
			*(currentAudioInfo.volumeBuffer + i) = 0; //mute on all other combos
		}
	}

	return;
}
