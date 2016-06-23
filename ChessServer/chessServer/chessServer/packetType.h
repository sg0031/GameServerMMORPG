#pragma once




enum EOperationType
{
	Sendtype = 1,
	Recvtype = 2,
	MonsterUpdate,
	MonsterRespon,
	playersHPincrease
};
struct OverEx	//오버렙트구조체 확장
{
	OVERLAPPED overLapped;
	SOCKET s;
	EOperationType operationType;	//패킷의 타입
	int prevSize;	//이전데이타 크기
	int currentSize;//현재데이타 크기
	WSABUF buf;
	unsigned char packetBuf[256];
	unsigned char iocpBuf[4096];
};
enum
{

	CS_RIGHT = 1, //플레이어 오른쪽 이동
	CS_LEFT,	  //플레이어 왼쪽 이동
	CS_UP,		  //플레이어 위쪽 이동
	CS_DOWN,	  //플레이어 아래쪽 이동
	CS_LOGIN,     //플레이어 로그인 요청
	CS_ATTACK_A,  //플레이어 기본공격
	CS_FIRE_SKILL,//플레이어 스킬사용
	CS_STATE_UPDATE,//플레이어 스테이터스 업데이트
	CS_STOP,      //플레이어가 keyup시에 보내줌
	CS_CHAT,      //플레이어 채팅
	CS_STR_UP,    //플레이어가 statuspoint를 써서 str를 올릴때
	CS_DEX_UP,    //플레이어가 statuspoint를 써서 dex를 올릴때
	CS_MENTAL_UP, //플레이어가 statuspoint를 써서 mental를 올릴때

	SC_MOVE_POSITION = 100, //플레이어 이동
	SC_LOGIN_COMPLTE,		//플레이어 로그인 성공
	SC_SET_PLAYER,			//플레이어 배치
	SC_REMOVE_PLAYER,		//플레이어 제거
	SC_BUFF,				//플레이어 버프
	SC_STATE_UPDATE,		//플레이어 상태 업데이트
	SC_PLAYER_STATUS,		//다른플레이어 상태표시
	SC_CHAT,				//다른플레이어 채팅표시
	SC_LEVEL_UP,			//플레이어 레벨업
	SC_HITDAMGE,			//플레이어가 몬스터한테 때렸을때
	SC_MONSTER_ATTACK_PLAYER,//몬스터가 플레이어를 때렸을때
	SC_MONSTER_DEAD			//몬스터가 죽었을때

};