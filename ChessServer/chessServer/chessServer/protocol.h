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
//Ŭ���̾�Ʈ���� ������ ��Ŷ������ ���̵��� �ʿ䰡 ����
//������ �������� �������� ������ ���̵����� ������ �� �ʿ䰡 ����.
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
struct CsPacketAttack //�÷��̾ ���ݹ�ư�� �������� ������ ��Ŷ�̴�.
{
	BYTE packetSize;
	BYTE packetType;
};

struct ScStatusInfo
{
	BYTE pakcetSize;
	BYTE packetType;
	int id;
	int attack; // �÷��̾� ���ݷ�
	int depend; //�÷��̾� ����
	int miss;//�÷��̾� ȸ�Ƿ�
	int level;//�÷��̾� ����
	int exp;//�÷��̾� ����ġ
	int str;//�÷��̾� ��
	int dex;//�÷��̾� ��ø
	int mental;//�÷��̾� ����
	int gold;//�÷��̾��� �������� ���
	int acr; //�÷��̿� ���߷�
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