#include "stdafx.h"
#include "Object.h"


void Rabbit::upDate()
{
	if (health <= 0)
		state = deadState;
	switch (state)
	{
	case waitState:
	{
		if (true == chaseRange()) {
			state = chaseState;
			float x = getPosX();
			float y = getPosY();
			setReturnPos(x, y);
		}
		if (true == hitDamge()) {
			state = chaseState;
		}
		else
			randomMove();
		break;
	}
	case chaseState:
	{
		D3DXVECTOR2 dir;
		D3DXVECTOR2 returnPos =getReturnPos();
		D3DXVECTOR2 currentPos;
		currentPos.x = getPosX();
		currentPos.y = getPosY();
		if (false == chaseRange()) {
			dir = returnPos - currentPos;
			D3DXVec2Normalize(&dir, &dir);
			setDir(dir);
			state = returnState;
		}
		if (true == attakcRange())
			state = attackState;
		else
			move();
		break;
	}
	case returnState:
	{
		D3DXVECTOR2 returnPos = getReturnPos();
		float x = getPosX();
		float y = getPosY();

		if (((returnPos.x+5 > x) || (x>returnPos.x-5)) && 
			((returnPos.y + 5 > y) || (y>returnPos.y - 5)))
			state = waitState;
		else
			move();
		break;
	}
	case attackState:
	{
		if (false == attakcRange())
			state = chaseState;
		//std::cout << getID() << "가 " << getTarget() << "을 공격중" << std::endl;
		break;
	}
	case deadState:
	{
		level = 0;
		//cout << getID() << "가 죽음" << endl;
		break;
	}

	}
}
bool Rabbit::attakcRange()
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
		return true;
	}
	else
	{
		return false;
	}
}
bool Rabbit::playerHit()
{
	return false;
}
bool Rabbit::hitDamge()
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
		return true;
	}
	else
	{
		return false;
	}
} //플레이어가 공격당했는지 판단하기위한 함수
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
	if (dist <= 50 * 50)
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
void Rabbit::decreaseHP(int ack)
{
	health -= ack;
	//cout << getID() << "몬스터 데미지 입음" << endl;
}