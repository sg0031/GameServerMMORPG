#pragma once
#define ATTACK_TYPE 1
#define PIECE_TYPE 2
#define STATIC_TYPE 3

enum buffType
{
	noBuff=100,
	attackUp,
	speedUp,
	dependUp
};
enum MonsterState
{
	waitState=200,
	attackState,
	chaseState,
	deadState,
	returnState
};

class Object
{
	D3DXVECTOR2 dir;
	bool active;
	float speed;
	int x;
	int y;
	int id;
	int target;
	D3DXVECTOR2 targetPos;
	D3DXVECTOR2 returnPos;
protected:
	buffType buff;   //몬스터를 잡았을시에 플레이어가 획득할수 있는 버프
	int type;    //몬스터의 선공형인지 후공형인지 판단
	int health; //몬스터 체력
	int attack;	//몬스터 공격력
	int depend; //몬스터 방어력
	int acr;    //몬스터 명중률
	int exp;    //몬스터가 주는 경험치
	int level;  //몬스터 레벨
	int gold;   //몬스터가주는 골드
	MonsterState state;  //몬스터의 현재 상태
public:
	Object() {};
	~Object() {};
	void setAttack(int ack) { attack = ack; }
	int getAttack() { return attack; }

	void setID(int id) { this->id = id; }
	int getID() { return id; }
	void setTarget(int ta) { target = ta; }
	int getTarget() { return target; }
	void setDir(D3DXVECTOR2 dr) { dir = dr; }
	D3DXVECTOR2 getDir() { return dir; }
	void setSpeed(float spd) { speed = spd; }
	float getSpeed() { return speed; }
	void setTagetPos(int x, int y) { targetPos.x = x; targetPos.y = y; }
	D3DXVECTOR2 getTagetPos() {return targetPos;}

	void setReturnPos(float x, float y) { returnPos.x = x; returnPos.y = y; }
	D3DXVECTOR2 getReturnPos() { return returnPos; }
	void setPosX(int x) { this->x = x; }
	void setPosY(int y) { this->y = y; }
	int getPosX() { return x; }
	int getPosY() { return y; }
	void setActive(int a) { active = a; }
	bool getActive() { return active; }
	int getExp() { return 5 * level; }

	int getState() { return state; }

	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
	virtual bool attakcRange() { return false; } //몬스터의 공격범위를 판단해주는 메소드
	virtual bool chaseRange() { return false; }//추격할때 호출되는 메소드
	virtual bool hitDamge() { return false; }	//평화몬스터의 경우 이메소드를 이용해서 데미지를 입었는지 판단한다.
	virtual void randomMove() {}; //랜덤하게 이동하는 메소드
	virtual bool playerHit() { return false; }
	virtual void decreaseHP(int attack) {};
};
class Stone :public Object
{
public:
	Stone() { type = STATIC_TYPE; };
	~Stone() {};
};
class Tree : public Object
{
public:
	Tree() { type = STATIC_TYPE; };
	~Tree() {};
};
class Rabbit : public Object
{
public:
	Rabbit() {
		buff=noBuff;
		type=PIECE_TYPE;
		health=100;
		attack=10;
		depend = 10;
		acr=10;
		exp=10;
		level=10;
		gold=10;
		state=waitState;
		setSpeed(2.0);
	};
	~Rabbit() {};
	virtual void upDate(); 
	virtual void move(); 
	virtual bool attakcRange(); 
	virtual bool hitDamge();
	virtual void randomMove();
	virtual bool chaseRange();
	virtual bool playerHit();
	virtual void decreaseHP(int attack);
};
//
//class Baby : public Object
//{
//public:
//	Baby() {
//		buff = noBuff;
//		type = PIECE_TYPE;
//		health = 100;
//		attack = 10;
//		depend = 10;
//		acr = 10;
//		exp = 40;
//		level = 50;
//		gold = 10;
//		state = wait;
//	};
//	~Baby() {};
//	virtual void upDate(); //몬스터의 상태를 매번 업데이트해주는 메소드
//	virtual void move(); //몬스터가 이동시에 발생되는 메소드
//	virtual bool attakcRange(); //몬스터의 공격범위를 판단해주는 메소드
//};
//class BabyGuard : public Object
//{
//public:
//	BabyGuard() {
//		buff = noBuff;
//		type = PIECE_TYPE;
//		health = 100;
//		attack = 10;
//		depend = 10;
//		acr = 10;
//		exp = 10;
//		level = 30;
//		gold = 10;
//		state = wait;
//	};
//	~BabyGuard() {};
//	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
//	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
//	virtual bool attakcRange(); //몬스터의 공격범위를 판단해주는 메소드
//};
//class Agro :public Object
//{
//public:
//	Agro() {
//		buff = noBuff;
//		type = PIECE_TYPE;
//		health = 100;
//		attack = 10;
//		depend = 10;
//		acr = 10;
//		exp = 10;
//		level = 20;
//		gold = 10;
//		state = wait;
//	};
//	~Agro() {};
//	virtual void upDate() {} //몬스터의 상태를 매번 업데이트해주는 메소드
//	virtual void move() {} //몬스터가 이동시에 발생되는 메소드
//	virtual bool attakcRange(); //몬스터의 공격범위를 판단해주는 메소드
//};