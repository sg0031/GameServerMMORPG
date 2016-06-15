#pragma once
#define PLAYER_START_POSX 4
#define PLAYER_START_POSY 4
#define PLAYER_INCREASE_HP 10 //�ۼ�Ʈ ����
#define MAX_HEALTH 1000
enum PlayerState
{
	waitPlayer = 300,
	attackPlayer,
	walkPlayer,
	deadPlayer
};
class Player
{
	int x; //�÷��̾� ��ǥ
	int y;
	int id;	//�÷��̾ �������� ��Ʈ�� ���̵�
	bool play; //���� �÷��� ������ �Ǵ�
	int attack; // �÷��̾� ���ݷ�
	int depend; //�÷��̾� ����
	int miss;//�÷��̾� ȸ�Ƿ�
	int level;//�÷��̾� ����
	int exp;//�÷��̾� ����ġ
	int str;//�÷��̾� ��
	int dex;//�÷��̾� ��ø
	int mental;//�÷��̾� ����
	int gold;//�÷��̾��� �������� ���
	int health; //�÷��̾� ü��
	int acr; //�÷��̿� ���߷�
	int statusCount; //�������ÿ� �����ִ�
	int predSectorX, predSectorY;
	int hitdamage;
	int animateState;
	int playerBuff;
	int playerDeBuff;
public:
	Player();
	~Player();
	std::mutex pLock;
	std::unordered_set<int> pViewList; //�÷��̾��� ������ �ѷ��� �÷��̾��Ʈ
	std::unordered_set<int> pObjectList; //�÷��̾� ������ ������Ʈ ����Ʈ
	int viewList[MAX_PLAYER];
	OverEx* overEx;
	void setDebuff(int de) { playerDeBuff = de; }
	void setBuff(int bu) { playerBuff = bu; }
	int getDebuff() { return playerDeBuff; }
	int getPlayerBuff() { return playerBuff; }

	void setState(int st) { animateState = st; }
	int getState() { return animateState; }
	void setAttack(int ack) { attack = ack; }
	int getAttack() { return attack; }
	void setDepend(int de) { depend = de; }
	int getDepend() { return depend; }

	void setHitDamage(int hit) { hitdamage = hit; }
	int getHitDamage() { return hitdamage; }
	void setPredSectorX(int sx) { predSectorX = sx; }
	void setPredSectorY(int sy) { predSectorY = sy; }
	int getPredSectorX() { return predSectorX; }
	int getPredSectorY() { return predSectorY; }

	void decreaseHP(int ack) { health -= ack; std::cout << id << "���� HP :" << health << std::endl; }
	int getHp() { return health; }
	//���ȹ��� ȣ��Ǵ� �޼ҵ�
	void increaseGold(int gol) { gold += gol; }
	//�������ÿ� ȣ��Ǵ� �޼ҵ�
	void levelUp() {
		exp = 0; 
		statusCount++; 
		health = MAX_HEALTH;
	}
	//���ü��ý� ��� ����
	void increaseMENTAL() { 
		mental++;
		attack = attack + (mental*0.5);
		depend = depend + (mental*0.5);
		statusCount--;
	}
	void increaseDEX() { 
		dex++;
		miss = miss + (dex * 1);
		acr = acr + (dex * 1);
		statusCount--;
	}
	void increaseSTR() { 
		str++; 
		attack = attack + (str * 2);
		statusCount--;
	}
	//�������ÿ� ȣ�� �� �޼ҵ�
	void resetPlayer() {
		x = PLAYER_START_POSX;
		y = PLAYER_START_POSY;
		health = MAX_HEALTH;
		exp = exp / 2;
	}
	//5�ʸ��� ȸ���� �÷��̾� hp
	void increaseHP() {
		health = health + (health / PLAYER_INCREASE_HP); 
	}
	//�÷��̾� ����ġ ȹ��� ���� �޼ҵ�
	void increaseExp(int ex) { exp += ex; }

	//�÷��̾� ���̵���ÿ� ���� �޼ҵ�
	void setPlay(bool p) { play = p; }
	bool getPlay() { return play; }
	void setID(int i) { id=i; }
	int getID() { return id; }
	//�÷��̾� ��ǥ�� ���� �޼ҵ�
	void setPositionX(int px);
	void setPositionY(int py);
	int getPositionX();
	int getPositionY();

	//�÷��̾� �̵����� �޼ҵ�
	void moveUp() { x--; }
	void moveDown() { x++; }
	void moveRight() { y++; }
	void moveLeft() { y--; }

};

