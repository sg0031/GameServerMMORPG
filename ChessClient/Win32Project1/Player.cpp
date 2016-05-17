#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	id = -1;
	x = -1;
	y = -1;
}


Player::~Player()
{
}

void Player::setPositionX(int x)
{
	this->x = x;
}
void Player::setPositionY(int y)
{
	this->y = y;
}
int Player::getPositionX()
{
	return x;
}
int Player::getPositionY()
{
	return y;
}