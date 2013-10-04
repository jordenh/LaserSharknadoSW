#ifndef BULLET_H
#define BULLET_H

#include "vga.h"
#include "player.h"
#include "audio.h"

#define NUM_BULLETS 128
#define BULLET_LENGTH 3

typedef enum {
	PLAYERBULLET,
	SHARKBULLET,
	NOTACTIVE
} bulletstatus;

typedef struct {
	int x, prevX;
	int y, prevY;
	bulletstatus status;
} Bullet;

Bullet bulletArray[NUM_BULLETS];

void initBullets();
void createBullet(bulletstatus status);
void moveAllBullets();
void drawAllBullets();
void eraseAllBullets();
void drawBullet(Bullet *bullet);
void eraseBullet(Bullet *bullet);
void moveRight(Bullet *bullet);

#endif
