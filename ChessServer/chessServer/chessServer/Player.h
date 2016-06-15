#pragma once
#define PLAYER_START_POSX 4
#define PLAYER_START_POSY 4
#define PLAYER_INCREASE_HP 10 //퍼센트 단위
#define MAX_HEALTH 1000
enum PlayerState
{
	waitPlayer = 300,
	attackPlayer,
	walkPlayer,
	deadPlayer
};
class Player
{
	int x; //플레이어 좌표
	int y;
	int id;	//플레이어를 구분짓는 인트형 아이디값
	bool play; //현재 플레이 중인지 판단
	int attack; // 플레이어 공격력
	int depend; //플레이어 방어력
	int miss;//플레이어 회피력
	int level;//플레이어 레벨
	int exp;//플레이어 경험치
	int str;//플레이어 힘
	int dex;//플레이어 민첩
	int mental;//플레이어 지력
	int gold;//플레이어의 보유중인 골드
	int health; //플레이어 체력
	int acr; //플레이여 명중률
	int statusCount; //레벨업시에 남아있는
	int predSectorX, predSectorY;
	int hitdamage;
	int animateState;
	int playerBuff;
	int playerDeBuff;
public:
	Player();
	~Player();
	std::mutex pLock;
	std::unordered_set<int> pViewList; //플레이어의 정보를 뿌려줄 플레이어리스트
	std::unordered_set<int> pObjectList; //플레이어 주위의 오브젝트 리스트
	int viewList[MAX_PLAYER];
	OverEx* overEx;
	void setDebuff(int de) { playerDeBuff = de; }
	void setBuff(int bu) { playerBuff = bu; }
	int getDebuff() { return playerDeBuff; }
	int getPlayerBuff() { return playerBuff; }

	void setState(int st) { animateState = st; }
	int getState() { return animateState; }
	void setAttack(int ack) { attack = ack; }
	int getAttack() { return attack; }
	void setDepend(int de) { depend = de; }
	int getDepend() { return depend; }

	void setHitDamage(int hit) { hitdamage = hit; }
	int getHitDamage() { return hitdamage; }
	void setPredSectorX(int sx) { predSectorX = sx; }
	void setPredSectorY(int sy) { predSectorY = sy; }
	int getPredSectorX() { return predSectorX; }
	int getPredSectorY() { return predSectorY; }

	void decreaseHP(int ack) { health -= ack; std::cout << id << "현재 HP :" << health << std::endl; }
	int getHp() { return health; }
	//골드획득시 호출되는 메소드
	void increaseGold(int gol) { gold += gol; }
	//레벨업시에 호출되는 메소드
	void levelUp() {
		exp = 0; 
		statusCount++; 
		health = MAX_HEALTH;
	}
	//스택선택시 상승 가능
	void increaseMENTAL() { 
		mental++;
		attack = attack + (mental*0.5);
		depend = depend + (mental*0.5);
		statusCount--;
	}
	void increaseDEX() { 
		dex++;
		miss = miss + (dex * 1);
		acr = acr + (dex * 1);
		statusCount--;
	}
	void increaseSTR() { 
		str++; 
		attack = attack + (str * 2);
		statusCount--;
	}
	//죽을을시에 호출 될 메소드
	void resetPlayer() {
		x = PLAYER_START_POSX;
		y = PLAYER_START_POSY;
		health = MAX_HEALTH;
		exp = exp / 2;
	}
	//5초마다 회복될 플레이어 hp
	void increaseHP() {
		health = health + (health / PLAYER_INCREASE_HP); 
	}
	//플레이어 경험치 획득시 사용될 메소드
	void increaseExp(int ex) { exp += ex; }

	//플레이어 아이디셋팅에 관한 메소드
	void setPlay(bool p) { play = p; }
	bool getPlay() { return play; }
	void setID(int i) { id=i; }
	int getID() { return id; }
	//플레이어 좌표에 관한 메소드
	void setPositionX(int px);
	void setPositionY(int py);
	int getPositionX();
	int getPositionY();

	//플레이어 이동관련 메소드
	void moveUp() { x--; }
	void moveDown() { x++; }
	void moveRight() { y++; }
	void moveLeft() { y--; }

};

