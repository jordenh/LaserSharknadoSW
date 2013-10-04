#ifndef AUDIO_H
#define AUDIO_H

#define PI 3.14159265359
#define NONE 0
#define LASER 1
#define PLAYER_DEATH 2
#define SHARK_DEATH 3
#define THEME 4
#define FAIL -1

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

#endif
