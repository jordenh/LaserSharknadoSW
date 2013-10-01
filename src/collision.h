#ifndef COLLISION_H
#define COLLISION_H

#define RIGHT 1
#define LEFT -1
#define TRUE 1
#define FALSE 0
#define SHARK_LENGTH 37
#define SHARK_HEIGHT 20
#define PLAYER_LENGTH 20
#define PLAYER_HEIGHT 20

// STUB
typedef struct {
	int x;
	int y;
} Shark;

typedef struct {
	int x;
	int y;
} Player;

typedef struct {
	int x;
	int y;
	int direction;
} Bullet;

int isBulletCollidingWithShark(Shark *shark, Bullet *bullet);
int isBulletCollidingWithPlayer(Player *player, Bullet *bullet);

#endif
