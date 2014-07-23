
// Shader Code

#include <windows.h>
#include <string>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../math/Math.h"
#include "../../base/base.h"
#include "../../etc/Utility.h"
#include <vector>
#include <map>
#include <sstream>
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "winmm.lib" )


using namespace std;
using namespace graphic;

LPDIRECT3DDEVICE9	g_pDevice = NULL;
const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y

POINT g_CurPos;
bool g_LButtonDown = false;
bool g_RButtonDown = false;
Matrix44 g_LocalTm;
Vector3 g_camPos(0,100,-200);
Vector3 g_lookAtPos(0,0,0);
Matrix44 g_matProj;
Matrix44 g_matView;

graphic::cShader g_shader;
LPD3DXMESH g_mesh;


LPDIRECT3DDEVICE9 graphic::GetDevice()
{
	return g_pDevice;
}


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
bool InitVertexBuffer();
void Render(int timeDelta);
void UpdateCamera();
void GetRay(int sx, int sy, Vector3 &orig, Vector3 &dir);
bool IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v );


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Shader_Normal";
	wchar_t windowName[32] = L"Shader_Normal";

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

	//윈도우를 정확한 작업영역 크기로 맞춘다
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	if (!graphic::InitDirectX(hWnd, WINSIZE_X, WINSIZE_Y, g_pDevice))
	{
		return 0;
	}

	InitVertexBuffer();
	ShowWindow( hWnd, nCmdShow );


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
			const int curT = timeGetTime();
			const int elapseT = curT - oldT;
			//if (elapseT > 15)
			//{
			oldT = curT;
			Render(elapseT);
			//}
		}
	}

	if (g_pDevice)
		g_pDevice->Release();
	return 0;
}


//
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		else if (wParam == VK_TAB)
		{
			static bool flag = false;
			g_pDevice->SetRenderState(D3DRS_CULLMODE, flag);
			g_pDevice->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
			flag = !flag;
		}
		else if (wParam == VK_SPACE)
		{
		}
		break;

	case WM_LBUTTONDOWN:
		{
			g_LButtonDown = true;
			g_CurPos.x = LOWORD(lParam);
			g_CurPos.y = HIWORD(lParam);
		}
		break;

	case WM_RBUTTONDOWN:
		{
			g_RButtonDown = true;
			g_CurPos.x = LOWORD(lParam);
			g_CurPos.y = HIWORD(lParam);
		}
		break;

	case WM_LBUTTONUP:
		g_LButtonDown = false;
		break;

	case WM_RBUTTONUP:
		g_RButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		if (g_LButtonDown)
		{
			POINT pos;
			pos.x = LOWORD(lParam);
			pos.y = HIWORD(lParam);

			const int x = pos.x - g_CurPos.x;
			const int y = pos.y - g_CurPos.y;
			g_CurPos = pos;

			Matrix44 mat1;
			mat1.SetRotationY( -x * 0.01f );
			Matrix44 mat2;
			mat2.SetRotationX( -y * 0.01f );

			g_LocalTm *= (mat1 * mat2);
		}
		if (g_RButtonDown)
		{
			POINT pos;
			pos.x = LOWORD(lParam);
			pos.y = HIWORD(lParam);

			const int x = pos.x - g_CurPos.x;
			const int y = pos.y - g_CurPos.y;
			g_CurPos = pos;

			Matrix44 rx;
			rx.SetRotationY( x * 0.005f );

			Matrix44 ry;
			ry.SetRotationX( y * 0.005f );

			Matrix44 m = rx * ry;
			g_camPos *= m;

			UpdateCamera();
		}	
		else
		{
			g_CurPos.x = LOWORD(lParam);
			g_CurPos.y = HIWORD(lParam);
		}
		break;

	case WM_MOUSEWHEEL:
		{
			const int fwKeys = GET_KEYSTATE_WPARAM(wParam);
			const int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			Vector3 dir = g_lookAtPos - g_camPos;
			dir.Normalize();
			g_camPos += (zDelta < 0)? dir * -50 : dir*50;
			UpdateCamera();
		}
		break;

	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


//랜더
void Render(int timeDelta)
{
	//화면 청소
	if (SUCCEEDED(g_pDevice->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		g_pDevice->BeginScene();

		RenderFPS(timeDelta);
		RenderAxis();


		g_shader.SetVector("vLightDir", Vector3(0,-1,0));
		g_shader.SetMatrix("mWVP", g_LocalTm * g_matView * g_matProj);

		Matrix44 mWIT = g_LocalTm.Inverse();
		mWIT.Transpose();
		g_shader.SetMatrix("mWIT", mWIT);

		g_shader.Begin();
		g_shader.BeginPass(0);
		g_mesh->DrawSubset(0);
		g_shader.EndPass();
		g_shader.End();


		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}
}


bool InitVertexBuffer()
{
	g_shader.Create("hlsl_box_normal.fx", "TShader" );

	D3DXCreateSphere(GetDevice(), 30, 20, 20,&g_mesh, NULL); 


	// 카메라, 투영행렬 생성
	UpdateCamera();

	g_matProj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f) ;
	g_pDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&g_matProj) ;
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}


void UpdateCamera()
{
	Vector3 dir = g_lookAtPos - g_camPos;
	dir.Normalize();
	g_matView.SetView(g_camPos, dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&g_matView);
}
