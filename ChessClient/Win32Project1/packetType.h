#pragma once
enum MonsterType
{
	Stone=100,
	Tree,
	Rabbit
};

enum EOperationType
{
	Sendtype = 1,
	Recvtype = 2
};
struct OverEx	//오버렙트구조체 확장
{
	OVERLAPPED overLapped;
	SOCKET s;
	EOperationType operationType;	//패킷의 타입
	int prevSize;	//이전데이타 크기
	int currentSize;//현재데이타 크기
	WSABUF buf;
	char packetBuf[256];
	char iocpBuf[4096];
};
enum
{
	
	CS_RIGHT=1,
	CS_LEFT,
	CS_UP,
	CS_DOWN,
	CS_LOGIN,
	CS_ATTACK_A,
	CS_FIRE_SKILL,
	CS_STATE_UPDATE,
	CS_STOP,
	CS_CHAT,
	CS_STR_UP,
	CS_DEX_UP,
	CS_MENTAL_UP,

	SC_MOVE_POSITION=100,
	SC_LOGIN_COMPLTE,
	SC_SET_PLAYER,
	SC_REMOVE_PLAYER,
	SC_BUFF,
	SC_STATE_UPDATE,
	SC_PLAYER_STATUS,
	SC_CHAT,
	SC_LEVEL_UP,
	SC_HITDAMGE,
	SC_MONSTER_ATTACK_PLAYER,
	SC_MONSTER_DEAD

};