#pragma once
class Server
{
	HWND socketHWND;
	HINSTANCE inst;

	SOCKET sock;
	WSABUF WSA_send_buf;
	char Send_buf[MAXBUFSIZE];
	WSABUF WSA_recv_buf;
	char Recv_buf[MAXBUFSIZE];
	WSABUF WSA_Complete_buf;
	char Complete_buf[MAXBUFSIZE];
	int in_packet_size;
	int save_packet_size;
public:
	int chatLine;
	WCHAR playerChat[100];
	WCHAR chatWindow[6][100];//√§∆√√¢
	WCHAR stateMessage[6][100];
	Object objects[MAX_OBJECT];
	Player players[MAX_PLAYER];
	static Server* getInstangce()
	{
		static Server inst;
		return &inst;
	}
	Server();
	int socketinit(WCHAR *ip);
	~Server();
	GameBoard board[WIDTH][HEIGHT];
	void setSocketHWND(HWND s) { socketHWND = s; }
	void setHINSTANCE(HINSTANCE g) { inst = g; }
	void ReadPacket();
	void KeyDown(WPARAM wParam);
	void KeyDownAttack(WPARAM key);
	void ProcessPacket(char* buf);
	void SendPacket(SOCKET s, void* buf);
	void requsetState();
	void SendMes(WCHAR *buf);
	void KeyUp();
	void statusUp();
};

