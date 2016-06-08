#pragma once
#define ATTACK_TYPE 1
#define PIECE_TYPE 2

enum buffType
{
	noBuff=100,
	attackUp,
	speedUp,
	dependUp
};


class Object
{
	bool active;
	int x;
	int y;
public:
	Object() { active = true; };
	~Object() {};
	void setPosX(int x) { this->x = x; }
	void setPosY(int y) { this->y = y; }
	int getPosX() { return x; }
	int getPosY() { return y; }
	void setActive(int a) { active = a; }
	bool getActive() { return active; }
};

//몬스터 클래스틑 오브젝트 클래스를 상속받는다.
//게임내에 존재하는 모든 오브젝트들은 좌표값을 갖고있는다.

class Monster : public Object
{
protected:
	int buff;   //몬스터를 잡았을시에 플레이어가 획득할수 있는 버프
	int type;    //몬스터의 선공형인지 후공형인지 판단
	int health; //몬스터 체력
	int attack;	//몬스터 공격력
	int depend; //몬스터 방어력
	int acr;    //몬스터 명중률
	int exp;    //몬스터가 주는 경험치
	int level;  //몬스터 레벨
	int gold;   //몬스터가주는 골드
	int state;  //몬스터의 현재 상태
public:
	Monster() {};
	~Monster() {};
	int getExp() { return 5 * level; }
	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
	virtual bool attakcRange() { return false; } //몬스터의 공격범위를 판단해주는 메소드
};

class Rabit : public Monster
{
public:
	Rabit() {};
	~Rabit() {};
	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
	virtual bool attakcRange() { return false; } //몬스터의 공격범위를 판단해주는 메소드
};

class Baby : public Monster
{
public:
	Baby() {};
	~Baby() {};
	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
	virtual bool attakcRange() { return false; } //몬스터의 공격범위를 판단해주는 메소드
};
class BabyGuard : public Monster
{
public:
	BabyGuard() {};
	~BabyGuard() {};
	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
	virtual bool attakcRange() { return false; } //몬스터의 공격범위를 판단해주는 메소드
};
class Agro :public Monster
{
public:
	Agro() {};
	~Agro() {};
	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
	virtual bool attakcRange() { return false; } //몬스터의 공격범위를 판단해주는 메소드
};