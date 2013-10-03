#include "bullet.h"

void drawBullet(Bullet *bullet){
	drawLine(bullet->x, bullet->y, bullet->x + BULLET_LENGTH, bullet->y, convert24BitRgbTo16(0xFF000C));//0xFF00);
}

void eraseBullet(Bullet *bullet){
	drawLine(bullet->x, bullet->y, bullet->x + BULLET_LENGTH, bullet->y, 0x0000);
}

void moveRight(Bullet *bullet){
	eraseBullet(bullet);
	bullet->x = bullet->x + 1;
	if (bullet->x >= SCREEN_WIDTH)
	{
		bullet->status = NOTACTIVE;
		return;
	}
	drawBullet(bullet);
}
