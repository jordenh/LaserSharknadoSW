/*
 * score.h
 *
 *  Created on: Oct 3, 2013
 *      Author: jorde_000
 */

#ifndef SCORE_H_
#define SCORE_H_

#define NUMSCORES 10
#define NUMINITIALS 3
#define MAXSCOREDIGITS 10
#define INITIALLIVES 3



#include "lcd.h"
#include "audio.h"
#include "timer.h"
#include "sd_card.h"
#include "vga.h"
#include "bmp.h"
#include "io.h"
#include "altera_nios2_qsys_irq.h"
#include "sys/alt_irq.h"
#include "displacement.h"
#include "collision.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"
#include "timer.h"
#include "sd_card.h"

struct scores {
	int currentPlayerScore;
	unsigned int currentPlayerLives;
	int highScoreBoard[10];
	unsigned char highScoreBoardInits[10][3];
	unsigned char currentScoreBoardCorrupt;
};

void initScoreBoard(struct scores * gameScores);
void defaultHighScoreBoard(struct scores * gameScores);

int readHighScoreBoardFromSD(struct scores * gameScores);
void updateHighScoreBoard(struct scores * gameScores);

void updateCurrentPlayerScore(struct scores * gameScores, int deltaScore);
int getCurrentPlayerScore(struct scores * gameScores);
void setCurrentPlayerLives(struct scores * gameScores, int newNumLives);
int getCurrentPlayerLives(struct scores * gameScores);

void drawInGameInfo(struct scores * gameScores);
void drawScore(struct scores * gameScores);
void clearScore(void);

#endif /* SCORE_H_ */

/*
 * high score board file will be in format -
 * 3Initials score1 3Initials score2 3Initials score3 .... 3Initials score10
 *
 */

