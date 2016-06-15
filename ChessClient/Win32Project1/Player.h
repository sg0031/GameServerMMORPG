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
	leftWaitPlayer=300,
	rightWaitPlayer,
	upWaitPlayer,
	downWaitPlayer,
	waitPlayer,
	attackPlayer,
	leftAttackPlayer,
	rightAttackPlayer,
	upAttackPlayer,
	downAttackPlayer,
	leftWalkPlayer,
	rightWalkPlayer,
	upWalkPlayer,
	downWalkPlayer,
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
public:
	TCHAR strinHP[100];
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

