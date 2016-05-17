#pragma once


class Server
{
	static HANDLE io;	//ºó Æ÷Æ® »ý¼º
	static int id;
	static int count;
	static Player players[MAX_PLAYER];
public:
	Server();
	~Server();
	static void acceptThread();
	static void lobbyThread();
	static void workerThread();
	static void processPacket(int id, char *ptr, double deltTime);
	static void sendPacket(int client, void* packet);
	static void viewListUpdate(int id);
	static Removeindex checkViewListPlayer(int id, int otherid);
};
