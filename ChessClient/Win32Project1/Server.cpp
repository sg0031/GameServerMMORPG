#include "stdafx.h"
#include "Server.h"


Server::Server()
{
	in_packet_size=0;
	save_packet_size=0;
}

Server::~Server()
{

}
int Server::socketinit(char* ip)
{
	int count = 0;
	for (auto i = OBJECT_START; i < MAX_OBJECT+ OBJECT_START; ++i)
	{
		objects[count].id = i;
		objects[count].isActive = false;
		if (i >= RABBIT_START)
		{
			objects[count].type = Rabbit;
		}
		else
		{
			objects[count].type = Stone;
		}
		count++;
	}
	WSADATA wsa;
	int ret = 0;
	//char ip[10];
	//cin >> ip;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "winsock init error" << endl;

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, 0);

	SOCKADDR_IN addr;

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	ret = WSAConnect(sock, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL);

	WSAAsyncSelect(sock, socketHWND, WM_SOCKET, FD_READ | FD_CLOSE);

	WSA_send_buf.buf = Send_buf;
	WSA_send_buf.len = MAXBUFSIZE;
	WSA_recv_buf.buf = Recv_buf;
	WSA_recv_buf.len = MAXBUFSIZE;

	DWORD iobyte;
	CsPacketLogin *login = reinterpret_cast<CsPacketLogin*>(Send_buf);
	login->packetSize = sizeof(CsPacketLogin);
	login->packetType = CS_LOGIN;

	ret = WSASend(sock, &WSA_send_buf, 1, &iobyte, 0, NULL, NULL);
	//cout << iobyte << endl;
	if (ret == SOCKET_ERROR)
		cout << "WSASend Error" << endl;

	return 1;
}

