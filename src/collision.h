#ifndef COLLISION_H
#define COLLISION_H

#include "stubs.h"
#include "player.h"
#include "bullet.h"
#include "shark.h"

int isBulletCollidingWithShark(Shark *shark, Bullet *bullet);
int isBulletCollidingWithPlayer(Player *player, Bullet *bullet);

#endif
