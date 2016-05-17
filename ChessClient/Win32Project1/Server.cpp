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
	//boardsetting
	int x = 0;
	int y = 0;
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			if (i % 2 == 0)
			{
				if (j % 2 == 0)
					board[i][j].setType(1);
				else
					board[i][j].setType(0);
			}
			else
			{
				if (j % 2 == 0)
					board[i][j].setType(0);
				else
					board[i][j].setType(1);
			}
			board[i][j].setBoardx(x);
			board[i][j].setBoardy(y);
			x += WIDTHRECTANGLE;
		}
		y += HEIGHTRECTANGLE;
		x = 0;
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
	addr.sin_addr.s_addr = inet_addr(ip);
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
	cout << "recv" << endl;
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
void Server::KeyDown(WPARAM wParam)
{
	int x = 0, y = 0, retval = 0;
	DWORD iobyte;
	DWORD ioflag = 0;

	if (wParam == VK_UP) y += 1;
	if (wParam == VK_DOWN) y -= 1;
	if (wParam == VK_LEFT) x -= 1;
	if (wParam == VK_RIGHT) x += 1;

	CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(Send_buf);
	movePacket->packetSize = sizeof(movePacket);
	WSA_send_buf.len = sizeof(movePacket);
	if (0 != x)
	{
		if (x == 1)
		{
			movePacket->packetType = CS_RIGHT;
		}
		else
		{
			movePacket->packetType = CS_LEFT;
		}
		retval = WSASend(sock, &WSA_send_buf, 1, &iobyte, ioflag, NULL, NULL);
		//cout << iobyte << endl;
		if (retval == SOCKET_ERROR)
		{
			cout << "WSASend() x Error" << endl;
			cout << WSAGetLastError() << endl;
		}
	}
	if (0 != y)
	{
		if (y == 1)
		{
			movePacket->packetType = CS_UP;
		}
		else
		{
			movePacket->packetType = CS_DOWN;
		}
		retval = WSASend(sock, &WSA_send_buf, 1, &iobyte, ioflag, NULL, NULL);
		if (retval == SOCKET_ERROR)
		{
			cout << "WSASend() x Error" << endl;
			cout << WSAGetLastError() << endl;
		}
	}
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

		cout << "my_id : " << p->id << endl;
		break;
	}
	case SC_SET_PLAYER:
	{
		ScPacketPutPlayer *position =
			reinterpret_cast<ScPacketPutPlayer*>(buf);

		for (auto i = 1; i < MAX_PLAYER; ++i)
		{
			if (-1 == players[i].getID())
			{
				players[i].setID(position->id);
				players[i].setPositionX(position->position.x);
				players[i].setPositionY(position->position.y);
				break;
			}
			else if (position->id == players[i].getID())
			{
				players[i].setID(position->id);
				players[i].setPositionX(position->position.x);
				players[i].setPositionY(position->position.y);
				break;
			}
		}
		break;
	}
	case SC_MOVE_POSITION:
	{
		cout << "pos" << endl;
		ScPacketMove *pos =
			reinterpret_cast<ScPacketMove*>(buf);
		for (int i = 0; i < MAX_PLAYER; ++i)
		{
			if (pos->id == players[i].getID())
			{
				players[i].setPositionX(pos->position.x);
				players[i].setPositionY(pos->position.y);
				break;
			}
		}
		break;
	}
	case SC_REMOVE_PLAYER:
	{
		ScPacketRemovePlayer *remove=reinterpret_cast<ScPacketRemovePlayer*>(buf);
		for (auto i = 0; i < MAX_PLAYER; ++i)
		{
			if (remove->id == players[i].getID())
			{
				players[i].setPositionX(-10);
				players[i].setPositionY(-10);
			}
		}
		break;
	}
	}
}
void Server::SendPacket(SOCKET s, void* buf)
{
	SOCKET Send_socket = s;
	int packet_size = reinterpret_cast<char*>(buf)[0];
	memcpy(Complete_buf, buf, packet_size);

	WSA_Complete_buf.buf = Complete_buf;
	WSA_Complete_buf.len = packet_size;
	DWORD iobyte;
	WSASend(Send_socket, &WSA_Complete_buf, 1, &iobyte, 0, NULL, NULL);
}
