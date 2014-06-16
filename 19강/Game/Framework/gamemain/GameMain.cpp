
#include "stdafx.h"
#include "GameMain.h"
#include <MMSystem.h>
#include "../window/utility.h"


using namespace framework;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//------------------------------------------------------------------------
// WindowMain 함수
//------------------------------------------------------------------------
int framework::FrameWorkWinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	const HWND hWnd = InitWindow(hInstance, 
		cGameMain::Get()->GetWindowName(), 
		cGameMain::Get()->GetWindowRect(), 
		nCmdShow,
		WndProc);

	if (hWnd == NULL)
		return 0;

	if (!cGameMain::Get()->Init(hWnd))
		return 0;

	cGameMain::Get()->Run();
	cGameMain::Get()->ShutDown();
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameMain::Get()->MessageProc(message, wParam, lParam);

	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		case VK_RETURN:
			break;
		}
		break;

	case WM_DESTROY:
		cGameMain::Get()->Exit();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




/////////////////////////////////////////////////////////////////////////////////////////////
cGameMain *cGameMain::m_pInstance = NULL;

cGameMain::cGameMain() :
	m_DxDevice(NULL)
{

}


cGameMain::~cGameMain()
{

}

// singleton function
cGameMain* cGameMain::Get()
{
	if (!m_pInstance)
		m_pInstance = CreateFrameWork();
	return m_pInstance;
}


bool cGameMain::Init(HWND hWnd)
{
	m_state = INIT;
	m_hWnd = hWnd;

	if (!graphic::InitDirectX(hWnd, 
		m_windowRect.right-m_windowRect.left,
		m_windowRect.bottom-m_windowRect.top,
		m_DxDevice))
		return false;

	if (!OnInit())
		return false;

	return true;
}


void cGameMain::ShutDown()
{
	m_DxDevice->Release();
	m_DxDevice = NULL;
	OnShutdown();
}


void cGameMain::Run()
{
	m_state = RUN; // Framework 실행

	MSG msg;
	while (RUN == m_state)
	{
		if (PeekMessage( &msg, m_hWnd, 0, 0, PM_NOREMOVE ))
		{
			GetMessage(&msg, NULL, 0, 0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		const int curT = timeGetTime();
		static int oldT = curT;
		const int elapseT = curT - oldT;
		const float t = elapseT * 0.001f;
		oldT = curT;

		Update(t);
		Render(t);
	}
}


void cGameMain::Update(const float elapseT)
{
	OnUpdate(elapseT);
}


void cGameMain::Render(const float elapseT)
{
	OnRender(elapseT);
}


void	cGameMain::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{

}


void cGameMain::Exit()
{
	m_state = SHUTDOWN;
}

const wstring& cGameMain::GetWindowName()
{
	return m_windowName;
}
const RECT& cGameMain::GetWindowRect()
{
	return m_windowRect;
}
