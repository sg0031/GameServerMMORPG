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
#define MAX_MAP_SIZE 2000
#define DIVDIE_SECTOR 100
#define MAX_SECTOR_SIZE 20

#define MONSTER_START 2000
#define OBJECT_START 1000


struct PlayerPosition
{
	int x;
	int y;
};

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
};