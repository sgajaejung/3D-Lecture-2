#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include "../../math/Math.h"
#include "DrawTriangle.h"

using namespace std;

const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
const int WINPOS_X = 0; //�ʱ� ������ ���� ��ġ X
const int WINPOS_Y = 0; //�ʱ� ������ ���� ��ġ Y

HWND g_hWnd;
vector<Vector3> g_vertices;
vector<int> g_indices;
Matrix44 g_matWorld1;
Matrix44 g_matLocal1;
Matrix44 g_matView;
Matrix44 g_matProjection;
Matrix44 g_matViewPort;
Vector3 g_cameraPos(0,200,-200);
Vector3 g_cameraLookat(0,0,0);


// �ݹ� ���ν��� �Լ� ������ Ÿ��
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
void Init();
void MainLoop(int elapse_time);
void	Render(HWND hWnd);
void	Paint(HWND hWnd, HDC hdc);


int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance, 
					 LPSTR lpCmdLine, 
					 int nCmdShow)
{
	wchar_t className[32] = L"Sample";
	wchar_t windowName[32] = L"Sample";

	//������ Ŭ���� ���� ����
	//���� �̷��� ������ ����ڴ� ��� ����
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.cbWndExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		//������ ������
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//�������� Ŀ����� ����
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//����������ܸ�� ����
	WndClass.hInstance = hInstance;				//���α׷��ν��Ͻ��ڵ� 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//������ ���ν��� �Լ� ������
	WndClass.lpszMenuName = NULL;						//�޴��̸� ������ NULL
	WndClass.lpszClassName = className;				//���� �ۼ��ϰ� �ִ� ������ Ŭ������ �̸�
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//������ �׸��� ��� ���� ( ����� ����ɶ� ȭ�鰻�� CS_HREDRAW | CS_VREDRAW )

	//������ �ۼ��� ������ Ŭ�������� ���
	RegisterClass( &WndClass );

	//������ ����
	//������ ������ �ڵ��� �������� g_hWnd �� �޴´�.
	HWND hWnd = CreateWindow(
		className,				//�����Ǵ� �������� Ŭ�����̸�
		windowName,				//������ Ÿ��Ʋ�ٿ� ��µǴ� �̸�
		WS_OVERLAPPEDWINDOW,	//������ ��Ÿ�� WS_OVERLAPPEDWINDOW
		WINPOS_X,				//������ ���� ��ġ X 
		WINPOS_Y,				//������ ���� ��ġ Y
		WINSIZE_X,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		WINSIZE_Y,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		GetDesktopWindow(),		//�θ� ������ �ڵ� ( ���α׷����� �ֻ��� ������� NULL �Ǵ� GetDesktopWindow() )
		NULL,					//�޴� ID ( �ڽ��� ��Ʈ�� ��ü�� �������ΰ�� ��Ʈ�� ID �� ��	
		hInstance,				//�� �����찡 ���� ���α׷� �ν��Ͻ� �ڵ�
		NULL					//�߰� ���� NULL ( �Ű���� )
		);

	g_hWnd = hWnd;

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	ShowWindow( hWnd, nCmdShow );

	Init();


	//�޽��� ����ü
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	int oldT = GetTickCount();
	while (msg.message != WM_QUIT)
	{
		//PeekMessage �� �޽��� ť�� �޽����� ��� ���α׷��� ���߱� �ʰ� ������ �ȴ�.
		//�̶� �޽���ť�� �޽����� ������ false �� ���ϵǰ� �޽����� ������ true �� �����̵ȴ�.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
			DispatchMessage( &msg );  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}
		else
		{
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			if (elapseT > 15)
			{
				oldT = curT;
				MainLoop(elapseT);
			}
		}
	}

	return 0;
}


