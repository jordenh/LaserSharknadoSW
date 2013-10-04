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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"
#include "timer.h"
#include "sd_card.h"

struct scores {
	int currentPlayerScore;
	int highScoreBoard[10];
	unsigned char * highScoreBoardInits[10];
};

void initScoreBoard(void);

void getHighScoreBoard(void);
void updateHighScoreBoard(void);

void updateCurrentPlayerScore(int deltaScore);
int getCurrentPlayerScore(void);


#endif /* SCORE_H_ */

/*
 * high score board file will be in format -
 * 3Initials score1 3Initials score2 3Initials score3 .... 3Initials score10
 *
 */
