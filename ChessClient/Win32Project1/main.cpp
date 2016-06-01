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
Server *s = Server::getInstangce();

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

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HWND hDlg = NULL;
	hWnd = hDlg;
	HDC hdc;
	HDC memdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HBRUSH hBrushBlack, oldBrushBlack;
	HBITMAP chessPiece;
	static char ip[10];
	static char pos[20];
	static int count;
	int playerX, playerY, playerEX, playerEY;
	int x=0, y=0;
	playerX = 0;
	playerY = 0;
	playerEX = 0;
	playerEY = 0;
	chessPiece = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	switch (iMsg)
	{
	case WM_CREATE:
		x = 0;
		y = 0;

		count = 0;
		SetTimer(hwnd, 1, 200,NULL);
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
		s->KeyDown(wParam);
		break;
	case WM_TIMER:
		playerX = s->players[0].getPositionX() - 5;
		playerY = s->players[0].getPositionY() - 5;
		playerEX = s->players[0].getPositionX() + 5;
		playerEY = s->players[0].getPositionY() + 5;
		if (playerX < 0)
			playerX = 0;
		if (playerY < 0)
			playerY = 0;
		if (playerEX > 100)
			playerEX = 100;
		if (playerEY > 100)
			playerEY = 100;
		InvalidateRect(hWnd, NULL, false);
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		wsprintf(pos, "player pos[ %d, %d ]", s->players[0].getPositionX(), s->players[0].getPositionY());
		TextOut(hdc, 10, 700, ip, sizeof(ip));
		TextOut(hdc, 10, 720, pos, sizeof(pos));
		for (int i = playerX; i < playerEX; ++i)
		{
			for (int j = playerY; j < playerEY; ++j)
			{
				if (s->board[i][j].getType() == 1)
				{
					hBrushBlack = CreateSolidBrush(RGB(0, 255, 128));
					oldBrushBlack = (HBRUSH)SelectObject(hdc, hBrushBlack);
					Rectangle(hdc,
						x,
						y,
						x + WIDTHRECTANGLE,
						y + HEIGHTRECTANGLE);
					SelectObject(hdc, oldBrushBlack);
					DeleteObject(hBrushBlack);
				}
				else
				{
					hBrush = CreateSolidBrush(RGB(255, 255, 255));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc,
						x,
						y,
						x + WIDTHRECTANGLE,
						y + HEIGHTRECTANGLE);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
				
				for (int p = 0; p < MAX_PLAYER; ++p)
				{
					if (s->players[p].getPositionX() == i && s->players[p].getPositionY() == j)
					{
						memdc = CreateCompatibleDC(hdc);
						(HBITMAP)SelectObject(memdc, chessPiece);
						TransparentBlt(hdc, x, y,
							WIDTHRECTANGLE, HEIGHTRECTANGLE, memdc, 0, 0, 35, 62, RGB(255, 255, 255));
						DeleteDC(memdc);
					}
				}
				x += WIDTHRECTANGLE;
			}
			y += HEIGHTRECTANGLE;
			x = 0;
		}
		y = 0;
		EndPaint(hwnd, &ps);
		break;
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
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
