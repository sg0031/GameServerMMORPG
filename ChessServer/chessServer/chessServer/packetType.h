#pragma once




enum EOperationType
{
	Sendtype = 1,
	Recvtype = 2,
	MonsterUpdate
};
struct OverEx	//������Ʈ����ü Ȯ��
{
	OVERLAPPED overLapped;
	SOCKET s;
	EOperationType operationType;	//��Ŷ�� Ÿ��
	int prevSize;	//��������Ÿ ũ��
	int currentSize;//���絥��Ÿ ũ��
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