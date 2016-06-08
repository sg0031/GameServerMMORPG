#pragma once
enum buffType
{
	noBuff = 100,
	attackUp,
	speedUp,
	dependUp
};
struct Object
{
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
public:
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

