// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include<iostream>
#include<vector>
#include<thread>
#include<WinSock2.h>
#include<Windows.h>
#include<time.h>
#include<queue>
#include<mutex>
#include<map>
#include <D3DX10Math.h>
#include<unordered_set>
#include"protocol.h"
#include"packetType.h"
#include"Handle.h"
#include"TimerThread.h"
#include"Object.h"
#include"Player.h"
#include"GameBoard.h"
#include"Sector.h"
#include"Server.h"

using namespace std;



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
