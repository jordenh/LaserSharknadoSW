#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "bullet.h"
#include "shark.h"
#include "bmp.h"
#include "nado.h"

void initCollision(void);
int isBulletCollidingWithShark(Shark *shark, Bullet *bullet);
int isBulletCollidingWithPlayer(Player *player, Bullet *bullet);
Shark *findSharkIfHit(Bullet *bullet);
void doSharkBulletCollision(void);
void doPlayerBulletCollision(void);
void doNadoBulletCollision(void);

#endif
