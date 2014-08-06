
// Dynamic Alpha blending

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

graphic::cLine g_line( D3DCOLOR_XRGB(0,255,0) );
graphic::cGrid2 g_gridAlpha;
graphic::cGrid2 g_grid;
const int TEXTURE_SIZE = 200;
const float CELL_SIZE = 200.f;
const int COL_CELL_COUNT = 16;
const int ROW_CELL_COUNT = 16;



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
bool Pick(int x, int y);
void GetRay(int sx, int sy, Vector3 &orig, Vector3 &dir);
bool IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	OUT FLOAT* t, OUT FLOAT* u, OUT FLOAT* v );
void Brush(const float u, const float v);
void GetTextureUV(const Vector3 &pos, OUT float &u, OUT float &v);


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Dynamic Alpha";
	wchar_t windowName[32] = L"Dynamic Alpha";

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
			oldT = curT;
			Render(elapseT);
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
			Pick(g_CurPos.x, g_CurPos.y);
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

			//Matrix44 mat1;
			//mat1.SetRotationY( -x * 0.01f );
			//Matrix44 mat2;
			//mat2.SetRotationX( -y * 0.01f );
			//g_LocalTm *= (mat1 * mat2);
			Pick(pos.x, pos.y);
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

		g_pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


		// 첫 번째 이미지를 알파이미지로, 두 번째 이미지를 원본 이미지로 쓴다.
		g_pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
		g_pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );

		g_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		g_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

		g_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		g_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTA_CURRENT);


		Matrix44 tm = g_LocalTm;
		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&tm);

		g_gridAlpha.Render();
		g_grid.Render(1);

		g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );


		RenderAxis();
		g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


		{ // Render Line
			Vector3 orig, dir;
			GetRay( g_CurPos.x, g_CurPos.y, orig, dir );
			Vector3 p0 = orig + Vector3(10,10,0);
			Vector3 p1 = orig + dir * 200.f;
			g_line.SetLine( p0, p1, 1 );
			g_line.Render();
		}


		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}
}


bool InitVertexBuffer()
{
	g_gridAlpha.Create(ROW_CELL_COUNT, COL_CELL_COUNT, CELL_SIZE, 1.f);
	g_gridAlpha.GetTexture().Create(TEXTURE_SIZE, TEXTURE_SIZE, D3DFMT_A8R8G8B8);
	g_grid.Create(ROW_CELL_COUNT, COL_CELL_COUNT, CELL_SIZE, 1.f);
	g_grid.GetTexture().Create( "../../media/grass_spring1.jpg");


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


bool Pick(int x, int y)
{
	Vector3 orig, dir;
	GetRay( x, y, orig, dir );

	sVertexNormTex *vertices = (sVertexNormTex*)g_gridAlpha.GetVertexBuffer().Lock();
	WORD *indices = (WORD*)g_gridAlpha.GetIndexBuffer().Lock();

	const int size = g_gridAlpha.GetIndexBuffer().GetFaceCount()*3;
	for( int i=0; i < size; i+=3 )
	{
		const Vector3 v1 = vertices[ indices[ i+0]].p;
		const Vector3 v2 = vertices[ indices[ i+1]].p;
		const Vector3 v3 = vertices[ indices[ i+2]].p;

		float t, u, v;
		if (IntersectTriangle(
			*(D3DXVECTOR3*)&orig,
			*(D3DXVECTOR3*)&dir,
			*(D3DXVECTOR3*)&v1,
			*(D3DXVECTOR3*)&v2,
			*(D3DXVECTOR3*)&v3,
			&t, &u, &v))
		{
			Vector3 pos = orig + dir*t;
			float tu, tv;
			GetTextureUV(pos, tu, tv);
			Brush(tu, tv);
		}
	}

	g_gridAlpha.GetVertexBuffer().Unlock();
	g_gridAlpha.GetIndexBuffer().Unlock();
	return true;
}


void GetRay(int sx, int sy, Vector3 &orig, Vector3 &dir)
{
	const float x =  ( (sx * 2.0F / WINSIZE_X ) - 1.0F );
	const float y = -( (sy * 2.0F / WINSIZE_Y) - 1.0F );

	Vector3 v;
	v.x = x / g_matProj._11;
	v.y = y / g_matProj._22;
	v.z =  1.0F;

	Matrix44 m = g_matView.Inverse();

	dir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	dir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	dir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;

	orig.x = m._41;
	orig.y = m._42;
	orig.z = m._43;
}


bool IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	OUT FLOAT* t, OUT FLOAT* u, OUT FLOAT* v )
{
	// Find vectors for two edges sharing vert0
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	D3DXVECTOR3 pvec;
	D3DXVec3Cross( &pvec, &dir, &edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot( &edge1, &pvec );

	D3DXVECTOR3 tvec;
	if( det > 0 )
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if( det < 0.0001f )
		return FALSE;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot( &tvec, &pvec );
	if( *u < 0.0f || *u > det )
		return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross( &qvec, &tvec, &edge1 );

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot( &dir, &qvec );
	if( *v < 0.0f || *u + *v > det )
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot( &edge2, &qvec );
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return TRUE;
}


void Brush(const float u, const float v)
{
	D3DLOCKED_RECT lockrect;
	g_gridAlpha.GetTexture().Lock(lockrect);

	const float innerRadius = 10;
	const float outterRadius = 200;
	const float innerAlpha = 1.f;

	BYTE *pbits = (BYTE*)lockrect.pBits;
	for (int ay=0; ay < TEXTURE_SIZE; ++ay)
	{
		for (int ax=0; ax < TEXTURE_SIZE; ++ax)
		{
			float au = (float)ax / (float)TEXTURE_SIZE;
			float av = (float)ay / (float)TEXTURE_SIZE;

			float len = sqrt( ((au-u)*(au-u)) + ((av-v)*(av-v)) );
			len *= (COL_CELL_COUNT * CELL_SIZE);

			// A8R8G8B8 Format
			DWORD *ppixel = (DWORD*)(pbits + (ax*4) + (lockrect.Pitch * ay));

			if (len <= innerRadius)
			{
				int color = (int)(255.f * innerAlpha);
				*ppixel = D3DCOLOR_ARGB(color, 0, 0, 0);
			}
			else if (len <= outterRadius)
			{
				// 보간
				const float width = outterRadius - innerRadius;
				const float delta = 1.f - ((len - innerRadius) / width);
				const int color = (int)(innerAlpha* delta * 255.f);

				const int dest = *ppixel >> 24;
				if (color > dest)
					*ppixel = D3DCOLOR_ARGB(color, 0, 0, 0);
			}
		}
	}

	g_gridAlpha.GetTexture().Unlock();
}


void GetTextureUV(const Vector3 &pos, OUT float &u, OUT float &v)
{
	const float width = COL_CELL_COUNT * CELL_SIZE;
	const float height = ROW_CELL_COUNT * CELL_SIZE;

	float x = pos.x + width/2.f;
	float z = pos.z + height/2.f;
	z = height - z;

	u = x / (float)width;
	v = z / (float)height;
}