void Server::ReadPacket()
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &WSA_recv_buf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret != 0)
	{
		int err_code = WSAGetLastError();
		cout << "Recv Error : " << err_code << endl;
	}
	//cout << "recv" << endl;
	char *ptr = reinterpret_cast<char*>(Recv_buf);

	while (0 != iobyte)
	{
		if (in_packet_size == 0)
		{
			in_packet_size = ptr[0];
		}

		if (iobyte + save_packet_size >= in_packet_size)
		{
			memcpy(Complete_buf + save_packet_size,
				ptr,
				in_packet_size - save_packet_size);

			ProcessPacket(Complete_buf);

			ptr += in_packet_size - save_packet_size;
			iobyte -= in_packet_size - save_packet_size;
			in_packet_size = 0;
			save_packet_size = 0;
		}
		else
		{
			memcpy(Complete_buf + save_packet_size, ptr, iobyte);
			save_packet_size += iobyte;
			iobyte = 0;
		}

	}
}
void Server::requsetState()
{
	CsPacketLogin requset;
	requset.packetSize = sizeof(CsPacketLogin);
	requset.packetType = CS_STATE_UPDATE;
	SendPacket(sock, &requset);
}
void Server::KeyUp()
{
	CsPacketLogin keyup;
	keyup.packetSize = sizeof(CsPacketLogin);
	keyup.packetType = CS_STOP;
	SendPacket(sock, &keyup);
}
void Server::KeyDownAttack(WPARAM key)
{
	CsPacketLogin attack;
	attack.packetSize = sizeof(CsPacketLogin);
	if (key == 'A') {
		attack.packetType = CS_ATTACK_A;
		SendPacket(sock, &attack);
	}
}
void Server::KeyDown(WPARAM wParam)
{
	int x = 0, y = 0, retval = 0;
	DWORD iobyte;
	DWORD ioflag = 0;

	if (wParam == VK_UP) y += 1;
	if (wParam == VK_DOWN) y -= 1;
	if (wParam == VK_LEFT) x -= 1;
	if (wParam == VK_RIGHT) x += 1;

	CsPacketMove movePacket;
	movePacket.packetSize = sizeof(CsPacketMove);
	if (0 != x)
	{
		if (x == 1)
		{
			movePacket.packetType = CS_RIGHT;
		}
		else
		{
			movePacket.packetType = CS_LEFT;
		}
	}
	if (0 != y)
	{
		if (y == 1)
		{
			movePacket.packetType = CS_UP;
		}
		else
		{
			movePacket.packetType = CS_DOWN;
		}
	}
	SendPacket(sock, &movePacket);
}
void Server::ProcessPacket(char* buf)
{
	switch (buf[1])
	{
	case SC_LOGIN_COMPLTE:
	{
		ScPacketPlayerPosition *p =
			reinterpret_cast<ScPacketPlayerPosition*>(buf);

		players[0].setID(p->id);
		players[0].setPositionX(p->position.x);
		players[0].setPositionY(p->position.y);
		players[0].setConnect(true);
		cout << "my_id : " << p->id << endl;
		break;
	}
	case SC_SET_PLAYER:
	{
		ScPacketPutPlayer *position =
			reinterpret_cast<ScPacketPutPlayer*>(buf);
		//cout << "set : " << position->position.x<<","<<position->position.y<< endl;
		if (position->id < OBJECT_START)
		{
			for (auto i = 1; i < MAX_PLAYER; ++i)
			{
				if (-1 == players[i].getID())
				{
					players[i].setID(position->id);
					players[i].setPositionX(position->position.x);
					players[i].setPositionY(position->position.y);
					players[i].setHP(position->health);
					players[i].setState(position->state);
					players[i].setConnect(true);
					break;
				}
				else if (position->id == players[i].getID())
				{
					players[i].setPositionX(position->position.x);
					players[i].setPositionY(position->position.y);
					players[i].setHP(position->health);
					players[i].setState(position->state);
					players[i].setConnect(true);
					break;
				}
			}
		}
		else
		{
			for (auto i = 0; i < MAX_OBJECT; ++i)
			{
				if (position->id == objects[i].id)
				{
					objects[i].x = position->position.x;
					objects[i].y = position->position.y;
					objects[i].health = position->health;
					objects[i].state = position->state;
					objects[i].isActive = true;
					break;
				}
			}
		}
		break;
	}
	case SC_MOVE_POSITION:
	{
		ScPacketMove *pos =
			reinterpret_cast<ScPacketMove*>(buf);
		//cout << "pos : " << pos->position.x <<","<<pos->position.y<< endl;
		if (pos->id < OBJECT_START)
		{
			for (int i = 0; i < MAX_PLAYER; ++i)
			{
				if (pos->id == players[i].getID())
				{
					players[i].setPositionX(pos->position.x);
					players[i].setPositionY(pos->position.y);
					players[i].setHP(pos->health);
					players[i].setState(pos->state);
					break;
				}
			}
		}
		else
		{
			for (auto i = 0; i < MAX_OBJECT; ++i)
			{
				if (pos->id == objects[i].id)
				{
					objects[i].x = pos->position.x;
					objects[i].y = pos->position.y;
					objects[i].health = pos->health;
					objects[i].state = pos->state;
					break;
				}
			}
		}
		break;
	}
	case SC_REMOVE_PLAYER:
	{
		//cout << "remove" << endl;
		ScPacketRemovePlayer *remove = reinterpret_cast<ScPacketRemovePlayer*>(buf);
		if (remove->id < OBJECT_START)
		{
			for (auto i = 0; i < MAX_PLAYER; ++i)
			{
				if (remove->id == players[i].getID())
				{
					players[i].setPositionX(-10);
					players[i].setPositionY(-10);
					players[i].setConnect(false);
					break;
				}
			}
		}
		else
		{
			for (auto i = 0; i < MAX_OBJECT; ++i)
			{
				if (remove->id == objects[i].id)
				{
					objects[i].isActive = false;
					break;
				}
			}
		}
		break;
	}
	case SC_BUFF:
	{
		ScPacketBuff *buff = reinterpret_cast<ScPacketBuff*>(buf);
		for (auto i = 0; i < MAX_PLAYER; ++i)
		{
			if (buff->id == players[i].getID())
			{
				if (buff->buff == attackDown || buff->buff == dependDown)
					players[i].setDebuff(buff->buff);
				else
					players[i].setBuff(buff->buff);
				break;
			}
		}

		break;
	}
	case SC_STATE_UPDATE:
	{
		ScPacketMove *state = reinterpret_cast<ScPacketMove*>(buf);
		for (auto i = 0; i < MAX_PLAYER; ++i)
		{
			if (state->id == players[i].getID())
			{
				players[i].setHP(state->health);
				players[i].setState(state->state);
				break;
			}
		}
		break;
	}
	case SC_PLAYER_STATUS:
	{
		ScStatusInfo *stat = reinterpret_cast<ScStatusInfo*>(buf);
		for (auto i = 0; i < MAX_PLAYER; ++i)
		{
			if (stat->id == players[i].getID())
			{
				players[i].setAttack(stat->attack); // 플레이어 공격력
				players[i].setDepend(stat->depend); //플레이어 방어력
				players[i].setMiss(stat->miss);//플레이어 회피력
				players[i].setLevel(stat->level);//플레이어 레벨
				players[i].setExp(stat->exp);//플레이어 경험치
				players[i].setStr(stat->str);//플레이어 힘
				players[i].setDex(stat->dex);//플레이어 민첩
				players[i].setMental(stat->mental);//플레이어 지력
				players[i].setGold(stat->gold);//플레이어의 보유중인 골드
				players[i].setAcr(stat->acr); //플레이여 명중률
				players[i].setMaxHP(stat->maxHealth);
				players[i].setStatuCount(stat->statusCount);
				break;
			}
		}
		break;
	}
	//스위치케이스문 끝나는 부분
	}
}
void Server::SendPacket(SOCKET s, void* buf)
{
	DWORD iobyte;
	DWORD ioflag = 0;
	SOCKET Send_socket = s;
	int packet_size = reinterpret_cast<unsigned char*>(buf)[0];
	OverEx *Send_Operation = new OverEx;
	ZeroMemory(Send_Operation, sizeof(OverEx));

	Send_Operation->buf.buf = Send_Operation->packetBuf;
	Send_Operation->buf.len = packet_size;

	//wsaCompleteBuf.buf = completeBuf;
	//wsaCompleteBuf.len = packet_size;

	memcpy(Send_Operation->packetBuf, reinterpret_cast<char*>(buf), packet_size);

	int retval = WSASend(Send_socket, &Send_Operation->buf, 1, &iobyte, ioflag, NULL, NULL);
	//cout << iobyte << endl;
	if (retval == SOCKET_ERROR)
	{
		cout << "WSASend() x Error" << endl;
		cout << WSAGetLastError() << endl;
	}
	delete Send_Operation;
	//cout << "client data send" << endl;
}