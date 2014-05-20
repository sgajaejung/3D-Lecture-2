
#include "stdafx.h"
#include "2DTransform.h"
#include "math/Math.h"
#include <vector>
#include <algorithm>

using namespace std;
#define MAX_LOADSTRING 100

HINSTANCE hInst;
HWND g_hWnd;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
vector<Vector3> g_vertices1;
vector<Vector3> g_vertices2;
Matrix44 g_matWorld1;
Matrix44 g_matLocal1;
Matrix44 g_matWorld2;
Matrix44 g_matLocal2;
Matrix44 g_matWorld3;
Matrix44 g_matLocal3;


ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void					MainLoop(int elapse_time);
void					Render(HWND hWnd);
void					Paint(HWND hWnd, HDC hdc);


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY2DTRANSFORM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// vertices1
	//       (-50,-50)  ----------------- (+50, -50)
	//       |                                                       |
	//       |                         +                           |
	//       |                                                       |
	//       (-50,+50)  ----------------- (+50, +50)
	const float w = 50.f;
	g_vertices1.push_back( Vector3(-w,-w,1) );
	g_vertices1.push_back( Vector3(w,-w,1) );
	g_vertices1.push_back( Vector3(w,w,1) );
	g_vertices1.push_back( Vector3(-w,w,1) );
	g_vertices1.push_back( Vector3(-w,-w,1) );


	// vertices2
	//       +(0,0)  ----------------- (+100, 0)
	//       |                                               |
	//       |                                               |
	//       |                                               |
	//       (0,+100)  ------------- (+1-0, +100)
	const float w2 = 100.f;
	g_vertices2.push_back( Vector3(0,0,1) );
	g_vertices2.push_back( Vector3(w2,0,1) );
	g_vertices2.push_back( Vector3(w2,w2,1) );
	g_vertices2.push_back( Vector3(0,w2,1) );
	g_vertices2.push_back( Vector3(0,0,1) );


	g_matWorld1.SetIdentity();
	g_matWorld1.Translate(Vector3(150,200,0));
	g_matLocal1.SetIdentity();

	g_matWorld2.SetIdentity();
	g_matWorld2.Translate(Vector3(400,200,0));	
	g_matLocal2.SetIdentity();

	g_matWorld3.SetIdentity();
	g_matWorld3.Translate(Vector3(600,200,0));	
	g_matLocal3.SetIdentity();


	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2DTRANSFORM));
	int oldT = GetTickCount();
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		const int curT = GetTickCount();
		const int elapseT = min(curT - oldT, 100);
		oldT = curT;
		MainLoop(elapseT);	
	}

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2DTRANSFORM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY2DTRANSFORM);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Paint(hWnd, ps.hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			{
				Matrix44 mat;
				mat.SetRotationZ(0.1f);
				g_matLocal1 *= mat;
			}
			break;
		case VK_RIGHT:
			{
				Matrix44 mat;
				mat.SetRotationZ(-0.1f);
				g_matLocal1 *= mat;
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


/**
 @brief 
 */
void	MainLoop(int elapse_time)
{
	Matrix44 mat2;
	mat2.SetRotationZ(1.f * (elapse_time*0.001f));
	g_matLocal2 *= mat2;

	Matrix44 mat3;
	mat3.SetRotationZ(-4.f * (elapse_time*0.001f));
	g_matLocal3 *= mat3;

	static int incT = 0;
	incT += elapse_time;
	Vector3 pos(0, sin(incT*0.001f)*200.f, 0);

	Matrix44 matT3;
	matT3.SetIdentity();
	matT3.Translate( Vector3(600,200,0) + pos );
	g_matWorld3 = matT3;


	// Render
	Render(g_hWnd);
	::InvalidateRect(g_hWnd, NULL, TRUE);
}


/**
 @brief 
 */
void	Render(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	Paint(hWnd, hdc);
	::ReleaseDC(hWnd, hdc);
}


void RenderVertices(HDC hdc, const vector<Vector3> &vertices, const Matrix44 &tm)
{
	for (unsigned int i=0; i < vertices.size(); ++i)
	{
		Vector3 p = vertices[ i];
		p = p * tm;

		if (0 == i)
			MoveToEx(hdc, (int)p.x, (int)p.y, NULL);
		else
			LineTo(hdc, (int)p.x, (int)p.y);
	}
}


/**
 @brief 
 */
void Paint(HWND hWnd, HDC hdc)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right-rc.left, rc.bottom-rc.top);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
	HBRUSH hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	FillRect(hdcMem, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);

	RenderVertices(hdcMem, g_vertices1, g_matLocal1 * g_matWorld1);
	RenderVertices(hdcMem, g_vertices2, g_matLocal2 * g_matWorld2);
	RenderVertices(hdcMem, g_vertices1, g_matLocal3 * g_matWorld3);

	BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}

