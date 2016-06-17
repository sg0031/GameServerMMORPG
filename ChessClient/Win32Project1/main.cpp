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
void drawPlayer(HDC hdc, HBITMAP image, int posx, int posy,int sprightx)
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
void drawAttackPlayer(HDC hdc, HBITMAP image, int posx, int posy)
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
void drawEffect(HDC hdc, HBITMAP image, int posx, int posy)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, image);
	TransparentBlt(hdc, posx-20, posy-20,
		100, 100, MemDC, 0, 0, 200, 200, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
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
void drawRabbitAttack(HDC hdc, HBITMAP image, int posx, int posy, int sprightx, int sprighty)
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
void drawRabbitDead(HDC hdc, HBITMAP image, int posx, int posy)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, image);
	TransparentBlt(hdc, posx, posy,
		WIDTHRECTANGLE, HEIGHTRECTANGLE, MemDC, 0, 0, 0, 0, RGB(255, 255, 255));
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
	WndClass.lpszClassName = L"Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow(L"Window Class Name",
		L"Window Title Name",
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
	static bool connectIp,chatEnterFlag;
	static HBITMAP chessPiece,skyMap,oldBit1,oldBit2,oldBit3;
	static HBITMAP stone,baby,rabbit,argo,babyguard;
	static HBITMAP player, attack, fireAttack, fireEffect;
	static WCHAR ip[10];
	static WCHAR pos[100];
	static WCHAR debuff[100];
	static WCHAR buff[100];
	static int chatLine, chatCount;
	static WCHAR myChatString[100];//내가 입력하는 글씨
	static WCHAR myViewString[100];
	static int myCharCount;
	static int count;
	static int printLine;
	static float attacktime;
	int camaraPlayerX;
	int camaraPlayerY;
	int screenX;
	int screenY;
	static int frameCount,walkCount,effectCount;
	
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
		attacktime = 0.0;
		printLine = 1;
		chatLine = 0;
		chatCount = 0;
		myCharCount = 0;
		CreateCaret(hWnd, NULL, 10, 10);
		SetCaretPos(700, 10);
		chatEnterFlag = false;
		connectIp = false;
		chessPiece = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		skyMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		stone= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		baby= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		rabbit= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		argo= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		babyguard= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		player= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		attack= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		fireAttack= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP10));
		fireEffect= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		GetClientRect(hwnd, &crt);
		count = 0;
		camaraPlayerX = 0;
		camaraPlayerY = 0;
		screenX = 0;
		screenY = 0;
		frameCount = 0;
		walkCount = 0;
		effectCount = 0;
		SetTimer(hwnd, 1, 14,NULL);
		ZeroMemory(&ip, sizeof(ip));
		break;
	case WM_CHAR:
		if (wParam == VK_RETURN &&chatEnterFlag == false && connectIp == true)
		{
			chatEnterFlag = true;
		}
		else if (chatEnterFlag == true)
		{
			if (wParam == VK_BACK) {
				myCharCount--;
				if (myCharCount < 0)
					myCharCount = 0;
				myChatString[myCharCount] = NULL;
			}
			else if (wParam == VK_RETURN) {
				for (int i = 5; i > 0; --i) {
					memcpy(&s->chatWindow[i], &s->chatWindow[i-1], 100);
				}
				ZeroMemory(&s->chatWindow[0],100);
				wsprintf(myViewString, L"%d : %s", s->players[0].getID(), myChatString);
				s->SendMes(myViewString);
				memcpy(&s->chatWindow[0], &myViewString, sizeof(myViewString));
				chatEnterFlag = false;
				myCharCount = 0;
				ZeroMemory(&myChatString, sizeof(myChatString));
			}
			else
				myChatString[myCharCount++] = (TCHAR)wParam;
		}
		InvalidateRect(hwnd, NULL, true);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN && connectIp == false) {
			s->socketinit(ip); connectIp = true;
		}
		if (wParam == VK_BACK&& connectIp == false)
		{
			count--;
			if (count < 0)
				count = 0;
			ip[count] = NULL;
		}
		if(connectIp ==false){
			ip[count++] = (TCHAR)wParam;
		}
		if(wParam==VK_LEFT || wParam==VK_RIGHT || wParam==VK_UP || wParam==VK_DOWN)
			s->KeyDown(wParam);
		if (wParam == 'A' && chatEnterFlag == false) {
			if (attacktime < GetTickCount()) {
				attacktime = GetTickCount()+1000;
				s->KeyDownAttack(wParam);
			}
		}
		if (wParam == 'Q'&& chatEnterFlag==false)
			s->KeyDownAttack(wParam);
		if (wParam == 'I'&& chatEnterFlag == false)
			s->KeyDownAttack(wParam);
		if (wParam == 'O'&& chatEnterFlag == false)
			s->KeyDownAttack(wParam);
		if (wParam == 'P'&& chatEnterFlag == false)
			s->KeyDownAttack(wParam);
		break;
	case WM_KEYUP:
		s->KeyUp();
		break;
	case WM_TIMER:
		//더블버퍼링---------------------------------
		hdc = GetDC(hwnd);
		if (bBitmap == NULL)
			bBitmap = CreateCompatibleBitmap(hdc, 500, 500);
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

		if (camaraPlayerX >= 250)
			screenX = camaraPlayerX - 250;
		if (camaraPlayerY >= 250)
			screenY = camaraPlayerY - 250;
		BitBlt(bMemdc1, 0, 0, 500, 500, bMemdc2, screenX, screenY, SRCCOPY);
		for (auto i = 5; i >=0; --i)
		{
			TextOut(hdc, 10, 510+(printLine*20), s->chatWindow[i], lstrlen(s->chatWindow[i]));
			TextOut(hdc, 510, 510 + (printLine * 20), s->stateMessage[i], lstrlen(s->stateMessage[i]));
			printLine++;
		}
		printLine = 1;
		TextOut(hdc, 10,510, myChatString, lstrlen(myChatString));
		TextOut(hdc, 510, 10, ip, lstrlen(ip));
		wsprintf(pos, L"player pos[ %d, %d ]", s->players[0].getPositionX(), s->players[0].getPositionY());
		TextOut(hdc, 510, 30, pos, lstrlen(pos));
		if(attackDown==s->players[0].getDebuff())
			wsprintf(debuff, L"Debuff[Attack 10% Down]");
		if (dependDown == s->players[0].getDebuff())
			wsprintf(debuff, L"Debuff[Depend 10% Down]");
		if (noBuff == s->players[0].getDebuff())
			wsprintf(debuff, L"Debuff[No DeBuff]");
		TextOut(hdc, 510, 50, debuff, lstrlen(debuff));
		if (noBuff == s->players[0].getDebuff())
			wsprintf(buff, L"Debuff[No Buff]");
		TextOut(hdc, 510, 70, buff, lstrlen(buff));
		wsprintf(s->players[0].strinHP, L"[Player HP : %d/%d]", s->players[0].getHp(),s->players[0].getMaxHP());
		TextOut(hdc, 510, 90, s->players[0].strinHP, lstrlen(s->players[0].strinHP));
		
		wsprintf(s->players[0].strinLevel, L"[level:%d statusCount:%d]", s->players[0].getLevel(), s->players[0].getStatusCount());
		TextOut(hdc, 510, 110, s->players[0].strinLevel, lstrlen(s->players[0].strinLevel));

		wsprintf(s->players[0].strinAttack, L"[attack:%d depend:%d]", s->players[0].getAttack(), s->players[0].getDepend());
		TextOut(hdc, 510, 130, s->players[0].strinAttack, lstrlen(s->players[0].strinAttack));

		wsprintf(s->players[0].strinExp, L"[exp:%d gold:%d]", s->players[0].getExp(), s->players[0].getGold());
		TextOut(hdc, 510, 150, s->players[0].strinExp, lstrlen(s->players[0].strinExp));

		wsprintf(s->players[0].strinSTR, L"[str:%d]", s->players[0].getStr());
		TextOut(hdc, 510, 170, s->players[0].strinSTR, lstrlen(s->players[0].strinSTR));

		wsprintf(s->players[0].strinDEX, L"[dex:%d]", s->players[0].getDex());
		TextOut(hdc, 510, 190, s->players[0].strinDEX, lstrlen(s->players[0].strinDEX));

		wsprintf(s->players[0].strinMental, L"[mental:%d]", s->players[0].getMental());
		TextOut(hdc, 510, 210, s->players[0].strinMental, lstrlen(s->players[0].strinMental));
		//SetBkMode(bMemdc1, TRANSPARENT);
		if (camaraPlayerX >= 250) 
			camaraPlayerX = camaraPlayerX - (camaraPlayerX - 250);
		if(camaraPlayerY >= 250)
			camaraPlayerY = camaraPlayerY - (camaraPlayerY - 250);
	
		if(walkPlayer==s->players[0].getState())
			drawPlayer(bMemdc1, player, camaraPlayerX, camaraPlayerY,walkCount*40);
		if(waitPlayer == s->players[0].getState())
			drawPlayer(bMemdc1, player, camaraPlayerX, camaraPlayerY, walkCount * 40);
		if(attackPlayer==s->players[0].getState())
			drawAttackPlayer(bMemdc1, attack, camaraPlayerX, camaraPlayerY);
		if (fireSkill == s->players[0].getState()) {
			drawEffect(bMemdc1, fireEffect, camaraPlayerX, camaraPlayerY);
		}
		else
			drawPlayer(bMemdc1, player, camaraPlayerX, camaraPlayerY, walkCount * 40);
		for (int p = 1; p < MAX_PLAYER; ++p)
		{
			if (true==s->players[p].getConnect())
			{
				wsprintf(s->players[p].strinHP, L"[%d(%d):%d/%d]",s->players[p].getID(), s->players[p].getLevel(), 
					s->players[p].getHp(),s->players[p].getMaxHP());
				TextOut(bMemdc1, s->players[p].getPositionX() - screenX-30, s->players[p].getPositionY() - screenY-20,
					s->players[p].strinHP, lstrlen(s->players[p].strinHP));
				if(walkPlayer== s->players[p].getState())
					drawPlayer(bMemdc1, player, s->players[p].getPositionX()- screenX, s->players[p].getPositionY()- screenY, walkCount*40);
				if (waitPlayer == s->players[p].getState())
					drawPlayer(bMemdc1, player, s->players[p].getPositionX() - screenX, s->players[p].getPositionY() - screenY, walkCount * 40);
				if (attackPlayer == s->players[p].getState())
					drawAttackPlayer(bMemdc1, attack, s->players[p].getPositionX() - screenX, s->players[p].getPositionY() - screenY);
				if (fireSkill == s->players[p].getState()) {
					drawEffect(bMemdc1, fireEffect, s->players[p].getPositionX() - screenX, s->players[p].getPositionY() - screenY);
				}
				else
					drawPlayer(bMemdc1, player, s->players[p].getPositionX() - screenX, s->players[p].getPositionY() - screenY, walkCount * 40);
			}
		}
		for (int m = 0; m < MAX_OBJECT; ++m)
		{
			if (true == s->objects[m].isActive) {
				if (Rabbit == s->objects[m].type) {
					if (attackState == s->objects[m].state) {
						drawRabbitAttack(bMemdc1, rabbit, s->objects[m].x - screenX, s->objects[m].y - screenY, frameCount * 40, 0);
						if (frameCount > 3)
							frameCount = 0;
						else
							frameCount++;
					}
					if(deadState==s->objects[m].state)
						drawRabbitDead(bMemdc1, rabbit, s->objects[m].x - screenX, s->objects[m].y - screenY);
					else
						drawRabbitWait(bMemdc1, rabbit, s->objects[m].x - screenX, s->objects[m].y - screenY);
				}
				else
					drawStone(bMemdc1, stone, s->objects[m].x - screenX, s->objects[m].y - screenY);
			}
		}


		//SelectObject(bMemdc3, oldBit3); DeleteDC(bMemdc3);
		SelectObject(bMemdc2, oldBit2); DeleteDC(bMemdc2);
		SelectObject(bMemdc1, oldBit1); DeleteDC(bMemdc1);
		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, NULL, false);
		//InvalidateRect(hwnd, NULL, true);
		break;
	case WM_PAINT:
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
		BitBlt(hdc, 0, 0,500,500, bMemdc1, 0, 0, SRCCOPY);
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
