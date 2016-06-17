#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	id = -1;
	x = PLAYER_START_POSX;
	y = PLAYER_START_POSY;
	attack=100; // 플레이어 공격력
	maxAttack = 100;
	depend=10; //플레이어 방어력
	maxDepend = 10;
	miss=10;//플레이어 회피력
	level=1;//플레이어 레벨
	exp=0;//플레이어 경험치
	str=0;//플레이어 힘
	dex=0;//플레이어 민첩
	mental=0;//플레이어 지력
	gold=0;//플레이어의 보유중인 골드
	health=MAX_HEALTH; //플레이어 체력
	maxHealth = MAX_HEALTH;
	acr=10; //플레이여 명중률
	statusCount=0; //레벨업시에 남아있는

	animateState = waitState;
	pViewList.clear();
	pObjectList.clear();

	overEx = new OverEx;
	ZeroMemory(&overEx->iocpBuf, sizeof(overEx->iocpBuf));
	ZeroMemory(&overEx->packetBuf, sizeof(overEx->packetBuf));
	overEx->s = NULL;
	overEx->operationType = Recvtype;
	overEx->prevSize = 0;
	overEx->currentSize = 0;
	overEx->buf.buf = reinterpret_cast<CHAR*>(overEx->iocpBuf);
	overEx->buf.len = sizeof(overEx->iocpBuf);
	ZeroMemory(&overEx->overLapped, sizeof(overEx->overLapped));
	for (auto i = 0; i < MAX_PLAYER; ++i)
	{
		viewList[i] = -1;
	}
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