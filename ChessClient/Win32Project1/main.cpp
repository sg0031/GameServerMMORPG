#include "stdafx.h"
using namespace std;
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#pragma comment (lib, "ws2_32.lib")



LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
HWND hWnd;
RECT crt;
HBITMAP  bBitmap;
int globalX;
int globalY;
Server *s = Server::getInstangce();

void drawObject(HDC hdc, HBITMAP image,int posx,int posy)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, image);
	TransparentBlt(hdc, posx, posy,
		WIDTHRECTANGLE, HEIGHTRECTANGLE, MemDC, 0, 0, 35, 62, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
void drawStone(HDC hdc, HBITMAP image, int posx, int posy)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, image);
	TransparentBlt(hdc, posx, posy,
		WIDTHRECTANGLE, HEIGHTRECTANGLE, MemDC, 0, 0, 40, 40, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
void drawBaby(HDC hdc, HBITMAP image, int posx, int posy,int sprightx,int sprighty)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, image);
	TransparentBlt(hdc, posx, posy,
		WIDTHRECTANGLE, HEIGHTRECTANGLE, MemDC, sprightx, sprighty, 40, sprighty+40, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
void drawRabbit(HDC hdc, HBITMAP image, int posx, int posy, int sprightx, int sprighty)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, image);
	TransparentBlt(hdc, posx, posy,
		WIDTHRECTANGLE, HEIGHTRECTANGLE, MemDC, sprightx, 0, sprightx+40, 40, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
void drawRabbitWait(HDC hdc, HBITMAP image, int posx, int posy)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, image);
	TransparentBlt(hdc, posx, posy,
		WIDTHRECTANGLE, HEIGHTRECTANGLE, MemDC, 0, 0, 40, 40, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	 hwnd;
	MSG 	 msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name",
		"Window Title Name",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		WINDOWSWIDTH,
		WINDOWSHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	s->setSocketHWND(hwnd);
	s->setHINSTANCE(hInst);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HWND hDlg = NULL;
	hWnd = hDlg;
	HDC hdc;
	static HDC bMemdc1,bMemdc2,bMemdc3;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HBRUSH hBrushBlack, oldBrushBlack;
	static HBITMAP chessPiece,skyMap,oldBit1,oldBit2,oldBit3;
	static HBITMAP stone,baby,rabbit,argo,babyguard;
	static TCHAR ip[10];
	static TCHAR pos[100];
	static TCHAR debuff[100];
	static TCHAR buff[100];
	static int count;
	int camaraPlayerX;
	int camaraPlayerY;
	int screenX;
	int screenY;
	static int frameCount;
	
	switch (iMsg)
	{
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam)) {
			closesocket((SOCKET)wParam);
			break;
		}
		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
			s->ReadPacket();
			//	InvalidateRect(hWnd, NULL, TRUE);
			break;
		case FD_CLOSE:
			closesocket((SOCKET)wParam);
			break;
		}
		break;
	}
	case WM_CREATE:
		chessPiece = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		skyMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		stone= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		baby= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		rabbit= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		argo= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		babyguard= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		GetClientRect(hwnd, &crt);
		count = 0;
		camaraPlayerX = 0;
		camaraPlayerY = 0;
		screenX = 0;
		screenY = 0;
		frameCount = 0;
		SetTimer(hwnd, 1, 100,NULL);
		ZeroMemory(&ip, sizeof(ip));
		break;
	case WM_CHAR:
		if(wParam==VK_RETURN)
			s->socketinit(ip);
		if (wParam == VK_BACK)
		{
			count--;
			if (count < 0)
				count = 0;
			ip[count] = NULL;
		}
		else
			ip[count++] = (TCHAR)wParam;
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if(wParam==VK_LEFT || wParam==VK_RIGHT || wParam==VK_UP || wParam==VK_DOWN)
			s->KeyDown(wParam);
		if (wParam == 'A')
			s->KeyDownAttack(wParam);
		break;
	case WM_KEYUP:
		s->KeyUp();
		break;
	case WM_TIMER:
		//더블버퍼링---------------------------------
		hdc = GetDC(hwnd);
		if (bBitmap == NULL)
			bBitmap = CreateCompatibleBitmap(hdc, 600, 600);
		bMemdc1 = CreateCompatibleDC(hdc);
		bMemdc2 = CreateCompatibleDC(bMemdc1);
		//bMemdc3 = CreateCompatibleDC(bMemdc2);
		oldBit1 = (HBITMAP)SelectObject(bMemdc1, bBitmap); // mem1dc에는 hBit1
		oldBit2 = (HBITMAP)SelectObject(bMemdc2, skyMap); // mem2dc에는 hBit2
	//	oldBit3 = (HBITMAP)SelectObject(bMemdc3, chessPiece);
		camaraPlayerX = s->players[0].getPositionX();
		camaraPlayerY = s->players[0].getPositionY();
		screenX = 0;
		screenY = 0;

		if (camaraPlayerX >= 300)
			screenX = camaraPlayerX - 300;
		if (camaraPlayerY >= 300)
			screenY = camaraPlayerY - 300;
		BitBlt(bMemdc1, 0, 0, 600, 600, bMemdc2, screenX, screenY, SRCCOPY);
			
		TextOut(hdc, 610, 10, ip, lstrlen(ip));
		wsprintf(pos, TEXT("player pos[ %d, %d ]"), s->players[0].getPositionX(), s->players[0].getPositionY());
		TextOut(hdc, 610, 30, pos, lstrlen(pos));
		if(attackDown==s->players[0].getDebuff())
			wsprintf(debuff, "Debuff[Attack 10% Down]");
		if (dependDown == s->players[0].getDebuff())
			wsprintf(debuff, "Debuff[Depend 10% Down]");
		if (noBuff == s->players[0].getDebuff())
			wsprintf(debuff, "Debuff[No DeBuff]");
		TextOut(hdc, 610, 50, debuff, lstrlen(debuff));
		if (noBuff == s->players[0].getDebuff())
			wsprintf(buff, "Debuff[No Buff]");
		TextOut(hdc, 610, 70, buff, lstrlen(buff));
		wsprintf(s->players[0].strinHP, "Player HP : %d", s->players[0].getHp());
		TextOut(hdc, 610,90, s->players[0].strinHP, lstrlen(s->players[0].strinHP));


		//SetBkMode(bMemdc1, TRANSPARENT);
		if (camaraPlayerX >= 300) 
			camaraPlayerX = camaraPlayerX - (camaraPlayerX - 300);
		if(camaraPlayerY >= 300)
			camaraPlayerY = camaraPlayerY - (camaraPlayerY - 300);
	
		drawObject(bMemdc1, chessPiece, camaraPlayerX, camaraPlayerY);
		for (int p = 1; p < MAX_PLAYER; ++p)
		{
			if (true==s->players[p].getConnect())
			{
				drawObject(bMemdc1, chessPiece, s->players[p].getPositionX()- screenX, s->players[p].getPositionY()- screenY);
			}
		}
		for (int m = 0; m < MAX_OBJECT; ++m)
		{
			if (true == s->objects[m].isActive) {
				if (Rabbit == s->objects[m].type) {
					if(attackState==s->objects[m].state)
						drawRabbit(bMemdc1, rabbit, s->objects[m].x - screenX, s->objects[m].y - screenY, frameCount * 30,0);
					else
						drawRabbitWait(bMemdc1, rabbit, s->objects[m].x - screenX, s->objects[m].y - screenY);
				}
				else
					drawStone(bMemdc1, stone, s->objects[m].x - screenX, s->objects[m].y - screenY);
			}
		}

		if (frameCount > 3)
			frameCount = 0;
		else
			frameCount++;
		//SelectObject(bMemdc3, oldBit3); DeleteDC(bMemdc3);
		SelectObject(bMemdc2, oldBit2); DeleteDC(bMemdc2);
		SelectObject(bMemdc1, oldBit1); DeleteDC(bMemdc1);
		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, NULL, false);
		//InvalidateRect(hwnd, NULL, true);
		break;
	case WM_PAINT:
		globalX = 300;
		globalY = 300;
		hdc = BeginPaint(hwnd, &ps);
		//for (int p = 0; p < MAX_PLAYER; ++p)
		//{
		//	if (true==s->players[p].getConnect())
		//	{
		//		//if(p==s->players[0].getID())
		//		//	Rectangle(hdc, globalX, globalY, globalX + 40, globalY + 40);
		//		//else
		//			Rectangle(hdc, s->players[p].getPositionX() , s->players[p].getPositionY(),
		//				s->players[p].getPositionX()+20, s->players[p].getPositionY()+20);
		//	}
		//}
		//for (int m = 0; m < MAX_OBJECT; ++m)
		//{
		//	if (true == s->objects[m].isActive) {
		//		if(Rabbit==s->objects[m].type)
		//			Rectangle(hdc, s->objects[m].x, s->objects[m].y,
		//				s->objects[m].x + 20, s->objects[m].y + 20);
		//		else
		//			Ellipse(hdc, s->objects[m].x, s->objects[m].y,
		//				s->objects[m].x + 20, s->objects[m].y + 20);
		//	}

		//}
		//--------------더블버퍼링-------------------------
		bMemdc1 = CreateCompatibleDC(hdc);
		// hBit1에는 배경과 텍스트가 출력된 비트맵이 저장, mem1dc에 설정
		oldBit1 = (HBITMAP)SelectObject(bMemdc1, bBitmap);
		// mem1dc에 있는 내용을 hdc에 뿌려준다.
		BitBlt(hdc, 0, 0,600,600, bMemdc1, 0, 0, SRCCOPY);

		SelectObject(bMemdc1, oldBit1);
		DeleteDC(bMemdc2);
		//DeleteDC(bMemdc3);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
