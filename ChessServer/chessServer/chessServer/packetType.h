#pragma once




enum EOperationType
{
	Sendtype = 1,
	Recvtype = 2,
	MonsterUpdate
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

	CS_RIGHT = 1,
	CS_LEFT,
	CS_UP,
	CS_DOWN,
	CS_LOGIN,


	SC_MOVE_POSITION = 100,
	SC_LOGIN_COMPLTE,
	SC_SET_PLAYER,
	SC_REMOVE_PLAYER

};