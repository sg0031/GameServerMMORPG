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

//���� Ŭ�����z ������Ʈ Ŭ������ ��ӹ޴´�.
//���ӳ��� �����ϴ� ��� ������Ʈ���� ��ǥ���� �����ִ´�.

class Monster : public Object
{
protected:
	int buff;   //���͸� ������ÿ� �÷��̾ ȹ���Ҽ� �ִ� ����
	int type;    //������ ���������� �İ������� �Ǵ�
	int health; //���� ü��
	int attack;	//���� ���ݷ�
	int depend; //���� ����
	int acr;    //���� ���߷�
	int exp;    //���Ͱ� �ִ� ����ġ
	int level;  //���� ����
	int gold;   //���Ͱ��ִ� ���
	int state;  //������ ���� ����
public:
	Monster() {};
	~Monster() {};
	int getExp() { return 5 * level; }
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};

class Rabit : public Monster
{
public:
	Rabit() {};
	~Rabit() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};

class Baby : public Monster
{
public:
	Baby() {};
	~Baby() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};
class BabyGuard : public Monster
{
public:
	BabyGuard() {};
	~BabyGuard() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};
class Agro :public Monster
{
public:
	Agro() {};
	~Agro() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};