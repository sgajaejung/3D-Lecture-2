
#include "stdafx.h"
#include "GameApp.h"

INIT_FRAMEWORK(cGameApp)


cGameApp::cGameApp()
{
	m_windowName = L"GameApp";
	const RECT r = {0, 0, 800, 600};
	m_windowRect = r;
}

cGameApp::~cGameApp()
{

}


bool cGameApp::OnInit()
{

	return true;
}


void cGameApp::OnUpdate(const float elapseT)
{

}


void cGameApp::OnRender(const float elapseT)
{

}


void cGameApp::OnShutdown()
{

}


void cGameApp::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{

}
