#define PI 3.14159265359
#define TONE 0
#define LASER 1
#define PLAYER_DEATH 2
#define SHARK_DEATH 3

void setupAudio(void);
void audioTest(void);
void runAudioDiagnostics(void);
void testTone(void);
void playLaser1(void);
void readWavFile(char *wavFileName, unsigned int fileWordLength, unsigned int *buffer);
void playAudioMono(unsigned int *buffer, int length);
void playLaser(void);
