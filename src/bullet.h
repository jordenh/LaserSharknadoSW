#ifndef BULLET_H
#define BULLET_H

#include "vga.h"

#define NUM_BULLETS 128
#define BULLET_LENGTH 3

typedef enum {
	PLAYERBULLET,
	SHARKBULLET,
	NOTACTIVE
} bulletstatus;

typedef struct {
	int x;
	int y;
	bulletstatus status;
} Bullet;

Bullet *bulletArray;

void initBullets();
void drawBullet(Bullet *bullet);
void eraseBullet(Bullet *bullet);
void moveRight(Bullet *bullet);

#endif
