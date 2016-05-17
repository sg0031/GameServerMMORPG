#pragma once
#define FIRST_ATTACK 1
#define PIECE 2

enum buffType
{
	noBuff=100,
	attackUp,
	speedUp,
	dependUp
};

class Object
{
protected:
	int x;
	int y;
public:
	Object() {};
	~Object() {};
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
class Agro :public Monster
{
public:
	Agro() {};
	~Agro() {};
	virtual void upDate() {} //������ ���¸� �Ź� ������Ʈ���ִ� �޼ҵ�
	virtual void move() {} //���Ͱ� �̵��ÿ� �߻��Ǵ� �޼ҵ�
	virtual bool attakcRange() { return false; } //������ ���ݹ����� �Ǵ����ִ� �޼ҵ�
};