
#pragma once


#include "gamemain/GameMain.h"

#pragma comment( lib, "winmm.lib" )


// �����ӿ�ũ �ʱ�ȭ. 
// WinMain �� �����Ѵ�.
#define INIT_FRAMEWORK(frameWorkClassName) \
	framework::cGameMain* framework::CreateFrameWork()\
	{\
		return new frameWorkClassName();\
	}\
	\
	int APIENTRY WinMain(HINSTANCE hInstance, \
		HINSTANCE hPrevInstance, \
		LPSTR lpCmdLine, \
		int nCmdShow)\
	{\
		framework::FrameWorkWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);\
	}
