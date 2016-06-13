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
	static HDC bMemdc1,bMemdc2;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HBRUSH hBrushBlack, oldBrushBlack;
	static HBITMAP chessPiece,skyMap,oldBit1,oldBit2,oldBit3;
	static char ip[10];
	static char pos[20];
	static int count;
	//chessPiece = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	//skyMap= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
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
	}
	break;
	case WM_CREATE:
		GetClientRect(hwnd, &crt);
		count = 0;
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
	case WM_TIMER:
		//더블버퍼링---------------------------------
		//hdc = GetDC(hwnd);
		//if (bBitmap == NULL)
		//	bBitmap = CreateCompatibleBitmap(hdc, 600, 600);
		//bMemdc1 = CreateCompatibleDC(hdc);
		//bMemdc2 = CreateCompatibleDC(bMemdc1);
		//oldBit1 = (HBITMAP)SelectObject(bMemdc1, bBitmap); // mem1dc에는 hBit1
		//oldBit2 = (HBITMAP)SelectObject(bMemdc2, skyMap); // mem2dc에는 hBit2
		//BitBlt(bMemdc1, 0, 0, 600, 600, bMemdc2, 0, 0, SRCCOPY);

		//wsprintf(pos, "player pos[ %d, %d ]", s->players[0].getPositionX(), s->players[0].getPositionY());
		//TextOut(hdc, 10, 660, ip, sizeof(ip));
		//TextOut(hdc, 10, 680, pos, sizeof(pos));
		////oldBit3 = (HBITMAP)SelectObject(bMemdc2, chessPiece);
		//
		////SetBkMode(bMemdc1, TRANSPARENT);
		//for (int p = 0; p < MAX_PLAYER; ++p)
		//{
		//	if (true==s->players[p].getConnect())
		//	{
		//		drawObject(bMemdc1, chessPiece,s->players[p].getPositionX(),s->players[p].getPositionY());
		//	}
		//}
		//
		////SelectObject(bMemdc2, oldBit3); DeleteDC(bMemdc2);
		//SelectObject(bMemdc2, oldBit2); DeleteDC(bMemdc2);
		//SelectObject(bMemdc1, oldBit1); DeleteDC(bMemdc1);
		//ReleaseDC(hwnd, hdc);
		//InvalidateRect(hwnd, NULL, false);
		InvalidateRect(hwnd, NULL, true);
		break;
	case WM_PAINT:
		globalX = 300;
		globalY = 300;
		hdc = BeginPaint(hwnd, &ps);
		for (int p = 0; p < MAX_PLAYER; ++p)
		{
			if (true==s->players[p].getConnect())
			{
				//if(p==s->players[0].getID())
				//	Rectangle(hdc, globalX, globalY, globalX + 40, globalY + 40);
				//else
					Rectangle(hdc, s->players[p].getPositionX() , s->players[p].getPositionY(),
						s->players[p].getPositionX()+20, s->players[p].getPositionY()+20);
			}
		}
		for (int m = 0; m < MAX_OBJECT; ++m)
		{
			if (true == s->objects[m].isActive) {
				if(Rabbit==s->objects[m].type)
					Rectangle(hdc, s->objects[m].x, s->objects[m].y,
						s->objects[m].x + 20, s->objects[m].y + 20);
				else
					Ellipse(hdc, s->objects[m].x, s->objects[m].y,
						s->objects[m].x + 20, s->objects[m].y + 20);
			}

		}
		//--------------더블버퍼링-------------------------
		//bMemdc1 = CreateCompatibleDC(hdc);
		//// hBit1에는 배경과 텍스트가 출력된 비트맵이 저장, mem1dc에 설정
		//oldBit1 = (HBITMAP)SelectObject(bMemdc1, bBitmap);
		//// mem1dc에 있는 내용을 hdc에 뿌려준다.
		//BitBlt(hdc, 0, 0,600,600, bMemdc1, 0, 0, SRCCOPY);

		//SelectObject(bMemdc1, oldBit1);
		//DeleteDC(bMemdc2);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
