#include "stdafx.h"
#include "Object.h"


void Rabbit::upDate()
{
	switch (state)
	{
	case waitState:
	{
		if (true == chaseRange())
			state = chaseState;
		if (true == hitDamge()) {
			state = chaseState;
		}
		else
			randomMove();
		break;
	}
	case attackState:
	{
		break;
	}
	case chaseState:
	{
		if (false == chaseRange())
			state = waitState;
		else
			move();
		break;
	}
	case deadState:
	{

		break;
	}

	}
}
bool Rabbit::chaseRange()
{
	D3DXVECTOR2 dir;
	D3DXVECTOR2 monPos;
	monPos.x = getPosX();
	monPos.y = getPosY();
	D3DXVECTOR2 targetPos = getTagetPos();
	float dist = (targetPos.x - monPos.x)
		*(targetPos.x - monPos.x)
		+ (targetPos.y - monPos.y)
		* (targetPos.y - monPos.y);
	if (dist <= 20 * 20)
	{
		dir = targetPos - monPos;
		D3DXVec2Normalize(&dir, &dir);
		setDir(dir);
		return true;
	}
	else
	{
		setTarget(-1);
		return false;
	}
}
void Rabbit::move()
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 dir = getDir();
	float sp = getSpeed();
	pos.x = getPosX();
	pos.y = getPosY();
	pos = pos + (sp*dir);
	setPosX(pos.x);
	setPosY(pos.y);
}
bool Rabbit::attakcRange()
{
	return false;
}
bool Rabbit::hitDamge()
{
	return false;
}
void Rabbit::randomMove()
{
	switch (rand()%3)
	{
	case 0:
	{
		setDir(D3DXVECTOR2{ 0,1 });
		move();
		break;
	}
	case 1:
	{
		setDir(D3DXVECTOR2{ 0,-1 });
		move();
		break;
	}
	case 2:
	{
		setDir(D3DXVECTOR2{ 1,0 });
		move();
		break;
	}
	case 3:
	{
		setDir(D3DXVECTOR2{ -1,0 });
		move();
		break;
	}

	}
}