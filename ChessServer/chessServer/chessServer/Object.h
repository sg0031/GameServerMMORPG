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
	wait=200,
	attack,
	chase,
	dead,
};

class Object
{
	bool active;
	int x;
	int y;
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
	void setPosX(int x) { this->x = x; }
	void setPosY(int y) { this->y = y; }
	int getPosX() { return x; }
	int getPosY() { return y; }
	void setActive(int a) { active = a; }
	bool getActive() { return active; }


	int getExp() { return 5 * level; }

	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};

//���� Ŭ�����z ������Ʈ Ŭ������ ��ӹ޴´�.
//���ӳ��� �����ϴ� ��� ������Ʈ���� ��ǥ���� �����ִ´�.

//class Monster : public Object
//{
//protected:
//	int buff;   //���͸� ������ÿ� �÷��̾ ȹ���Ҽ� �ִ� ����
//	int type;    //������ ���������� �İ������� �Ǵ�
//	int health; //���� ü��
//	int attack;	//���� ���ݷ�
//	int depend; //���� ����
//	int acr;    //���� ���߷�
//	int exp;    //���Ͱ� �ִ� ����ġ
//	int level;  //���� ����
//	int gold;   //���Ͱ��ִ� ���
//	int state;  //������ ���� ����
//public:
//	Monster() {};
//	~Monster() {};
//	
//	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
//	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
//	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
//};
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
class Rabit : public Object
{
public:
	Rabit() {
		buff=noBuff;
		type=PIECE_TYPE;
		health=100;
		attack=10;
		depend = 10;
		acr=10;
		exp=10;
		level=10;
		gold=10;
		state=wait;
	};
	~Rabit() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};

class Baby : public Object
{
public:
	Baby() {
		buff = noBuff;
		type = PIECE_TYPE;
		health = 100;
		attack = 10;
		depend = 10;
		acr = 10;
		exp = 10;
		level = 10;
		gold = 10;
		state = wait;
	};
	~Baby() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};
class BabyGuard : public Object
{
public:
	BabyGuard() {
		buff = noBuff;
		type = PIECE_TYPE;
		health = 100;
		attack = 10;
		depend = 10;
		acr = 10;
		exp = 10;
		level = 10;
		gold = 10;
		state = wait;
	};
	~BabyGuard() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};
class Agro :public Object
{
public:
	Agro() {
		buff = noBuff;
		type = PIECE_TYPE;
		health = 100;
		attack = 10;
		depend = 10;
		acr = 10;
		exp = 10;
		level = 10;
		gold = 10;
		state = wait;
	};
	~Agro() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};