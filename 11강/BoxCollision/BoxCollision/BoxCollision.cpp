#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <fstream>
#include "../math/Math.h"
#include "DrawTriangle.h"

using namespace std;

const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y

HWND g_hWnd;
vector<Vector3> g_vertices;
vector<Vector3> g_normals;
vector<int> g_indices;
Matrix44 g_matWorld1;
Matrix44 g_matLocal1;

Matrix44 g_matWorld2;
Matrix44 g_matLocal2;

Matrix44 g_matView;
Matrix44 g_matProjection;
Matrix44 g_matViewPort;
Vector3 g_cameraPos(0,1000,-1000);
Vector3 g_cameraLookat(0,0,0);

Box g_box1;
Box g_box2;
bool g_isCollision = false;


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
void Init();
void MainLoop(int elapse_time);
void	Render(HWND hWnd);
void	Paint(HWND hWnd, HDC hdc);
bool ReadModelFile( const string &fileName, vector<Vector3> &vertices, vector<int> &indices, 
	vector<Vector3> &normals);
void GetVerticesMinMax( const vector<Vector3> &vertices, OUT Vector3 &vMin, OUT Vector3 &vMax);



int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance, 
					 LPSTR lpCmdLine, 
					 int nCmdShow)
{
	wchar_t className[32] = L"Sample";
	wchar_t windowName[32] = L"Sample";

	//윈도우 클레스 정보 생성
	//내가 이러한 윈도를 만들겠다 라는 정보
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.cbWndExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		//윈도우 배경색상
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//윈도우의 커서모양 결정
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//윈도우아이콘모양 결정
	WndClass.hInstance = hInstance;				//프로그램인스턴스핸들 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//윈도우 프로시져 함수 포인터
	WndClass.lpszMenuName = NULL;						//메뉴이름 없으면 NULL
	WndClass.lpszClassName = className;				//지금 작성하고 있는 윈도우 클레스의 이름
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//윈도우 그리기 방식 설정 ( 사이즈가 변경될때 화면갱신 CS_HREDRAW | CS_VREDRAW )

	//위에서 작성한 윈도우 클레스정보 등록
	RegisterClass( &WndClass );

	//윈도우 생성
	//생성된 윈도우 핸들을 전역변수 g_hWnd 가 받는다.
	HWND hWnd = CreateWindow(
		className,				//생성되는 윈도우의 클래스이름
		windowName,				//윈도우 타이틀바에 출력되는 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일 WS_OVERLAPPEDWINDOW
		WINPOS_X,				//윈도우 시작 위치 X 
		WINPOS_Y,				//윈도우 시작 위치 Y
		WINSIZE_X,				//윈도우 가로 크기 ( 작업영역의 크기가 아님 )
		WINSIZE_Y,				//윈도우 세로 크기 ( 작업영역의 크기가 아님 )
		GetDesktopWindow(),		//부모 윈도우 핸들 ( 프로그램에서 최상위 윈도우면 NULL 또는 GetDesktopWindow() )
		NULL,					//메뉴 ID ( 자신의 컨트롤 객체의 윈도우인경우 컨트롤 ID 가 된	
		hInstance,				//이 윈도우가 물릴 프로그램 인스턴스 핸들
		NULL					//추가 정보 NULL ( 신경끄자 )
		);

	g_hWnd = hWnd;

	//윈도우를 정확한 작업영역 크기로 맞춘다
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	ShowWindow( hWnd, nCmdShow );

	Init();


	//메시지 구조체
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	int oldT = GetTickCount();
	while (msg.message != WM_QUIT)
	{
		//PeekMessage 는 메시지 큐에 메시지가 없어도 프로그램이 멈추기 않고 진행이 된다.
		//이때 메시지큐에 메시지가 없으면 false 가 리턴되고 메시지가 있으면 true 가 리턴이된다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage( &msg );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
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
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
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
		switch (wParam)
		{
		case VK_ESCAPE:
			::DestroyWindow(hWnd);
			break;

		case 'D':
		case 'A':
			{
				g_cameraPos.x += (wParam=='D')? 100.f : -100.f;
				Vector3 dir = g_cameraLookat - g_cameraPos;
				dir.Normalize();
				g_matView.SetView(g_cameraPos, dir, Vector3(0,1,0));
			}
			break;

		case 'W':
		case 'S':
			{
				g_cameraPos.z += (wParam=='W')? 100.f : -100.f;
				Vector3 dir = g_cameraLookat - g_cameraPos;
				dir.Normalize();
				g_matView.SetView(g_cameraPos, dir, Vector3(0,1,0));
			}
			break;

		case VK_UP:
		case VK_DOWN:
			{
				Matrix44 mat;
				mat.SetTranslate(Vector3(0, 0, (wParam==VK_UP)? 20.f : -20.f));
				g_matWorld2  *= mat;
			}
			break;

		case VK_LEFT:
		case VK_RIGHT:
			{
				Matrix44 mat;
				mat.SetTranslate(Vector3((wParam==VK_LEFT)? -20.f : 20.f, 0, 0));
				g_matWorld2  *= mat;
			}
			break;
		}
		break;
	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


void Init()
{
	ReadModelFile("../../media/cube_normal.dat", g_vertices, g_indices, g_normals);

	
	Vector3 vMin, vMax;
	GetVerticesMinMax(g_vertices, vMin, vMax);
	g_box1.SetBox(vMin, vMax);
	g_box2.SetBox(vMin, vMax);

	g_matWorld1.SetTranslate(Vector3(-200,0,0));
	g_matWorld2.SetTranslate(Vector3(200,0,0));


	Vector3 dir = g_cameraLookat - g_cameraPos;
	dir.Normalize();
	g_matView.SetView(g_cameraPos, dir, Vector3(0,1,0));
	g_matProjection.SetProjection( MATH_PI / 4.f, 1.0f, 1.0f, 100.0f );

	RECT cr;
	::GetClientRect(g_hWnd, &cr);
	const float width = (float)(cr.right-cr.left);
	const float height = (float)(cr.bottom - cr.top);
	g_matViewPort.SetViewport(width, height);
}


/**
 @brief 
 */
void	MainLoop(int elapse_time)
{
	// collision Test
	g_box1.SetWorldTM(g_matWorld1);
	g_box1.Update();
	g_box2.SetWorldTM(g_matWorld2);
	g_box2.Update();
	g_isCollision = g_box1.Collision(g_box2);


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



bool ReadModelFile( const string &fileName, vector<Vector3> &vertices, vector<int> &indices,
	vector<Vector3> &normals)
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string vtx, eq;
	int numVertices;
	fin >> vtx >> eq >> numVertices;

	if (numVertices <= 0)
		return  false;

	vertices.resize(numVertices);

	float num1, num2, num3;
	for (int i = 0; i < numVertices; i++)
	{
		fin >> num1 >> num2 >> num3;
		vertices[i] = Vector3(num1, num2, num3);
	}

	string idx;
	int numIndices;
	fin >> idx >> eq >> numIndices;

	if (numIndices <= 0)
		return false;

	indices.resize(numIndices*3);

	int num4, num5, num6;
	for (int i = 0; i < numIndices*3; i+=3)
	{
		fin >> num4 >> num5 >> num6;
		indices[ i] = num4;
		indices[ i+1] = num5;
		indices[ i+2] = num6;	
	}


	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;

	normals.resize(numVertices);

	if (numNormal > 0)
	{
		float num1, num2, num3;
		vector<int> vertCount(numVertices, 0);
		for (int i = 0; i < numNormal; i++)
		{
			fin >> num1 >> num2 >> num3;
			Vector3 n(num1, num2, num3);

			// 법선벡터의 평균을 구해서 할당한다.
			for (int k=0; k < 3; ++k)
			{
				const int vtxIdx = indices[ i*3 + k];
				normals[ vtxIdx] += n;
				++vertCount[ vtxIdx];
			}
		}

		for (int i=0; i < numVertices; ++i)
		{
			normals[ i] /= (float)vertCount[ i];
			normals[ i].Normalize();
		}
	}

	return true;
}


// vertices의 최대,최소 정점위치를 리턴한다.
void GetVerticesMinMax( const vector<Vector3> &vertices, OUT Vector3 &vMin, OUT Vector3 &vMax)
{
	vMax = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
	vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	for (int i=0; i < (int)vertices.size(); ++i)
	{
		const Vector3 &v = vertices[i];

		if (vMax.x < v.x)
			vMax.x = v.x;
		if (vMax.y < v.y)
			vMax.y = v.y;
		if (vMax.z < v.z)
			vMax.z = v.z;

		if (vMin.x > v.x)
			vMin.x = v.x;
		if (vMin.y > v.y)
			vMin.y = v.y;
		if (vMin.z > v.z)
			vMin.z = v.z;
	}
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


void RenderIndicesWireFrame(HDC hdc, const vector<Vector3> &vertices, const vector<int> &indices, 
	vector<Vector3> &normals,
	const Matrix44 &tm, const Matrix44 &vpv)
{
	Vector3 camDir = g_cameraLookat - g_cameraPos;
	camDir.Normalize();

	for (unsigned int i=0; i < indices.size(); i+=3)
	{
		Vector3 p1 = vertices[ indices[ i]];
		Vector3 p2 = vertices[ indices[ i+1]];
		Vector3 p3 = vertices[ indices[ i+2]];

		p1 = p1 * tm;
		p2 = p2 * tm;
		p3 = p3 * tm;

		// culling
		Vector3 n1 = normals[ indices[i]].MultiplyNormal( tm );
		Vector3 n2 = normals[ indices[i+1]].MultiplyNormal( tm );
		Vector3 n3 = normals[ indices[i+2]].MultiplyNormal( tm );
		const float dot1 = n1.DotProduct(camDir);
		const float dot2 = n2.DotProduct(camDir);
		const float dot3 = n3.DotProduct(camDir);
		if ((dot1 > 0) && (dot2 > 0) && (dot3 > 0))
			continue;

		p1 = p1 * vpv;
		p2 = p2 * vpv;
		p3 = p3 * vpv;

		Rasterizer::Color color(0,0,255,1);
		Rasterizer::DrawLine(hdc, color, p1.x, p1.y, color, p2.x, p2.y );
		Rasterizer::DrawLine(hdc, color, p2.x, p2.y, color, p3.x, p3.y );
		Rasterizer::DrawLine(hdc, color, p3.x, p3.y, color, p1.x, p1.y );
	}
}


void RenderIndices(HDC hdc, const vector<Vector3> &vertices, const vector<int> &indices, 
	vector<Vector3> &normals,
	const Matrix44 &tm, const Matrix44 &vpv)
{
	Vector3 camDir = g_cameraLookat - g_cameraPos;
	camDir.Normalize();

	for (unsigned int i=0; i < indices.size(); i+=3)
	{
		Vector3 p1 = vertices[ indices[ i]];
		Vector3 p2 = vertices[ indices[ i+1]];
		Vector3 p3 = vertices[ indices[ i+2]];

		p1 = p1 * tm;
		p2 = p2 * tm;
		p3 = p3 * tm;

		// culling
		Vector3 n1 = normals[ indices[i]].MultiplyNormal( tm );
		Vector3 n2 = normals[ indices[i+1]].MultiplyNormal( tm );
		Vector3 n3 = normals[ indices[i+2]].MultiplyNormal( tm );
		const float dot1 = n1.DotProduct(camDir);
		const float dot2 = n2.DotProduct(camDir);
		const float dot3 = n3.DotProduct(camDir);
		if ((dot1 > 0) && (dot2 > 0) && (dot3 > 0))
			continue;

		p1 = p1 * vpv;
		p2 = p2 * vpv;
		p3 = p3 * vpv;

		Rasterizer::Color c0(0,0,255,1);
		Rasterizer::Color c1(255,255,255,1);
		Vector3 lightDir(0,-1,0);
		Vector3 H = -(camDir + lightDir);
		H.Normalize();
		Rasterizer::Color ambient(0,20,0,1);

		Rasterizer::Color color1, color2, color3;
		{
			Rasterizer::Color diffuse = c0 * max(0, n1.DotProduct(-lightDir));
			Rasterizer::Color specular = c1*pow(n1.DotProduct(H), 16);
			color1 = ambient + diffuse + specular;
		}
		{
			Rasterizer::Color diffuse = c0 * max(0, n2.DotProduct(-lightDir));
			Rasterizer::Color specular = c1*pow(n2.DotProduct(H), 16);
			color2 = ambient + diffuse + specular;
		}
		{
			Rasterizer::Color diffuse = c0 * max(0, n3.DotProduct(-lightDir));
			Rasterizer::Color specular = c1*pow(n3.DotProduct(H), 16);
			color3 = ambient + diffuse + specular;
		}

		Rasterizer::DrawTriangle(hdc, 
			color1, p1.x, p1.y,
			color2, p2.x, p2.y,
			color3, p3.x, p3.y);
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

	Matrix44 vpv = g_matView * g_matProjection * g_matViewPort;

	RenderIndicesWireFrame(hdcMem, g_vertices, g_indices, g_normals,
		g_matLocal1 * g_matWorld1, vpv);
	RenderVertices(hdcMem, g_box1.m_box, g_matLocal1 * g_matWorld1 * vpv);


	RenderIndicesWireFrame(hdcMem, g_vertices, g_indices, g_normals,
		g_matLocal2 * g_matWorld2, vpv);
	RenderVertices(hdcMem, g_box1.m_box, g_matLocal2 * g_matWorld2 * vpv);


	if (g_isCollision)
	{
		TextOutA(hdcMem, 10, 10, "Collision", 9);
	}


	BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}
