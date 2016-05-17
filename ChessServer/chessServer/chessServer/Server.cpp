#include "stdafx.h"
#include "Server.h"


HANDLE Server::io = NULL;	//빈 포트 생성
int Server::id = 0;
Player Server::players[MAX_PLAYER];
int Server::count = 0;


Server::Server()
{
	srand((unsigned)time(NULL));
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "초기화 error" << endl;

	SYSTEM_INFO is;
	GetSystemInfo(&is);

	vector<thread*> worker;

	io = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	for (auto i = 0; i < (int)is.dwNumberOfProcessors; ++i)
	{
		worker.push_back(new thread{ workerThread });
	}

	auto Acceptex = thread{ acceptThread };
	auto Lobby = thread{ lobbyThread };

	while (1)
	{
		Sleep(1000);
	}

	Acceptex.join();
	Lobby.join();

	for (auto t : worker)
	{
		t->join();
	}
}


Server::~Server()
{
}

void Server::acceptThread()
{
	cout << "ChessServerInit" << endl;

	SOCKET listenSock;
	SOCKADDR_IN  addr;

	listenSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,
		NULL, 0, WSA_FLAG_OVERLAPPED);

	//주소 설정
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	//bind 부분
	int retval = 0;
	retval = ::bind(listenSock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(SOCKADDR));
	if (retval == SOCKET_ERROR)
		cout << "Bind Error" << endl;

	//리슨 소켓 설정
	retval = listen(listenSock, SOMAXCONN);
	if (retval == INVALID_SOCKET)
	{
		cout << "listen_sock error" << endl;
	}

	SOCKET clientSock;
	SOCKADDR_IN clientAddr;

	clientSock = NULL;

	while (1)
	{
		//	myLock.lock();
		int len = sizeof(clientAddr);
		clientSock = WSAAccept(listenSock, (SOCKADDR*)&clientAddr,
			&len, NULL, NULL);

		if (clientSock == INVALID_SOCKET)
			cout << "Client Socket Error" << endl;

		
		players[count].overEx->s = clientSock;
		CreateIoCompletionPort((HANDLE)clientSock, io, count, 0);
		//	cout << id << "명 접속" << endl;

		unsigned long recvflag = 0;
		unsigned long ioByte = 0;
		int ret = 0;
		ret = WSARecv(clientSock, &players[count].overEx->buf,
			1, NULL, &recvflag,
			reinterpret_cast<LPOVERLAPPED>(&players[count].overEx->overLapped), NULL);
		if (ret == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				cout << "error code : " << WSAGetLastError() << endl;
		}
		count++;
		//cout << count << endl;
		//	myLock.unlock();
	}
}
void Server::lobbyThread()
{

}
void Server::workerThread()
{
	//패킷 재조립이란?
	int retval;
	unsigned long ioSize;
	unsigned long recvSize = 0;
	unsigned long objectId;
	unsigned long ioFlag = 0;
	OverEx *over = nullptr;
	double currentTime = 0.0;

	currentTime = clock();

	while (1)
	{
		GetQueuedCompletionStatus(io, &ioSize, &objectId,
			reinterpret_cast<LPOVERLAPPED *>(&over), INFINITE);

		if (ioSize == 0)
		{
			//cout << iosize << endl;
			closesocket(over->s);
			//WSAGetOverlappedResult
			//게임종료 처리
		}

		if (over->operationType == Recvtype)
		{
			//cout << iosize << endl;
			//cout << "Recvtype" << endl;
			char *buf = over->iocpBuf;	//?
			unsigned long dataToProcess = ioSize;
			over->currentSize = dataToProcess;

			while (dataToProcess > 0)
			{
				if (over->currentSize == 0)
					over->currentSize = buf[0];

				unsigned int needToBuild =
					over->currentSize - over->prevSize;

				if (dataToProcess >= needToBuild)
				{
					double newTime = clock();
					double frameTime = newTime - currentTime;
					if (frameTime > 0.25)
						frameTime = 0.25;
					currentTime = newTime;
					//accumulator += frameTime;
					//조립
					memcpy(over->packetBuf + over->prevSize, buf, needToBuild);
					//cout << static_cast<int>(Object_ID) << endl;
					processPacket(static_cast<int>(objectId), over->packetBuf, frameTime);
					//t += dt;
					//accumulator -= dt;
					over->currentSize = 0;
					over->prevSize = 0;
					buf += needToBuild;
					dataToProcess -= needToBuild;
				}
				else
				{
					//훗날을 기약해
					memcpy(over->packetBuf + over->prevSize, buf, dataToProcess);
					over->prevSize += dataToProcess;
					dataToProcess = 0;
				}

			}
			retval = WSARecv(over->s, &over->buf, 1, &recvSize, &ioFlag, &over->overLapped, NULL);
		}

		else if (over->operationType == Sendtype)
		{
			delete over;
		}
	}
}
void Server::processPacket(int id, char *ptr, double deltaTime)
{
	switch (ptr[1])
	{
	case CS_LOGIN:
	{
		cout << "Login Accept" << endl;
		players[id].setID(id);
		cout << "id : " << players[id].getID() << endl;
		players[id].setPlay(true);

		ScPacketPlayerPosition login;
		login.pakcetSize = sizeof(ScPacketPlayerPosition);
		login.packetType = SC_LOGIN_COMPLTE;
		login.id = players[id].getID();
		login.position.x = players[id].getPositionX();
		login.position.y = players[id].getPositionY();
		sendPacket(id, &login);
		viewListUpdate(id);
		break;
	}
	case CS_RIGHT:
	{
	//	cout << "Right" << endl;
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int y = players[id].getPositionY();
		players[id].setPositionY(++y);
		viewListUpdate(id);
		ScPacketMove packet;
		packet.pakcetSize = sizeof(ScPacketMove);
		packet.id = players[id].getID();
		packet.packetType = SC_MOVE_POSITION;
		packet.position.x = players[id].getPositionX();
		packet.position.y = players[id].getPositionY();
		sendPacket(id, &packet);

		for (auto p = 0; p < MAX_PLAYER; ++p)
		{
			if (-1 != players[id].viewList[p])
			{
				sendPacket(players[id].viewList[p], &packet);
			}
		}
		break;
	}
	case CS_LEFT:
	{
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int y = players[id].getPositionY();
		players[id].setPositionY(--y);
		viewListUpdate(id);
		ScPacketMove packet;
		packet.pakcetSize = sizeof(ScPacketMove);
		packet.id = players[id].getID();
		packet.packetType = SC_MOVE_POSITION;
		packet.position.x = players[id].getPositionX();
		packet.position.y = players[id].getPositionY();
		sendPacket(id, &packet);

		for (auto p = 0; p < MAX_PLAYER; ++p)
		{
			if (-1 != players[id].viewList[p])
			{
				sendPacket(players[id].viewList[p], &packet);
			}
		}
		break;
	}
	case CS_UP:
	{
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int x = players[id].getPositionX();
		players[id].setPositionX(--x);
		viewListUpdate(id);
		ScPacketMove packet;
		packet.pakcetSize = sizeof(ScPacketMove);
		packet.id = players[id].getID();
		packet.packetType = SC_MOVE_POSITION;
		packet.position.x = players[id].getPositionX();
		packet.position.y = players[id].getPositionY();
		sendPacket(id, &packet);

		for (auto p = 0; p < MAX_PLAYER; ++p)
		{
			if (-1 != players[id].viewList[p])
			{
				sendPacket(players[id].viewList[p], &packet);
			}
		}
		break;
	}
	case CS_DOWN:
	{
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int x = players[id].getPositionX();
		players[id].setPositionX(++x);

		viewListUpdate(id);

		ScPacketMove packet;
		packet.pakcetSize = sizeof(ScPacketMove);
		packet.id = players[id].getID();
		packet.packetType = SC_MOVE_POSITION;
		packet.position.x = players[id].getPositionX();
		packet.position.y = players[id].getPositionY();
		sendPacket(id, &packet);

		for (auto p = 0; p < MAX_PLAYER; ++p)
		{
			if (-1 != players[id].viewList[p])
			{
				sendPacket(players[id].viewList[p], &packet);
			}
		}
		break;
	}
	}

	//매번 플레이어들의 위치값 갱신

}
Removeindex Server::checkViewListPlayer(int id, int otherid)
{
	//id가 otherid의 리스트에 등록되어 있으면
	Removeindex index;
	for (auto i = 0; i < MAX_PLAYER; ++i)
	{
		if (id == players[otherid].viewList[i])
		{
			index.Bol = true;
			index.index = i;
			return index;
			break;
		}
	}
	index.Bol = false;
	return index;
}
void Server::viewListUpdate(int id)
{
	Removeindex nearCheck1,nearCheck2,removeCheck1,removeCheck2;
	int nearCount = 0, removeCount = 0;
	int nearList[10], removeList[10];
	for (auto n = 0; n < 10; ++n) {
		nearList[n] = -1; removeList[n] = -1;
	}
	int startViewX = 0, startViewY = 0, endViewX = 0, endViewY = 0;
	startViewX = players[id].getPositionX() - 2;
	startViewY = players[id].getPositionY() - 2;
	endViewX = players[id].getPositionX() + 2;
	endViewY = players[id].getPositionY() + 2;

	if (startViewX < 0)
		startViewX = 0;
	if (startViewY < 0)
		startViewY = 0;
	if (endViewX > 100)
		endViewX = 100;
	if (endViewY > 100)
		endViewY = 100;

	for (auto p = 0; p < MAX_PLAYER; ++p)
	{
		if (false == players[p].getPlay()) continue;
		if (id == p) continue;
		//플레이어가 게임을 플레이하는 중일 경우 체크를 해본다.
		if ((startViewX <= players[p].getPositionX()) && (startViewY <= players[p].getPositionY()) &&
			(endViewX >= players[p].getPositionX()) && (endViewY >= players[p].getPositionY()))
		{
			//시야 범위안에 플레이어가 존재할 경우
			//nearlist 업데이트
			nearList[nearCount] = p;
			nearCount++;
			nearCheck1 = checkViewListPlayer(id, p);
			nearCheck2 = checkViewListPlayer(p, id);
		
			if (true== nearCheck1.Bol)
			{
				//내가 상대리스트에 존재하고
				if (false == nearCheck2.Bol)
				{
					//내리스트에 상대가 존재하지 않는다고 하면 상대를 리스트에 추가
					for (auto u = 0; u < MAX_PLAYER; ++u)
					{
						if (-1 == players[id].viewList[u])
						{
							players[id].viewList[u] = p;
							break;
						}
					}
				}
			}
			else if (false == nearCheck1.Bol)
			{
				//내가 상대리스트에 없다면 나를 상대리스트에 추가한다
				for (auto u = 0; u < MAX_PLAYER; ++u)
				{
					if (-1 == players[p].viewList[u])
					{
						players[p].viewList[u] = id;
						break;
					}
				}
			}
		}
		else
		{
			//시야에 존재 하지 않을경우
			removeList[removeCount] = p;
			removeCount++;
			removeCheck1 = checkViewListPlayer(id, p);
			removeCheck2 = checkViewListPlayer(p, id);
			//내가 상대리스트에 존재할 경우
			if (true == removeCheck1.Bol)
			{
				players[p].viewList[removeCheck1.index] = -1;
			}
			//상대리스트에 내가 존재할 경우
			if (true == removeCheck2.Bol)
			{
				players[id].viewList[removeCheck2.index] = -1;
			}
		}
	}
	ScPacketPutPlayer put;
	put.packetSize = sizeof(ScPacketPutPlayer);
	put.packetType = SC_SET_PLAYER;
	for (auto n = 0; n < nearCount; ++n)
	{
		//nearlist putplayer 패킷을 전송
		put.id = nearList[n];
		put.position.x = players[put.id].getPositionX();
		put.position.y = players[put.id].getPositionY();
		sendPacket(id, &put);
	}
	for (auto i = 0; i < nearCount; ++i)
	{
		if (-1 != nearList[i])
		{
			put.id = id;
			put.position.x = players[id].getPositionX();
			put.position.y = players[id].getPositionY();
			sendPacket(nearList[i], &put);
		}
	}
	ScPacketRemovePlayer remove;
	remove.packetSize = sizeof(ScPacketRemovePlayer);
	remove.packetType = SC_REMOVE_PLAYER;
	for (auto r = 0; r < removeCount; ++r)
	{
		//removePlayer 패킷을 전송
		remove.id = removeList[r];
		sendPacket(id, &remove);
	}
	for (auto j = 0; j < removeCount; ++j)
	{
		if (-1 != removeList[j])
		{
			remove.id = id;
			sendPacket(removeList[j], &remove);
		}
	}
}

void Server::sendPacket(int client, void* packet)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	OverEx *Send_Operation = new OverEx;
	ZeroMemory(&Send_Operation->overLapped, sizeof(WSAOVERLAPPED));

	Send_Operation->operationType = Sendtype;

	Send_Operation->buf.buf = Send_Operation->packetBuf;
	Send_Operation->buf.len = packet_size;

	memcpy(Send_Operation->packetBuf, reinterpret_cast<char*>(packet), packet_size);

	DWORD iobyte;
	DWORD sendFlag = 0;
	int retval;
	retval = WSASend(players[client].overEx->s, &Send_Operation->buf, 1,
		&iobyte, sendFlag, &Send_Operation->overLapped, NULL);
	if (retval != 0)
	{
		cout << GetLastError() << endl;
		cout << "error sendpacket" << endl;
	}
}