//
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_ERASEBKGND:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Paint(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		break;
	case WM_DESTROY: //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


void Init()
{

	// vertices1
	//       (-50,+50, +50)  ----------------- (+50, +50, +50)
	//       |                                                       |
	//       |                         +                           |
	//       |                                                       |
	//       (-50,+50, -50)  ----------------- (+50, +50, -50)
	//
	//       (-50,-50, +50)  ----------------- (+50, -50, +50)
	//       |                                                       |
	//       |                         +                           |
	//       |                                                       |
	//       (-50,-50, -50)  ----------------- (+50, -50, -50)

	const float w = 30.f;
	g_vertices.reserve(128);
	g_vertices.push_back( Vector3(-w,w,w) );
	g_vertices.push_back( Vector3(w,w,w) );
	g_vertices.push_back( Vector3(w,w,-w) );
	g_vertices.push_back( Vector3(-w,w,-w) );

	g_vertices.push_back( Vector3(-w,-w,w) );
	g_vertices.push_back( Vector3(w,-w,w) );
	g_vertices.push_back( Vector3(w,-w,-w) );
	g_vertices.push_back( Vector3(-w,-w,-w) );


	g_indices.reserve(128);
	// top
	g_indices.push_back(0);
	g_indices.push_back(2);
	g_indices.push_back(3);
	g_indices.push_back(0);
	g_indices.push_back(1);
	g_indices.push_back(2);
	// front
	g_indices.push_back(3);
	g_indices.push_back(2);
	g_indices.push_back(7);
	g_indices.push_back(2);
	g_indices.push_back(6);
	g_indices.push_back(7);
	// back
	g_indices.push_back(1);
	g_indices.push_back(4);
	g_indices.push_back(5);
	g_indices.push_back(1);
	g_indices.push_back(0);
	g_indices.push_back(4);
	// left
	g_indices.push_back(3);
	g_indices.push_back(4);
	g_indices.push_back(0);
	g_indices.push_back(7);
	g_indices.push_back(4);
	g_indices.push_back(3);
	// right
	g_indices.push_back(2);
	g_indices.push_back(5);
	g_indices.push_back(6);
	g_indices.push_back(2);
	g_indices.push_back(1);
	g_indices.push_back(5);
	// bottom
	g_indices.push_back(4);
	g_indices.push_back(7);
	g_indices.push_back(6);
	g_indices.push_back(4);
	g_indices.push_back(6);
	g_indices.push_back(5);

	g_matWorld1.SetTranslate(Vector3(0,0,0));

	Vector3 dir = g_cameraLookat - g_cameraPos;
	dir.Normalize();
	g_matView.SetView(g_cameraPos, dir, Vector3(0,1,0));
	g_matProjection.SetProjection( MATH_PI / 4.f, 1.0f, 1.0f, 100.0f );

	RECT cr;
	::GetClientRect(g_hWnd, &cr);
	const float width = (float)(cr.right-cr.left);
	const float height = (float)(cr.bottom - cr.top);
	g_matViewPort._11 = width/2;
	g_matViewPort._22 = -height/2;
	g_matViewPort._33 = 0;
	g_matViewPort._41 = width/2;
	g_matViewPort._42 = height/2;
	g_matViewPort._43 = 0;
}


/**
 @brief 
 */
void	MainLoop(int elapse_time)
{
	Matrix44 mat;
	mat.SetRotationY(elapse_time/500.f);
	g_matLocal1 *= mat;

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
		Vector3 p = vertices[ i] * tm;

		if (0 == i)
			MoveToEx(hdc, (int)p.x, (int)p.y, NULL);
		else
			LineTo(hdc, (int)p.x, (int)p.y);
	}
}


void RenderIndices(HDC hdc, const vector<Vector3> &vertices, const vector<int> &indices, const Matrix44 &tm)
{
	Vector3 camDir(0,0,1);

	for (unsigned int i=0; i < indices.size(); i+=3)
	{
		Vector3 p1 = vertices[ indices[ i]];
		Vector3 p2 = vertices[ indices[ i+1]];
		Vector3 p3 = vertices[ indices[ i+2]];

		p1 = p1 * tm;
		p2 = p2 * tm;
		p3 = p3 * tm;

		// culling

		Rasterizer::Color color(255,0,0,1);
		Rasterizer::DrawLine(hdc, color, p1.x, p1.y,color, p2.x, p2.y);
		Rasterizer::DrawLine(hdc, color, p1.x, p1.y,color, p3.x, p3.y);
		Rasterizer::DrawLine(hdc, color, p3.x, p3.y,color, p2.x, p2.y);
		//Rasterizer::DrawTriangle(hdc, color, p1.x, p1.y, color, p2.x, p2.y, color, p3.x, p3.y);
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

	RenderIndices(hdcMem, g_vertices, g_indices, g_matLocal1 * g_matWorld1 * g_matView);

	BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}

