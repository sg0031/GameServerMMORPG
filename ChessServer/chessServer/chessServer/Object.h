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
	deadState
};

class Object
{
	bool active;
	int x;
	int y;
	int target;
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
	void setTarget(int ta) { target = ta; }
	int getTarget() { return target; }

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
	};
	~Rabbit() {};
	virtual void upDate(); 
	virtual void move(); 
	virtual bool attakcRange(); 
	virtual bool hitDamge();
	virtual void randomMove();
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