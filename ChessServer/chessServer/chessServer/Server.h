#pragma once


class Server
{
	static HANDLE io;	//ºó Æ÷Æ® »ý¼º
	static int id;
	static int count;
	static std::map<int, Player> players;
	static std::map<int, Object*> objects;
	static Sector gameMap[MAX_SECTOR_SIZE][MAX_SECTOR_SIZE];
	static TimerThread timer;
public:
	Server();
	~Server();
	static void acceptThread();
	static void workerThread();
	static void processPacket(int id,unsigned char *ptr, double deltTime);
	static void sendPacket(int client, void* packet);
	static void viewListUpdate(int id);
	static void updateSector(int id);
	static void monsterProcessPacket(int id);
	static bool attackCrushCheck(int player, int monster);
	static bool fireAttackCheck(int id, int Mon);
	static bool crushObject(int player, int object);
	static void sendPlayerStatus(int id);
};
