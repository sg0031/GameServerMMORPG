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
#define MAX_OBJECT 3000
#define MAX_MAP_SIZE 2000
#define DIVDIE_SECTOR 100
#define MAX_SECTOR_SIZE 20

#define OBJECT_START 1000
#define RABBIT_START 1500
#define ARGO_START 2000
#define BABY_START 2500


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
#pragma pack (push, 1)
//클라이언트에서 보내는 패킷에서는 아이디값이 필요가 없다
//이유는 소켓으로 구분짓기 때문에 아이디값으로 구분을 줄 필요가 없다.
struct CsPacketChat {
	BYTE size;
	BYTE type;
	WCHAR message[100];
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
struct CsPacketAttack //플레이어가 공격버튼을 눌렀을때 보내는 패킷이다.
{
	BYTE packetSize;
	BYTE packetType;
};

struct ScStatusInfo
{
	BYTE pakcetSize;
	BYTE packetType;
	int id;
	int attack; // 플레이어 공격력
	int depend; //플레이어 방어력
	int miss;//플레이어 회피력
	int level;//플레이어 레벨
	int exp;//플레이어 경험치
	int str;//플레이어 힘
	int dex;//플레이어 민첩
	int mental;//플레이어 지력
	int gold;//플레이어의 보유중인 골드
	int acr; //플레이여 명중률
	int maxHealth;
	int statusCount;
};
struct ScPacketBuff
{
	BYTE pakcetSize;
	BYTE packetType;
	int id;
	int buff;
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
	int state;
	int health;
	PlayerPosition position;
};
struct ScPacketPutPlayer
{
	BYTE packetSize;
	BYTE packetType;
	int id;
	int state;
	int health;
	PlayerPosition position;
};
struct ScPacketRemovePlayer
{
	BYTE packetSize;
	BYTE packetType;
	int id;
};
struct ScPacketChat {
	BYTE size;
	BYTE type;
	int id;
	WCHAR message[100];
};
struct ScPacketStateMessage {
	BYTE size;
	BYTE type;
	int id;
	int gold;
	int exp;
	int damage;
};
#pragma pack (pop)