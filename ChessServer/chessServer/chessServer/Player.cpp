#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	id = -1;
	x = PLAYER_START_POSX;
	y = PLAYER_START_POSY;
	attack=100; // �÷��̾� ���ݷ�
	maxAttack = 100;
	depend=10; //�÷��̾� ����
	maxDepend = 10;
	miss=10;//�÷��̾� ȸ�Ƿ�
	level=1;//�÷��̾� ����
	exp=0;//�÷��̾� ����ġ
	str=0;//�÷��̾� ��
	dex=0;//�÷��̾� ��ø
	mental=0;//�÷��̾� ����
	gold=0;//�÷��̾��� �������� ���
	health=MAX_HEALTH; //�÷��̾� ü��
	maxHealth = MAX_HEALTH;
	acr=10; //�÷��̿� ���߷�
	statusCount=0; //�������ÿ� �����ִ�

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