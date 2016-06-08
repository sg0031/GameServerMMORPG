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

