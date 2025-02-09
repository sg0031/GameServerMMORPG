#include "stdafx.h"
#include "Server.h"


HANDLE Server::io = NULL;	//빈 포트 생성
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
	for (auto i = OBJECT_START; i < MAX_OBJECT+ OBJECT_START; ++i)
	{
		if (i >= RABBIT_START)
		{
			objects[i] = new Rabbit;

			int x = (rand() % 1960)+40;
			int y = (rand() % 1960)+40;
			objects[i]->setID(i);
			objects[i]->setPosX(x);
			objects[i]->setPosY(y);
			objects[i]->setReturnPos(x, y);
			objects[i]->setActive(false);
			gameMap[x / DIVDIE_SECTOR][y / DIVDIE_SECTOR].object.insert(i);

		}
		//if (i >= ARGO_START)
		//{
		//	objects[i] = new Agro;
		//	int x = rand() % 2000;
		//	int y = rand() % 2000;
		//	objects[i]->setPosX(x);
		//	objects[i]->setPosY(y);
		//	objects[i]->setActive(false);
		//	gameMap[x / DIVDIE_SECTOR][y / DIVDIE_SECTOR].object.insert(i);
		//}
		//if (i >= BABY_START)
		//{
		//	objects[i] = new Baby;
		//	int x = rand() % 2000;
		//	int y = rand() % 2000;
		//	objects[i]->setPosX(x);
		//	objects[i]->setPosY(y);
		//	objects[i]->setActive(false);
		//	gameMap[x / DIVDIE_SECTOR][y / DIVDIE_SECTOR].object.insert(i);
		//	for (auto j = i+1; j < i + 6; ++j) {
		//		objects[j] = new BabyGuard;
		//		int x = rand() % 2000;
		//		int y = rand() % 2000;
		//		objects[j]->setPosX(x);
		//		objects[j]->setPosY(y);
		//		objects[j]->setActive(false);
		//		gameMap[x / DIVDIE_SECTOR][y / DIVDIE_SECTOR].object.insert(j);
		//	}	
		//	i += 5;
		//}
		else
		{
			objects[i] = new Stone;
			int x = rand() % 2000;
			int y = rand() % 2000;
			objects[i]->setID(i);
			objects[i]->setPosX(x);
			objects[i]->setPosY(y);
			objects[i]->setActive(false);
			gameMap[x / DIVDIE_SECTOR][y / DIVDIE_SECTOR].object.insert(i);
	/*		int ran = rand() % 2;
			if (0 == ran / 2)
			{
				objects[i] = new Stone;
				int x = rand() % 2000;
				int y = rand() % 2000;
				objects[i]->setPosX(x);
				objects[i]->setPosY(y);
				objects[i]->setActive(false);
				gameMap[x / DIVDIE_SECTOR][y / DIVDIE_SECTOR].object.insert(i);
			}
			else
			{
				objects[i] = new Tree;
				int x = rand() % 2000;
				int y = rand() % 2000;
				objects[i]->setPosX(x);
				objects[i]->setPosY(y);
				objects[i]->setActive(false);
				gameMap[x / DIVDIE_SECTOR][y / DIVDIE_SECTOR].object.insert(i);
			}*/

		}
	}
	srand((unsigned)time(NULL));
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "초기화 error" << endl;

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
		CreateIoCompletionPort((HANDLE)clientSock, hManger->gHandle, count, 0);
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
		GetQueuedCompletionStatus(hManger->gHandle, &ioSize, &objectId,
			reinterpret_cast<LPOVERLAPPED *>(&over), INFINITE);

		if (ioSize == 0)
		{
			//cout << iosize << endl;
			closesocket(over->s);
			players.erase(static_cast<int>(objectId)); //플레이어가 게임에서 나갔을때 리스트에서 지워줘야 한다.

			//WSAGetOverlappedResult
			//게임종료 처리
		}

		if (over->operationType == Recvtype)
		{
			//cout << iosize << endl;
			//cout << "Recvtype" << endl;
			unsigned char *buf = over->iocpBuf;	//?
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
					frameTime = frameTime / 1000;
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
		else if (over->operationType == MonsterUpdate)
		{
			//objects[static_cast<int>(objectId)]->upDate();		//몬스터 상태 업데이트
			monsterProcessPacket(static_cast<int>(objectId));   //몬스터 업데이트후에 패킷을 뿌려줌

			delete over;
		}
		else if (over->operationType == MonsterRespon)
		{
			int monID = static_cast<int>(objectId);
			objects[monID]->responHP();
			objects[monID]->setLevel(rand()%10+5);
			objects[monID]->setState(waitState);
			objects[monID]->setTarget(-1);
			timer.AddGameEvent(MonsterUpdate, monID, 1000);
			//cout << monID << "부활" << endl;
			delete over;
		}
		else if (over->operationType == playersHPincrease)
		{
			int ID = static_cast<int>(objectId);
			players[ID].increaseHP();
			ScPacketMove health;
			health.pakcetSize = sizeof(ScPacketMove);
			health.packetType = SC_STATE_UPDATE;
			health.health = players[ID].getHp();
			health.id = ID;
			sendPacket(ID, &health);
			if (0 != players[ID].pViewList.size())
				for (auto i : players[ID].pViewList)
					sendPacket(i, &health);
			timer.AddGameEvent(playersHPincrease, id, 10000);
			delete over;
		}
	}
}
void Server::monsterProcessPacket(int id)
{
	//objects[id]->upDate();
	unordered_set<int> nearList;
	D3DXVECTOR2 targetPos;

	ScPacketMove packet;
	packet.pakcetSize = sizeof(ScPacketMove);
	packet.id = id;
	packet.packetType = SC_MOVE_POSITION;
	packet.position.x = objects[id]->getPosX();
	packet.position.y = objects[id]->getPosY();
	packet.state = objects[id]->getState();

	int startSectorX = (objects[id]->getPosX() / DIVDIE_SECTOR) - 1;
	int startSectorY = (objects[id]->getPosY() / DIVDIE_SECTOR) - 1;
	int endSectorX = (objects[id]->getPosX() / DIVDIE_SECTOR) + 1;
	int endSectorY = (objects[id]->getPosY() / DIVDIE_SECTOR) + 1;

	if (startSectorX < 0)
		startSectorX = 0;
	if (startSectorY < 0)
		startSectorY = 0;
	if (endSectorX > MAX_SECTOR_SIZE)
		endSectorX = MAX_SECTOR_SIZE;
	if (endSectorY > MAX_SECTOR_SIZE)
		endSectorY = MAX_SECTOR_SIZE;
	//시야주변 섹터를 돌면서 현재 위치를 바탕으로 검사를 진행한다.
	for (auto i = startSectorX; i <= endSectorX; i++)
	{
		for (auto j = startSectorY; j <= endSectorY; ++j)
		{
			if (0 == gameMap[i][j].player.size()) continue;
			for (auto i : gameMap[i][j].player)
			{
				nearList.insert(i);
			}
		}
	}
	if (0 != nearList.size()) {
		for (auto i : nearList) {
			if (-1 == objects[id]->getTarget()) {
				objects[id]->setTarget(i);
				targetPos.x = players[i].getPositionX();
				targetPos.y = players[i].getPositionY();
				objects[id]->setTagetPos(targetPos.x, targetPos.y);
				if (false == objects[id]->chaseRange()) {
					objects[id]->setTarget(-1);
					continue;
				}
				else if (true == objects[id]->chaseRange())
					break;
			}
		}
	}
	int tarID=objects[id]->getTarget();
	if (-1 != tarID) {
		targetPos.x = players[tarID].getPositionX();
		targetPos.y = players[tarID].getPositionY();
		objects[id]->setTagetPos(targetPos.x, targetPos.y);
	}
	objects[id]->upDate();
	if (true == objects[id]->hitDamge() && deadState != objects[id]->getState()) {
		//플레이어가 몬스터한테맞았을떄 hp를 뿌려줌
		players[objects[id]->getTarget()].decreaseHP(objects[id]->getAttack());
		ScPacketMove health;
		health.pakcetSize = sizeof(ScPacketMove);
		health.packetType = SC_STATE_UPDATE;
		health.health = players[objects[id]->getTarget()].getHp();
		health.id = objects[id]->getTarget();
		sendPacket(objects[id]->getTarget(), &health);
		if(0!=players[objects[id]->getTarget()].pViewList.size())
			for(auto i: players[objects[id]->getTarget()].pViewList)
				sendPacket(i, &health);

		ScPacketStateMessage meg;
		meg.id = objects[id]->getTarget();
		meg.size = sizeof(ScPacketStateMessage);
		meg.type = SC_MONSTER_ATTACK_PLAYER;
		meg.damage = objects[id]->getAttack();
		meg.monsterId = id;
		sendPacket(objects[id]->getTarget(), &meg);
	}
	for (auto i : nearList)
		sendPacket(i, &packet);
	if (0 != nearList.size()) {
		if (deadState == objects[id]->getState()) {
			ScPacketMove packet;
			packet.pakcetSize = sizeof(ScPacketMove);
			packet.id = id;
			packet.packetType = SC_MOVE_POSITION;
			packet.position.x = objects[id]->getPosX();
			packet.position.y = objects[id]->getPosY();
			packet.state = objects[id]->getState();
			for (auto i : nearList)
				sendPacket(i, &packet);
			timer.AddGameEvent(MonsterRespon, id, 30000);
		}
		else
			timer.AddGameEvent(MonsterUpdate, id, 1000);
	}
	else if (true == nearList.empty()) objects[id]->setActive(false);
}
void Server::processPacket(int id,unsigned char *ptr, double deltaTime)
{
	int crushCount = 0;
	int listSize = players[id].pObjectList.size();
	switch (ptr[1])
	{
	case CS_LOGIN:
	{
		cout << "Login Accept" << endl;
		players[id].setID(id);
		cout << "id : " << players[id].getID() << endl;
		players[id].setPlay(true);
		//로그인시에 현재좌표를 기준으로 섹터의 위치를 기록한다.
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
		timer.AddGameEvent(playersHPincrease, id, 5000);
		break;
	}
	case CS_RIGHT:
	{
		D3DXVECTOR2 dir = { 1,0 };
		D3DXVECTOR2 pos;
		//	cout << "Right" << endl;
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		pos.x= players[id].getPositionX();
		pos.y = players[id].getPositionY();
		if (pos.x + 1 > MAX_MAP_SIZE)
			pos.x = MAX_MAP_SIZE;
		//for (auto i : players[id].pObjectList) {
		//	if (i < 1500) continue;
		//	if (true == crushObject(id, i))
		//		count++;
		//}
		//if(0==count)
		pos = pos + (MAX_SPEED * dir);
		players[id].setPositionX(pos.x);
		players[id].setPositionY(pos.y);
		players[id].setState(walkPlayer);
		break;
	}
	case CS_LEFT:
	{
		D3DXVECTOR2 dir = { -1,0 };
		D3DXVECTOR2 pos;
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		pos.x = players[id].getPositionX();
		pos.y = players[id].getPositionY();
		if (pos.x - 1 < 0)
			pos.x = 0;
		//for (auto i : players[id].pObjectList) {
		//	if (i < 1500) continue;
		//	if (true == crushObject(id, i))
		//		count++;
		//}
		//if (0 == count)
		pos = pos + (MAX_SPEED * dir);
		players[id].setPositionX(pos.x);
		players[id].setPositionY(pos.y);
		players[id].setState(walkPlayer);
		break;
	}
	case CS_UP:
	{
		D3DXVECTOR2 dir = { 0,-1 };
		D3DXVECTOR2 pos;
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		pos.x = players[id].getPositionX();
		pos.y = players[id].getPositionY();
		if (pos.y - 1 < 0)
			pos.y = 0;
		//for (auto i : players[id].pObjectList) {
		//	if (i < 1500) continue;
		//	if (true == crushObject(id, i))
		//		count++;
		//}
		//if (0 == count)
		pos = pos + (MAX_SPEED * dir);
		players[id].setPositionX(pos.x);
		players[id].setPositionY(pos.y);
		players[id].setState(walkPlayer);
		break;
	}
	case CS_DOWN:
	{
		D3DXVECTOR2 dir = { 0,1 };
		D3DXVECTOR2 pos;
		CsPacketMove *movePacket = reinterpret_cast<CsPacketMove*>(ptr);
		pos.x = players[id].getPositionX();
		pos.y = players[id].getPositionY();
		if (pos.y + 1 > MAX_MAP_SIZE)
			pos.y = MAX_MAP_SIZE;
		//for (auto i : players[id].pObjectList) {
		//	if (i < 1500) continue;
		//	if (true == crushObject(id, i))
		//		count++;
		//}
		//if (0 == count)
		pos = pos + (MAX_SPEED * dir);
		players[id].setPositionX(pos.x);
		players[id].setPositionY(pos.y);
		players[id].setState(walkPlayer);
		break;
	}
	case CS_ATTACK_A:
	{
		unordered_set<int> dead;
		unordered_set<int> hit;
		ScPacketStateMessage meg;
		meg.id = id;
		meg.size = sizeof(ScPacketStateMessage);
		players[id].setState(attackPlayer);

		players[id].pLock.lock();
		for (auto i : players[id].pObjectList) {
			if (deadState == objects[i]->getState())continue;
			if (true == attackCrushCheck(id, i)) {
				objects[i]->decreaseHP(players[id].getAttack());
				hit.insert(i);	
				//_asm mfence;
				if (0>= objects[i]->getHP()) {
					players[id].increaseExp(objects[i]->getExp());
					players[id].increaseGold(objects[i]->getGold());
					dead.insert(i);
					//_asm mfence;
					
					if (true == players[id].levelUpcheck()) {
						players[id].levelUp();
						meg.type = SC_LEVEL_UP;
						sendPacket(id, &meg);
					}
				}

			}
		}
		players[id].pLock.unlock();

		ScPacketStateMessage sendmeg;
		sendmeg.id = id;
		sendmeg.size = sizeof(ScPacketStateMessage);
		for (auto i : hit) {
			sendmeg.type = SC_HITDAMGE;
			sendmeg.damage = players[id].getAttack();
			sendmeg.monsterId = i;
			sendPacket(id, &sendmeg);
		}

		for (auto i : dead) {
			sendmeg.type = SC_MONSTER_DEAD;
			sendmeg.exp = objects[i]->getExp();
			sendmeg.gold = objects[i]->getGold();
			sendmeg.monsterId = i;
			//objects[i]->setLevel(0);
			sendPacket(id, &sendmeg);
		}


		sendPlayerStatus(id);
		break;
	}
	case CS_FIRE_SKILL:
	{
		unordered_set<int> dead;
		unordered_set<int> hit;
		ScPacketStateMessage meg;
		meg.id = id;
		meg.size = sizeof(ScPacketStateMessage);
		players[id].setState(fireSkill);

		players[id].pLock.lock();
		for (auto i : players[id].pObjectList) {
			if (deadState == objects[i]->getState())continue;
			if (true == fireAttackCheck(id, i)) {
				objects[i]->decreaseHP(players[id].getAttack());
				hit.insert(i);
				_asm mfence;
				if (0>= objects[i]->getHP()) {
					players[id].increaseExp(objects[i]->getExp());
					players[id].increaseGold(objects[i]->getGold());
					dead.insert(i);
					_asm mfence;
					//objects[i]->setLevel(0);
					if (true == players[id].levelUpcheck()) {
						players[id].levelUp();
						meg.type = SC_LEVEL_UP;
						sendPacket(id, &meg);
					}
				}

			}
		}
		players[id].pLock.unlock();

		ScPacketStateMessage sendmeg;
		sendmeg.id = id;
		sendmeg.size = sizeof(ScPacketStateMessage);
		for (auto i : hit) {
			sendmeg.type = SC_HITDAMGE;
			sendmeg.damage = players[id].getAttack();
			sendmeg.monsterId = i;
			sendPacket(id, &sendmeg);
		}

		for (auto i : dead) {
			sendmeg.type = SC_MONSTER_DEAD;
			sendmeg.exp = objects[i]->getExp();
			sendmeg.gold = objects[i]->getGold();
			sendmeg.monsterId = i;
			sendPacket(id, &sendmeg);
		}
		sendPlayerStatus(id);
		break;
	}
	case CS_STOP:
	{
		players[id].setState(waitPlayer);
		break;
	}
	case CS_CHAT:
	{
		CsPacketChat *chat = reinterpret_cast<CsPacketChat*>(ptr);
		ScPacketChat meg;
		meg.size = sizeof(ScPacketChat);
		meg.type = SC_CHAT;
		meg.id = id;
		wcscpy(meg.message, chat->message);
		//WideCharToMultiByte(CP_ACP, 0, chat->message, -1, (LPSTR)meg.message, 200, NULL, NULL);
		//memcpy(&meg.message, &chat->message, sizeof(chat->message));
		//sendPacket(id,&meg);
		players[id].pLock.lock();
		for (auto i : players[id].pViewList)
			sendPacket(i, &meg);
		players[id].pLock.unlock();
		break;
	}
	case CS_MENTAL_UP:
	{
		players[id].increaseMENTAL();
		break;
	}
	case CS_STR_UP:
	{
		players[id].increaseSTR();
		break;
	}
	case CS_DEX_UP:
	{
		players[id].increaseDEX();
		break;
	}
	}
	//매번 플레이어들의 위치값 갱신
	ScPacketMove packet;
	packet.pakcetSize = sizeof(ScPacketMove);
	packet.id = players[id].getID();
	packet.packetType = SC_MOVE_POSITION;
	packet.health = players[id].getHp();
	packet.state = players[id].getState();
	packet.position.x = players[id].getPositionX();
	packet.position.y = players[id].getPositionY();
	sendPacket(id, &packet);

	updateSector(id);
	viewListUpdate(id);
	sendPlayerStatus(id);
	players[id].pLock.lock();
	for (auto i : players[id].pViewList)
		sendPacket(i, &packet);
	players[id].pLock.unlock();
}
bool Server::attackCrushCheck(int player, int monster)
{
	D3DXVECTOR2 pPos;
	D3DXVECTOR2 monPos;
	pPos.x = players[player].getPositionX();
	pPos.y = players[player].getPositionY();
	monPos.x = objects[monster]->getPosX();
	monPos.y = objects[monster]->getPosY();
	float dist = (monPos.x - pPos.x)
		*(monPos.x - pPos.x)
		+ (monPos.y - pPos.y)
		* (monPos.y - pPos.y);
	if (dist <= 20 * 20)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Server::fireAttackCheck(int id, int Mon) {
	D3DXVECTOR2 pPos;
	D3DXVECTOR2 monPos;
	pPos.x = players[id].getPositionX();
	pPos.y = players[id].getPositionY();
	monPos.x = objects[Mon]->getPosX();
	monPos.y = objects[Mon]->getPosY();
	float dist = (monPos.x - pPos.x)
		*(monPos.x - pPos.x)
		+ (monPos.y - pPos.y)
		* (monPos.y - pPos.y);
	if (dist <= 50 * 50)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Server::crushObject(int player, int monster)
{
	D3DXVECTOR2 pPos;
	D3DXVECTOR2 monPos;
	pPos.x = players[player].getPositionX();
	pPos.y = players[player].getPositionY();
	monPos.x = objects[monster]->getPosX();
	monPos.y = objects[monster]->getPosY();
	float dist = (monPos.x - pPos.x)
		*(monPos.x - pPos.x)
		+ (monPos.y - pPos.y)
		* (monPos.y - pPos.y);
	if (dist <= 20 * 20)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Server::updateSector(int id)
{
	int currX = players[id].getPositionX() / DIVDIE_SECTOR;
	int currY = players[id].getPositionY() / DIVDIE_SECTOR;
	int predX = players[id].getPredSectorX();
	int predY = players[id].getPredSectorY();

	if ((currX != predX) || (currY != predY)) //이전섹터와 같지 않으면
	{
		//std::cout << "변경" << std::endl;
		if (100 == gameMap[currX][currY].debuff) {
			players[id].setMaxAttack();
			players[id].setMaxDepend();
		}
		else if (104 == gameMap[currX][currY].debuff) {
			int decountattack = players[id].getAttack() * 0.1;
			int attack = players[id].getAttack();
			players[id].setAttack(attack - decountattack);
			players[id].setMaxDepend();
		}
		else if (106 == gameMap[currX][currY].debuff) {
			int decount = players[id].getDepend() * 0.1;
			int depend = players[id].getDepend();
			players[id].setDepend(depend - decount);
			players[id].setMaxAttack();
		}
		//--------------------------------디버프 적용----------------------------
		players[id].setDebuff(gameMap[currX][currY].debuff);
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

		ScPacketBuff buff;
		buff.pakcetSize = sizeof(ScPacketBuff);
		buff.packetType = SC_BUFF;
		buff.id = id;
		buff.buff = players[id].getDebuff();
		sendPacket(id, &buff);
		sendPlayerStatus(id);
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
	//시야주변 섹터를 돌면서 현재 위치를 바탕으로 검사를 진행한다.
	for (auto i = startSectorX; i <= endSectorX; i++)
	{
		for (auto j = startSectorY; j <= endSectorY; ++j)
		{
			if ((0 == gameMap[i][j].player.size()) && (0 == gameMap[i][j].object.size())) continue; //섹터에 플레이어나 오브젝트가 없을경우는 건너뛴다.
			gameMap[i][j].sLock.lock();
			for (auto p : gameMap[i][j].player) //플레이어에 대한
			{
				if (id == p) continue; //자기자신에 대한건 건너 뛴다.
				if (false == players[p].getPlay()) continue;	//플레이어의 플레이 여부 판단
				nearList.insert(p);
			}
			gameMap[i][j].sLock.unlock();

			gameMap[i][j].sLock.lock();
			for (auto m : gameMap[i][j].object) //오브젝트에 대한
			{
				//if (false == objects[m]->getActive()) continue;
				nearList.insert(m);
			}
			gameMap[i][j].sLock.unlock();
		}
	}
	//nearlist에 들어있는 아이디값들이 현재 뷰리스트에 포함되어있는지 판단한다.
	//존재하면 그냥 넘어가고 존재하지 않으면 뷰리스트에 추가를 해 준다.
	for (auto i : nearList)
	{
		if (i >= OBJECT_START)	//오브젝트일 경우
		{
			players[id].pLock.lock();
			if (0==players[id].pObjectList.count(i))
			{
				players[id].pObjectList.insert(i);
				if (false == objects[i]->getActive()) {
					objects[i]->setActive(true); //플레이어의 시야에 들어왔기떄문에 오브젝트를 활동상태로 바꿔줘야한다.
					timer.AddGameEvent(MonsterUpdate, i, 1000);
				}
				players[id].pLock.unlock();	
			}
			else
			{
				players[id].pLock.unlock();
				continue;
			}
		}
		if (i < OBJECT_START)	//플레이어일 경우
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
	//다른플레이어 리스트에 자신이 있나 확인해야 한다.
	//이 작업은 플레이어들에 한해서 해야한다.
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
	//현재 nearlist에 없는데 뷰리스트에 존재하는 아이디들을 리무브 리스트로 옴겨준다.
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
	ScPacketRemovePlayer remove;
	remove.packetSize = sizeof(ScPacketRemovePlayer);
	remove.packetType = SC_REMOVE_PLAYER;
	for (auto i : removeList)
	{
		remove.id = i;
		sendPacket(id, &remove);
	}

	for (auto i : removeList)
	{
		if (i >= OBJECT_START)
		{
			continue;	//몬스터의 경우는 지나간다.
		}
		else
		{
			remove.id = id;
			sendPacket(i, &remove);
		}
	}

	//nearlist에 있는 대상들을 뿌려준다.
	ScPacketPutPlayer put;
	put.packetSize = sizeof(ScPacketPutPlayer);
	put.packetType = SC_SET_PLAYER;
	for (auto i : nearList)
	{
		if (i >= OBJECT_START)	//오브젝트의 경우
		{
			put.id = i;
			put.position.x = objects[i]->getPosX();
			put.position.y = objects[i]->getPosY();
			put.state = objects[i]->getState();
			sendPacket(id, &put);
		}
		else				//플레이어의 경우
		{
			put.id = i;
			put.position.x = players[i].getPositionX();
			put.position.y = players[i].getPositionY();
			put.health = players[i].getHp();
			put.state = players[i].getState();
			sendPacket(id, &put);
		}
	}

	for (auto i : nearList)
	{
		if (i >= OBJECT_START)
		{
			continue;	//몬스터의 경우는 지나간다.
		}
		else
		{
			put.id = id;
			put.position.x = players[id].getPositionX();
			put.position.y = players[id].getPositionY();
			put.health = players[id].getHp();
			put.state = players[id].getState();
			sendPacket(i, &put);
		}
	}
}
void Server::sendPlayerStatus(int id) {
	ScStatusInfo stat;
	stat.packetType = SC_PLAYER_STATUS;
	stat.pakcetSize = sizeof(ScStatusInfo);
	stat.id = id;
	stat.attack = players[id].getAttack();
	stat.acr = players[id].getAcr();
	stat.depend = players[id].getDepend();
	stat.dex = players[id].getDex();
	stat.gold = players[id].getGole();
	stat.level = players[id].getLevel();
	stat.maxHealth = players[id].getMaxHealth();
	stat.mental = players[id].getMental();
	stat.miss = players[id].getMiss();
	stat.str = players[id].getStr();
	stat.statusCount = players[id].getStatusCount();
	stat.exp = players[id].getExp();
	sendPacket(id, &stat);
	for (auto i : players[id].pViewList)
		sendPacket(i, &stat);
}
void Server::sendPacket(int client, void* packet)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	OverEx *Send_Operation = new OverEx;
	ZeroMemory(&Send_Operation->overLapped, sizeof(WSAOVERLAPPED));

	Send_Operation->operationType = Sendtype;

	Send_Operation->buf.buf = reinterpret_cast<CHAR*>(Send_Operation->packetBuf);
	Send_Operation->buf.len = packet_size;

	memcpy(Send_Operation->packetBuf, reinterpret_cast<unsigned char*>(packet), packet_size);

	DWORD iobyte;
	DWORD sendFlag = 0;
	int retval;
	retval = WSASend(players[client].overEx->s, &Send_Operation->buf, 1,
		NULL,0, &Send_Operation->overLapped, NULL);
	if (retval != 0)
	{
		cout << GetLastError() << endl;
		cout << "error sendpacket" << endl;
	}
}