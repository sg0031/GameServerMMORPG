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
	int buff;   //���͸� ������ÿ� �÷��̾ ȹ���Ҽ� �ִ� ����
	int type;    //������ ���������� �İ������� �Ǵ� ������Ʈ���� �Ǵ�
	int health; //���� ü��
	int attack;	//���� ���ݷ�
	int depend; //���� ����
	int acr;    //���� ���߷�
	int exp;    //���Ͱ� �ִ� ����ġ
	int level;  //���� ����
	int gold;   //���Ͱ��ִ� ���
	int state;  //������ ���� ����
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

	int attack; // �÷��̾� ���ݷ�
	int depend; //�÷��̾� ����
	int miss;//�÷��̾� ȸ�Ƿ�
	int level;//�÷��̾� ����
	int exp;//�÷��̾� ����ġ
	int str;//�÷��̾� ��
	int dex;//�÷��̾� ��ø
	int mental;//�÷��̾� ����
	int gold;//�÷��̾��� �������� ���
	int acr; //�÷��̿� ���߷�
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

