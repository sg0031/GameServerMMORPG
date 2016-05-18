#pragma once
#define WINDOWSWIDTH 800
#define WINDOWSHEIGHT 800

#define WIDTH 100
#define HEIGHT 100

#define WIDTHRECTANGLE 8
#define HEIGHTRECTANGLE 8


#define MAXBUFSIZE 256
#define WM_SOCKET WM_USER+1

#define MAX_PLAYER 10

#define STATIC_OBJECT_START 0
#define DYNAMIC_OBJECT_START 500

struct Removeindex
{
	bool Bol;
	int index;
};
struct PlayerPosition
{
	int x;
	int y;
};

//Ŭ���̾�Ʈ���� ������ ��Ŷ������ ���̵��� �ʿ䰡 ����
//������ �������� �������� ������ ���̵����� ������ �� �ʿ䰡 ����.
struct CsPacketLogin
{
	BYTE packetSize;
	BYTE packetType;
};

struct CsPacketMove
{
	BYTE packetSize;
	BYTE packetType;
	int id;
};
struct CsPacketAttack //�÷��̾ ���ݹ�ư�� �������� ������ ��Ŷ�̴�.
{
	BYTE packetSize;
	BYTE packetType;
};


struct ScPacketPlayerPosition
{
	BYTE pakcetSize;
	BYTE packetType;
	int id;
	PlayerPosition position;
};
struct ScPacketMove
{
	BYTE pakcetSize;
	BYTE packetType;
	int id;
	PlayerPosition position;
};
struct ScPacketPutPlayer
{
	BYTE packetSize;
	BYTE packetType;
	int id;
	PlayerPosition position;
};
struct ScPacketRemovePlayer
{
	BYTE packetSize;
	BYTE packetType;
	int id;
	int removeID;
};