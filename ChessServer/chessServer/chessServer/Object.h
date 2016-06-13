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
	buffType buff;   //���͸� ������ÿ� �÷��̾ ȹ���Ҽ� �ִ� ����
	int type;    //������ ���������� �İ������� �Ǵ�
	int health; //���� ü��
	int attack;	//���� ���ݷ�
	int depend; //���� ����
	int acr;    //���� ���߷�
	int exp;    //���Ͱ� �ִ� ����ġ
	int level;  //���� ����
	int gold;   //���Ͱ��ִ� ���
	MonsterState state;  //������ ���� ����
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

	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
	virtual bool chaseRange() { return false; }//�߰��Ҷ� ȣ��Ǵ� �޼ҵ�
	virtual bool hitDamge() { return false; }	//��ȭ������ ��� �̸޼ҵ带 �̿��ؼ� �������� �Ծ����� �Ǵ��Ѵ�.
	virtual void randomMove() {}; //�����ϰ� �̵��ϴ� �޼ҵ�
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
//	virtual void upDate(); //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
//	virtual void move(); //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
//	virtual bool attakcRange(); //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
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
//	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
//	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
//	virtual bool attakcRange(); //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
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
//	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
//	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
//	virtual bool attakcRange(); //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
//};