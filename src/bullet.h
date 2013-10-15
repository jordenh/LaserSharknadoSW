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
	NOTACTIVE,
	DELETED,
	ONEMORE
} bullettype;

struct Bullet {
	float x, prevX;
	float y, prevY;
	bullettype type;
	char isAimed;
	float slope;
	int laserColor;
	struct Bullet *next;
	struct Bullet *prev;
};

typedef struct Bullet Bullet;

Bullet bulletArray[NUM_BULLETS];
extern Bullet *playerBulletList;
extern Bullet *sharkBulletList;

void initBullets();
void createBullet(bullettype status, int x, int y, int sudoRandomSeed);
void moveAllBullets();
void drawAllBullets();
void eraseAllBullets();
void drawBullet(Bullet *bullet);
void eraseBullet(Bullet *bullet);
void moveRight(Bullet *bullet);
void moveBulletRight(Bullet *bullet);
void moveBulletLeft(Bullet *bullet);
void deleteBullet(Bullet *bullet);

#endif
