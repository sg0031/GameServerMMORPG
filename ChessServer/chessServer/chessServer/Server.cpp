#include "stdafx.h"
#include "Server.h"


HANDLE Server::io = NULL;	//�� ��Ʈ ����
int Server::id = 0;
//Player Server::players[MAX_PLAYER];
std::map<int, Player> Server::players;
std::map<int, Object*> Server::objects;
Sector Server::gameMap[MAX_SECTOR_SIZE][MAX_SECTOR_SIZE];
int Server::count = 0;
TimerThread Server::timer;
HandleManager *hManger = HandleManager::getInstance();

Server::Server()
{
	srand((unsigned)time(NULL));
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "�ʱ�ȭ error" << endl;

	SYSTEM_INFO is;
	GetSystemInfo(&is);

	vector<thread*> worker;

	hManger->gHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	for (auto i = 0; i < (int)is.dwNumberOfProcessors; ++i)
	{
		worker.push_back(new thread{ workerThread });
	}

	auto Acceptex = thread{ acceptThread };
	auto timerEx = thread{ &TimerThread::eventTimerThread,&timer };

	while (1)
	{
		Sleep(1000);
	}

	Acceptex.join();
	timerEx.join();

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

	//�ּ� ����
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	//bind �κ�
	int retval = 0;
	retval = ::bind(listenSock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(SOCKADDR));
	if (retval == SOCKET_ERROR)
		cout << "Bind Error" << endl;

	//���� ���� ����
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
		CreateIoCompletionPort((HANDLE)clientSock, hManger->gHandle, count, 0);
		//	cout << id << "�� ����" << endl;

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
void Server::workerThread()
{
	//��Ŷ �������̶�?
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
		GetQueuedCompletionStatus(hManger->gHandle, &ioSize, &objectId,
			reinterpret_cast<LPOVERLAPPED *>(&over), INFINITE);

		if (ioSize == 0)
		{
			//cout << iosize << endl;
			closesocket(over->s);
			players.erase(static_cast<int>(objectId)); //�÷��̾ ���ӿ��� �������� ����Ʈ���� ������� �Ѵ�.

			//WSAGetOverlappedResult
			//�������� ó��
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
					//����
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
					//�ʳ��� �����
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
		//�α��νÿ� ������ǥ�� �������� ������ ��ġ�� ����Ѵ�.
		int x = players[id].getPositionX() / DIVDIE_SECTOR;
		int y = players[id].getPositionY() / DIVDIE_SECTOR;
		gameMap[x][y].sLock.lock();
		gameMap[x][y].player.insert(id);
		gameMap[x][y].sLock.unlock();

		players[id].setPredSectorX(x);
		players[id].setPredSectorY(y);

		ScPacketPlayerPosition login;
		login.pakcetSize = sizeof(ScPacketPlayerPosition);
		login.packetType = SC_LOGIN_COMPLTE;
		login.id = players[id].getID();
		login.position.x = players[id].getPositionX();
		login.position.y = players[id].getPositionY();
		sendPacket(id, &login);

		updateSector(id);
		viewListUpdate(id);

		break;
	}
	case CS_RIGHT:
	{
		//	cout << "Right" << endl;
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int x = players[id].getPositionX();
		if (x + 1 > MAX_MAP_SIZE)
			x = MAX_MAP_SIZE;
		else x++;
		players[id].setPositionX(x);
		break;
	}
	case CS_LEFT:
	{
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int x = players[id].getPositionX();
		if (x - 1 < 0)
			x = 0;
		else x--;
		players[id].setPositionX(x);
		break;
	}
	case CS_UP:
	{
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int y = players[id].getPositionY();
		if (y - 1 < 0)
			y = 0;
		else y--;
		players[id].setPositionY(y);
		break;
	}
	case CS_DOWN:
	{
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		int y = players[id].getPositionY();
		if (y + 1 > MAX_MAP_SIZE)
			y = MAX_MAP_SIZE;
		else y++;
		players[id].setPositionY(y);
		break;
	}

	}
	//�Ź� �÷��̾���� ��ġ�� ����
	ScPacketMove packet;
	packet.pakcetSize = sizeof(ScPacketMove);
	packet.id = players[id].getID();
	packet.packetType = SC_MOVE_POSITION;
	packet.position.x = players[id].getPositionX();
	packet.position.y = players[id].getPositionY();
	sendPacket(id, &packet);

	updateSector(id);
	viewListUpdate(id);

	players[id].pLock.lock();
	for (auto i : players[id].pViewList)
		sendPacket(i, &packet);
	players[id].pLock.unlock();
}
void Server::updateSector(int id)
{
	int currX = players[id].getPositionX() / DIVDIE_SECTOR;
	int currY = players[id].getPositionY() / DIVDIE_SECTOR;
	int predX = players[id].getPredSectorX();
	int predY = players[id].getPredSectorY();

	if ((currX != predX) || (currY != predY)) //�������Ϳ� ���� ������
	{
		//std::cout << "����" << std::endl;
		players[id].setPredSectorX(currX);
		players[id].setPredSectorY(currY);
		gameMap[currX][currY].sLock.lock();
		if (0 == gameMap[currX][currY].player.count(id))
		{
			gameMap[currX][currY].player.insert(id);
			gameMap[currX][currY].sLock.unlock();
		}
		else gameMap[currX][currY].sLock.unlock();

		gameMap[predX][predY].sLock.lock();
		if (0 != gameMap[predX][predY].player.count(id))
		{
			gameMap[predX][predY].player.erase(id);
			gameMap[predX][predY].sLock.unlock();
		}
		else gameMap[predX][predY].sLock.unlock();
	}
}
void Server::viewListUpdate(int id)
{
	std::unordered_set<int> nearList;
	std::unordered_set<int> removeList;
	int startSectorX, startSectorY, endSectorX, endSectorY;

	startSectorX = (players[id].getPositionX() / DIVDIE_SECTOR)-1;
	startSectorY = (players[id].getPositionY() / DIVDIE_SECTOR)-1;
	endSectorX = (players[id].getPositionX() / DIVDIE_SECTOR)+1;
	endSectorY = (players[id].getPositionY() / DIVDIE_SECTOR)+1;

	if (startSectorX < 0)
		startSectorX = 0;
	if (startSectorY < 0)
		startSectorY = 0;
	if (endSectorX > MAX_SECTOR_SIZE)
		endSectorX = MAX_SECTOR_SIZE;
	if (endSectorY > MAX_SECTOR_SIZE)
		endSectorY = MAX_SECTOR_SIZE;
	//�þ��ֺ� ���͸� ���鼭 ���� ��ġ�� �������� �˻縦 �����Ѵ�.
	for (auto i = startSectorX; i <= endSectorX; i++)
	{
		for (auto j = startSectorY; j <= endSectorY; ++j)
		{
			if ((0 == gameMap[i][j].player.size()) && (0 == gameMap[i][j].object.size())) continue; //���Ϳ� �÷��̾ ������Ʈ�� �������� �ǳʶڴ�.
			gameMap[i][j].sLock.lock();
			for (auto p : gameMap[i][j].player) //�÷��̾ ����
			{
				if (id == p) continue; //�ڱ��ڽſ� ���Ѱ� �ǳ� �ڴ�.
				if (false == players[p].getPlay()) continue;	//�÷��̾��� �÷��� ���� �Ǵ�
				nearList.insert(p);
			}
			gameMap[i][j].sLock.unlock();

			gameMap[i][j].sLock.lock();
			for (auto m : gameMap[i][j].object) //������Ʈ�� ����
			{
				//if (false == objects[m]->getActive()) continue;
				nearList.insert(m);
			}
			gameMap[i][j].sLock.unlock();
		}
	}
	//nearlist�� ����ִ� ���̵𰪵��� ���� �丮��Ʈ�� ���ԵǾ��ִ��� �Ǵ��Ѵ�.
	//�����ϸ� �׳� �Ѿ�� �������� ������ �丮��Ʈ�� �߰��� �� �ش�.
	for (auto i : nearList)
	{
		if (i >= OBJECT_START)	//������Ʈ�� ���
		{
			players[id].pLock.lock();
			if (0==players[id].pObjectList.count(i))
			{
				players[id].pObjectList.insert(i);
				objects[i]->setActive(true); //�÷��̾��� �þ߿� ���Ա⋚���� ������Ʈ�� Ȱ�����·� �ٲ�����Ѵ�.
				players[id].pLock.unlock();	
			}
			else
			{
				players[id].pLock.unlock();
				continue;
			}
		}
		if (i < OBJECT_START)	//�÷��̾��� ���
		{
			players[id].pLock.lock();
			if (0 == players[id].pViewList.count(i))
			{
				players[id].pViewList.insert(i);
				players[id].pLock.unlock();
			}
			else
			{
				players[id].pLock.unlock();
				continue;
			}
		}
	}
	//�ٸ��÷��̾� ����Ʈ�� �ڽ��� �ֳ� Ȯ���ؾ� �Ѵ�.
	//�� �۾��� �÷��̾�鿡 ���ؼ� �ؾ��Ѵ�.
	for (auto i : nearList)
	{
		if (i < OBJECT_START) {
			players[i].pLock.lock();
			if (0 == players[i].pViewList.count(id))
			{
				players[i].pViewList.insert(id);
				players[i].pLock.unlock();
			}
			else
			{
				players[i].pLock.unlock();
				continue;
			}
		}
	}
	//���� nearlist�� ���µ� �丮��Ʈ�� �����ϴ� ���̵���� ������ ����Ʈ�� �Ȱ��ش�.
	players[id].pLock.lock();
	for (auto i : players[id].pViewList)
	{
		if (0 != nearList.count(i))	continue;
		removeList.insert(i);
	}
	for (auto i : players[id].pObjectList)
	{
		if (0 != nearList.count(i)) continue;
		removeList.insert(i);
	}

	for (auto i : removeList)
	{
		if (i >= OBJECT_START)
		{
			players[id].pObjectList.erase(i);
		}
		else
			players[id].pViewList.erase(i);
	}
	players[id].pLock.unlock();


	for (auto i : removeList) {
		if (i>=OBJECT_START) continue;
		players[i].pLock.lock();
		if (0 != players[i].pViewList.count(id)) {
			players[i].pViewList.erase(id);
			players[i].pLock.unlock();
		}
		else 	players[i].pLock.unlock();
	}


	//nearlist�� �ִ� ������ �ѷ��ش�.
	ScPacketPutPlayer put;
	put.packetSize = sizeof(ScPacketPutPlayer);
	put.packetType = SC_SET_PLAYER;
	for (auto i : nearList)
	{
		if (i >= OBJECT_START)	//������Ʈ�� ���
		{
			put.id = i;
			put.position.x = objects[i]->getPosX();
			put.position.y = objects[i]->getPosY();
			sendPacket(id, &put);
		}
		else				//�÷��̾��� ���
		{
			put.id = i;
			put.position.x = players[i].getPositionX();
			put.position.y = players[i].getPositionY();
			sendPacket(id, &put);
		}
	}

	for (auto i : nearList)
	{
		if (i >= OBJECT_START)
		{
			continue;	//������ ���� ��������.
		}
		else
		{
			put.id = id;
			put.position.x = players[i].getPositionX();
			put.position.y = players[i].getPositionY();
			sendPacket(i, &put);
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