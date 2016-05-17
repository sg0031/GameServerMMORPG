#include "stdafx.h"
#include "Server.h"

Server::Server()
{
	size = 0;
	//inPacketSize = new int;
	inPacketSize = &size;
	myId = 0;
	count = 0;
	for (int i = 0; i < SECTOR_WIDETH; ++i)
	{
		for (int j = 0; j < SECTOR_LENGTH; ++j)
		{
			sector[i][j].startSectorPosition.z = (j*80.0);
			sector[i][j].endSectorPosition.z = sector[i][j].startSectorPosition.z+80.0;
			sector[i][j].startSectorPosition.x = (i*60.0);
			sector[i][j].endSectorPosition.x = sector[i][j].startSectorPosition.x+60.0;
		}
	}
	ZeroMemory(&objectList, sizeof(objectList));
}
Server::~Server()
{

}
void Server::setSocketHWND(HWND s){
	socketHWND = s;
}
void Server::setHINSTANCE(HINSTANCE g)
{
	ghInst = g;
}
int Server::socketInit()
{
	WSADATA wsa;
	int ret = 0;

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

	wsaSendBuf.buf = sendBuf;
	wsaSendBuf.len = MAX_SIZE;
	wsaRecvBuf.buf = recvBuf;
	wsaRecvBuf.len = MAX_SIZE;

	DWORD iobyte;
	DWORD sendFlag=0;

	CsPacketLogin *login =
		reinterpret_cast<CsPacketLogin*>(sendBuf);
	login->packetSize = sizeof(CsPacketLogin);
	login->packetType = CS_LOGIN_REQUEST;

	ret = WSASend(sock, &wsaSendBuf, 1, &iobyte, sendFlag, NULL, NULL);
	cout << iobyte << endl;
	if (ret == SOCKET_ERROR)
		cout << "WSASend Error" << endl;
	cout << "Login Request" << endl;
	return 1;
}
void Server::keyDownAttacket(char key)
{
	CspacketAttack attack;
	attack.packetSize = sizeof(CspacketAttack);
	if (key == 'Q')
	{
		attack.packetType = CS_USE_SKILL_Q;
	}
	if (key == 'W')
	{
		attack.packetType = CS_USE_SKILL_W;
	}
	sendPacket(sock, &attack);
}
void Server::keyDown(WPARAM wParam)
{
	D3DXVECTOR3 playerDircetion;

	if (wParam == VK_UP)
	{
		playerDircetion = D3DXVECTOR3(0, 0, -1);
	}
	if (wParam == VK_DOWN)
	{
		playerDircetion = D3DXVECTOR3(0, 0, 1);
	}
	if (wParam == VK_LEFT)
	{
		playerDircetion = D3DXVECTOR3(-1, 0, 0);
	}
	if (wParam == VK_RIGHT)
	{
		playerDircetion = D3DXVECTOR3(1, 0, 0);
	}
	if (wParam == 'Q' || wParam == 'W')
	{
		keyDownAttacket(wParam);
	}

	CsPacketMove myPacket;
	myPacket.id = Player[0].getPlayerID();
	myPacket.packetSize = sizeof(CsPacketMove);
	myPacket.packetType = CS_MOVE;
	myPacket.direction = playerDircetion;
	sendPacket(sock, &myPacket);


}
void Server::readPacket()
{
	//DWORD iobyte, ioflag = 0;

	//int ret = WSARecv(sock, &wsaRecvBuf, 1, &iobyte, &ioflag, NULL, NULL);
	//if (ret != 0)
	//{
	//	int err_code = WSAGetLastError();
	//	cout << "Recv Error : " << err_code << endl;
	//}
	////cout << "recv" << endl;
	//char *ptr = reinterpret_cast<char*>(recvBuf);

	//while (0 != iobyte)
	//{
	//	if (inPacketSize == 0)
	//	{
	//		inPacketSize = reinterpret_cast<int*>(ptr);
	//	}

	//	if (iobyte + savePacketSize >= *inPacketSize)
	//	{
	//		memcpy(completeBuf + savePacketSize,
	//			ptr,
	//			*inPacketSize - savePacketSize);

	//		processPacket(completeBuf);

	//		ptr += *inPacketSize - savePacketSize;
	//		iobyte -= *inPacketSize - savePacketSize;
	//		inPacketSize = 0;
	//		savePacketSize = 0;
	//	}
	//	else
	//	{
	//		memcpy(completeBuf + savePacketSize, ptr, iobyte);
	//		savePacketSize += iobyte;
	//		iobyte = 0;
	//	}
	//}
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &wsaRecvBuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret != 0)
	{
		int err_code = WSAGetLastError();
		cout << "Recv Error : " << err_code << endl;
	}
	//cout << "recv" << endl;
	char *ptr = reinterpret_cast<char*>(recvBuf);

	while (0 != iobyte)
	{
		if (size == 0)
		{
			inPacketSize = reinterpret_cast<int*>(ptr);
			size = *inPacketSize;
		}

		if (iobyte + savePacketSize >= size)
		{
			memcpy(completeBuf + savePacketSize,
				ptr,
				size - savePacketSize);

			processPacket(completeBuf);

			ptr += size - savePacketSize;
			iobyte -= size - savePacketSize;
			size = 0;
			savePacketSize = 0;
		}
		else
		{
			memcpy(completeBuf + savePacketSize, ptr, iobyte);
			savePacketSize += iobyte;
			iobyte = 0;
		}
	}
}
void Server::processPacket(char* ptr)
{
	BYTE *header = reinterpret_cast<BYTE*>(ptr + 4);
	//cout << "process" << endl;
	switch (*header)
	{
	case SC_LOGIN_SUCCESS:
	{
		ScPacketMove *login =
			reinterpret_cast<ScPacketMove*>(ptr);
		myId = login->id;
		Player[0].setPlayerID(myId);
		Player[0].setPlayerPosition(login->position);
		Player[0].setState(login->state);
		Player[0].setPlay(true);
		cout << "myId : " << Player[0].getPlayerID() << endl;
		break;
	}
	case SC_PLAYER_LIST:
	{
		ScPacketAcceptPlayerList *list =
			reinterpret_cast<ScPacketAcceptPlayerList*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (list->id == Player[0].getPlayerID())
			{
				Player[i].setPlayerPosition(list->position);
				break;
			}
			else if (-1==Player[i].getPlayerID())
			{
				Player[i].setPlayerID(list->id);
				Player[i].setPlayerPosition(list->position);
				Player[i].setPlay(true);
				break;
			}
		}
		break;
	}
	case SC_MOVE_POSITION:
	{
//		cout << "movePacket" << endl;
		ScPacketMove *move =
			reinterpret_cast<ScPacketMove*>(ptr);
		//cout << p->id << "," << p->x << "," << p->y << endl;
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (Player[i].getPlayerID() == move->id)
			{
				Player[i].setPlayerPosition(move->position);
				Player[i].setPlayerDirection(move->direction);
				Player[i].setState(move->state);
				break;
			}
		}
		//Player[move->id].setPlayerPosition(move->position);
		break;
	}

	case SC_MOVE_ERROR_CHECK:
	{
		cout << "�̵� ����ȭ üũ" << endl;
		ScPacketMove *check = reinterpret_cast<ScPacketMove*>(ptr);
		break;
	}
	case SC_SECTOR_UPDATE:
	{
		ScPacketObject *object = reinterpret_cast<ScPacketObject*>(ptr);
		ZeroMemory(&objectList, sizeof(objectList));
		memcpy_s(&objectList, sizeof(objectList), &object->objects, sizeof(object->objects));
		break;
	}
	case SC_PUT_PLAYER:
	{
		ScPacektPutPlayer *put = reinterpret_cast<ScPacektPutPlayer*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (put->id == Player[i].getPlayerID())
			{
				Player[i].setPlayerPosition(put->position);
				Player[i].setPlayerDirection(put->direction);
				Player[i].setState(put->state);
				Player[i].setPlay(true);
				break;
			}
			else if (-1 == Player[i].getPlayerID())
			{
				Player[i].setPlayerPosition(put->position);
				Player[i].setPlayerDirection(put->direction);
				Player[i].setPlayerID(put->id);
				Player[i].setState(put->state);
				Player[i].setPlay(true);
				break;
			}
		}
		break;
	}
	case SC_REMOVE_PLAYER:
	{
		ScPacketRemoveObject *remove = reinterpret_cast<ScPacketRemoveObject*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (remove->id == Player[i].getPlayerID())
			{
				Player[i].setPlayerPosition(D3DXVECTOR3(-10.0, 0, -10.0));
				Player[i].setPlay(false);
				break;
			}
		}
		break;
	}
	case SC_STATE_UPDATE:
	{
		ScPacketUpdateState *stateup = reinterpret_cast<ScPacketUpdateState*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (stateup->id == Player[i].getPlayerID())
			{
				Player[i].setState(stateup->state);
				break;
			}
		}
		break;
	}
	case SC_MONSTER_UPDATE:
	{
		ScPacketMonsterList *mon = reinterpret_cast<ScPacketMonsterList*>(ptr);
		ZeroMemory(&monsterList, sizeof(monsterList));
		memcpy_s(&monsterList, sizeof(monsterList), &mon->monster, sizeof(mon->monster));
		break;
	}
	case SC_MONSTER_POS:
	{
		ScPacketMonsterPos *monMove = reinterpret_cast<ScPacketMonsterPos*>(ptr);
		for (int i = 0; i < 100; ++i)
		{
			if (monMove->monsterID == i)
			{
				monsterList[i].monsterPos = monMove->position;
				monsterList[i].monsterDir = monMove->direction;
				break;
			}
		}
		break;
	}
	case SC_MONSTER_STATE_UPDATE:
	{
		ScPacketMonsterState *monState = reinterpret_cast<ScPacketMonsterState*>(ptr);
		for (int i = 0; i < 100; ++i)
		{
			if (monState->monsterID == i)
			{
				monsterList[i].state = monState->monsterState;
				break;
			}
		}
		break;
	}
	}
}
void Server::sendPacket(SOCKET s, void* buf)
{
	DWORD iobyte;
	DWORD ioflag = 0;
	SOCKET Send_socket = s;
	int *packet_size = reinterpret_cast<int*>(buf);
	OverEx *Send_Operation = new OverEx;
	ZeroMemory(Send_Operation, sizeof(OverEx));
	
	Send_Operation->buf.buf = Send_Operation->packetBuf;
	Send_Operation->buf.len = *packet_size;

	//wsaCompleteBuf.buf = completeBuf;
	//wsaCompleteBuf.len = packet_size;

	memcpy(Send_Operation->packetBuf, reinterpret_cast<char*>(buf), *packet_size);

	int retval = WSASend(Send_socket, &Send_Operation->buf, 1, &iobyte, ioflag, NULL, NULL);
	//cout << iobyte << endl;
	if (retval == SOCKET_ERROR)
	{
		cout << "WSASend() x Error" << endl;
		cout << WSAGetLastError() << endl;
	}
	//cout << "client data send" << endl;
}
int Server::getMyId()
{
	return myId;
}