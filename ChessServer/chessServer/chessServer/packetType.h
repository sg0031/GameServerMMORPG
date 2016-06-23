#pragma once




enum EOperationType
{
	Sendtype = 1,
	Recvtype = 2,
	MonsterUpdate,
	MonsterRespon,
	playersHPincrease
};
struct OverEx	//������Ʈ����ü Ȯ��
{
	OVERLAPPED overLapped;
	SOCKET s;
	EOperationType operationType;	//��Ŷ�� Ÿ��
	int prevSize;	//��������Ÿ ũ��
	int currentSize;//���絥��Ÿ ũ��
	WSABUF buf;
	unsigned char packetBuf[256];
	unsigned char iocpBuf[4096];
};
enum
{

	CS_RIGHT = 1, //�÷��̾� ������ �̵�
	CS_LEFT,	  //�÷��̾� ���� �̵�
	CS_UP,		  //�÷��̾� ���� �̵�
	CS_DOWN,	  //�÷��̾� �Ʒ��� �̵�
	CS_LOGIN,     //�÷��̾� �α��� ��û
	CS_ATTACK_A,  //�÷��̾� �⺻����
	CS_FIRE_SKILL,//�÷��̾� ��ų���
	CS_STATE_UPDATE,//�÷��̾� �������ͽ� ������Ʈ
	CS_STOP,      //�÷��̾ keyup�ÿ� ������
	CS_CHAT,      //�÷��̾� ä��
	CS_STR_UP,    //�÷��̾ statuspoint�� �Ἥ str�� �ø���
	CS_DEX_UP,    //�÷��̾ statuspoint�� �Ἥ dex�� �ø���
	CS_MENTAL_UP, //�÷��̾ statuspoint�� �Ἥ mental�� �ø���

	SC_MOVE_POSITION = 100, //�÷��̾� �̵�
	SC_LOGIN_COMPLTE,		//�÷��̾� �α��� ����
	SC_SET_PLAYER,			//�÷��̾� ��ġ
	SC_REMOVE_PLAYER,		//�÷��̾� ����
	SC_BUFF,				//�÷��̾� ����
	SC_STATE_UPDATE,		//�÷��̾� ���� ������Ʈ
	SC_PLAYER_STATUS,		//�ٸ��÷��̾� ����ǥ��
	SC_CHAT,				//�ٸ��÷��̾� ä��ǥ��
	SC_LEVEL_UP,			//�÷��̾� ������
	SC_HITDAMGE,			//�÷��̾ �������� ��������
	SC_MONSTER_ATTACK_PLAYER,//���Ͱ� �÷��̾ ��������
	SC_MONSTER_DEAD			//���Ͱ� �׾�����

};