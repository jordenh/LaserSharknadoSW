#ifndef BULLET_H
#define BULLET_H

#include "vga.h"
#include "player.h"
#include "audio.h"
#include "nado.h"

#define NUM_BULLETS 128
#define BULLET_LENGTH 3
#define BULLET_SPEED 2

typedef enum {
	PLAYERBULLET,
	SHARKBULLET,
	NOTACTIVE,
	DELETE
} bullettype;

struct Bullet {
	float x, prevX, dx;  // x location, prev x location and the distance moved per tick
	float y, prevY, dy; // y location, prev y location and the distance moved per tick
	bullettype type;
	char isAimed; // flag if the bullet is aimed
	float slope; // slope of the bullet trajectory
	int laserColor; // color of the laser - player and shark have different colors
	struct Bullet *next; // there is a linked list of active shark and player bullets
	struct Bullet *prev; // these pointers form the links between the nodes
};

typedef struct Bullet Bullet;

Bullet bulletArray[NUM_BULLETS];
extern Bullet *playerBulletList;
extern Bullet *sharkBulletList;

void initBullets();
void createBullet(bullettype status, int x, int y, int aimed);
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
