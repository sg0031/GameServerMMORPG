#pragma once
enum MonsterState
{
	waitState = 200,
	attackState,
	chaseState,
	deadState,
	returnState
};
enum PlayerState
{
	waitPlayer = 300,
	attackPlayer,
	fireSkill,
	walkPlayer,
	deadPlayer
};
struct Object
{
	int id;
	int x;
	int y;
	bool isActive;
	int buff;   //몬스터를 잡았을시에 플레이어가 획득할수 있는 버프
	int type;    //몬스터의 선공형인지 후공형인지 또는 오브젝트인지 판단
	int health; //몬스터 체력
	int attack;	//몬스터 공격력
	int depend; //몬스터 방어력
	int acr;    //몬스터 명중률
	int exp;    //몬스터가 주는 경험치
	int level;  //몬스터 레벨
	int gold;   //몬스터가주는 골드
	int state;  //몬스터의 현재 상태
};
class Player
{
	int x;
	int y;
	int id;
	bool isConnect;
	int state;
	int mapDebuff;
	int getBuff;
	int health;

	int attack; // 플레이어 공격력
	int depend; //플레이어 방어력
	int miss;//플레이어 회피력
	int level;//플레이어 레벨
	int exp;//플레이어 경험치
	int str;//플레이어 힘
	int dex;//플레이어 민첩
	int mental;//플레이어 지력
	int gold;//플레이어의 보유중인 골드
	int acr; //플레이여 명중률
	int maxHealth;
	int statusCount;
public:
	TCHAR viewChat[100];
	TCHAR chat[100];
	RECT rc;
	TCHAR strinHP[100];
	TCHAR strinMaxHP[100];
	TCHAR strinAttack[10];
	TCHAR strinDepend[10];
	TCHAR strinMiss[10];
	TCHAR strinExp[10];
	TCHAR strinSTR[10];
	TCHAR strinDEX[10];
	TCHAR strinMental[10];
	TCHAR strinGold[10];
	TCHAR strinAcr[10];
	TCHAR strinLevel[100];

	void setStatuCount(int st) { statusCount = st; }
	int getStatusCount() { return statusCount; }
	void setAttack(int Attack) { attack = Attack; }
	int getAttack() { return attack; }
	void setDepend(int de) { depend = de; }
	int getDepend() { return depend; }
	void setMiss(int mis) { miss = mis; }
	int getMiss() { return miss; }
	void setLevel(int le) { level = le; }
	int getLevel() { return level; }
	void setExp(int ex) { exp = ex; }
	int getExp() { return exp; }
	void setStr(int st) { str = st; }
	int getStr() { return str; }
	void setDex(int de) { dex = de; }
	int getDex() { return dex; }
	void setMental(int men) { mental = men; }
	int getMental() { return mental; }
	void setGold(int go) { gold = go; }
	int getGold() { return gold; }
	void setAcr(int ac) { acr = ac; }
	int getAcr() { return acr; }
	void setMaxHP(int hp) { maxHealth = hp; }
	int getMaxHP() { return maxHealth; }

	void setHP(int hp) { health = hp; }
	int getHp() { return health; }
	void setDebuff(int de) { mapDebuff = de; }
	void setBuff(int bu) { getBuff = bu; }
	int getDebuff() { return mapDebuff; }
	int getPlayerBuff() { return getBuff; }

	void setState(int st) { state = st; }
	int getState() { return state; }
	void setConnect(bool sCon) { isConnect = sCon; }
	bool getConnect() { return isConnect; }
	void setID(int i) { id=i; }
	int getID() { return id; }
	void setPositionX(int px);
	void setPositionY(int py);
	int getPositionX();
	int getPositionY();
	void moveUp() { x--; }
	void moveDown() { x++; }
	void moveRight() { y++; }
	void moveLeft() { y--; }
	Player();
	~Player();
};

