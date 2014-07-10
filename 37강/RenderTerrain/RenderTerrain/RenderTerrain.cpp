#include <windows.h>
#include <string>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../math/Math.h"
#include "../../base/base.h"
#include <vector>
#include <map>
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )


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

graphic::cVertexBuffer g_VtxBuff;
graphic::cIndexBuffer g_IdxBuff;
graphic::cLight g_Light;


LPDIRECT3DDEVICE9 graphic::GetDevice()
{
	return g_pDevice;
}


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
bool InitDirectX(HWND hWnd);
bool InitVertexBuffer();
void Render(int timeDelta);
void UpdateCamera();
bool Pick(int x, int y);
void GetRay(int sx, int sy, Vector3 &orig, Vector3 &dir);
bool IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v );


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Render Terrain";
	wchar_t windowName[32] = L"Render Terrain";

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

	if (!InitDirectX(hWnd))
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
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			if (elapseT > 15)
			{
				oldT = curT;
				Render(elapseT);
			}
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
			//g_LButtonDown = true;
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

			Matrix44 mat1;
			mat1.SetRotationY( -x * 0.01f );
			Matrix44 mat2;
			mat2.SetRotationX( -y * 0.01f );

			//g_LocalTm *= (mat1 * mat2);
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
		break;

	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


bool InitDirectX(HWND hWnd)
{
	LPDIRECT3D9 d3d9;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );

	// 하드웨어 정보를 가지고 와서 자신의 정점 프로세스 타입을 정하자
	D3DCAPS9 caps;

	//Direct3D9 객체 통해 비디오 카드의 하드웨어 정보를 가지고 온다.
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,			//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		&caps						//디바이스 정보를 받아올 D3DCAPS9 포인터
		);

	//정점계산 처리방식을 지정할 플레그 값
	int vertexProcessing = 0;

	//정점 위치와 광원 계산시 하드웨어 사용이 가능한가
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	//3. D3DPRESENT_PARAMETERS 구조체 정보를 생성
	//내가 이러한 Device 를 만들겟다라는 정보

	D3DPRESENT_PARAMETERS d3dpp;		
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
	d3dpp.BackBufferCount = 1;						//백버퍼 갯수 그냥 1개
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//백버퍼 색상 포맷 알파8비트,레드8비트,그린8비트,블루8비트
	d3dpp.BackBufferHeight = WINSIZE_Y;				//백버퍼 픽셀 크기
	d3dpp.BackBufferWidth = WINSIZE_X;				//백버퍼 픽셀 크기
	d3dpp.EnableAutoDepthStencil = true;						//자동 깊이버퍼 사용 여부 ( 그냥 true )
	d3dpp.Flags = 0;						//추기 플래그 ( 일단 0 )
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	//화면 주사율 ( 그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일시 )
	d3dpp.hDeviceWindow = hWnd;					//Device 가 출력될 윈도우 핸들
	d3dpp.MultiSampleQuality = 0;						//멀티 샘플링 질
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//멀티 샘플링 타입 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//화면 스왑 체인 방식
	d3dpp.Windowed = true;						//윈도우 모드냐? ( 이게 false 면 풀스크린 된다! )

	//4. Device 생성
	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,					//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,						//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		hWnd,								//디바이스를 사용할 윈도우 핸들
		vertexProcessing,					//정점 처리 방식에 대한 플레그
		&d3dpp,								//앞에서 정의한 D3DPRESENT_PARAMETERS 구조체 포인터
		&g_pDevice							//얻어올 디바이스 더블포인터
		)))
	{
		//디바이스 생성 실패
		d3d9->Release(); // Deivce 를 만들기 위해 생성된 Direct3D9 객체를 해제
		d3d9 = NULL;

		MessageBoxA( hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK );
		return false;
	}


	//디바이스 생성 성공
	d3d9->Release(); // Deivce 를 만들었으니 넌 더이상 필요없다 ( 사라져라! )
	d3d9 = NULL;
	return true;
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

		Matrix44 tm = g_LocalTm;
		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&tm);

		
		g_VtxBuff.Bind();
		g_IdxBuff.Bind();
		g_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_VtxBuff.GetVertexCount(), 
			0, g_IdxBuff.GetFaceCount());


		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}
}


bool InitVertexBuffer()
{
	// Init Grid
	const int rowVtxCnt  = 65;
	const int colVtxCnt  = 65;
	const float cellSize = 50.f;
	const int rowCellCnt = rowVtxCnt-1;
	const int colCellCnt = colVtxCnt-1;
	const int cellCnt = rowCellCnt * colCellCnt;
	const int vtxCount= rowVtxCnt * colVtxCnt;

	g_VtxBuff.Create( vtxCount, sizeof(sVertexNormDiffuse), sVertexNormDiffuse::FVF);
	{
		sVertexNormDiffuse *vertices = (sVertexNormDiffuse*)g_VtxBuff.Lock();
		const float startx = -cellSize*(rowCellCnt/2);
		const float starty = cellSize*(colCellCnt/2);
		const float endx = startx + cellSize*rowCellCnt;
		const float endy = starty - cellSize*colCellCnt;

		const float uCoordIncrementSize = 1.0f / (float)colCellCnt;
		const float vCoordIncrementSize = 1.0f / (float)rowCellCnt;

		int i=0;
		for (float y=starty; y >= endy; y -= cellSize, ++i)
		{
			int k=0;
			for (float x=startx; x <= endx; x += cellSize, ++k )
			{
				int index = (i * colVtxCnt) + k;
				vertices[ index].p = Vector3(x, 0.1f, y);
				vertices[ index].n = Vector3(0,1,0);
				vertices[ index].c = 0xffffffff;
			}
		}
		g_VtxBuff.Unlock();
	}	


	g_IdxBuff.Create( cellCnt*2 );
	{
		WORD *indices = (WORD*)g_IdxBuff.Lock();
		int baseIndex = 0;
		for( int i=0; i < rowCellCnt; ++i )
		{
			for( int k=0; k < colCellCnt; ++k )
			{
				indices[ baseIndex] = (i * colVtxCnt) + k;
				indices[ baseIndex + 1] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 2] = ((i+1) * colVtxCnt) + k;

				indices[ baseIndex + 3] = ((i+1) * colVtxCnt) + k;
				indices[ baseIndex + 4] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 5] = ((i+1) * colVtxCnt) + k + 1;

				// next quad
				baseIndex += 6;
			}
		}	
		g_IdxBuff.Unlock();
	}

	
	// 빛 생성.
	Vector4 color(1,1,1,1);
	g_Light.Init(graphic::cLight::LIGHT_DIRECTIONAL, 
		color*0.4f, color, color*0.6f, Vector3(0,-1,0));

	// 카메라, 투영행렬 생성
	UpdateCamera();

	g_matProj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	g_pDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&g_matProj) ;

	g_Light.Bind(0);
	g_pDevice->LightEnable(
		0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
		true); // true = 활성화 ， false = 비활성화

	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

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

	sVertexNormDiffuse *vertices = (sVertexNormDiffuse*)g_VtxBuff.Lock();
	WORD *indices = (WORD*)g_IdxBuff.Lock();

	const int size = g_IdxBuff.GetFaceCount()*3;
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
			vertices[ indices[ i+0]].c = 0xffff0000;
			vertices[ indices[ i+1]].c = 0xffff0000;
			vertices[ indices[ i+2]].c = 0xffff0000;
		}
	}

	g_VtxBuff.Unlock();
	g_IdxBuff.Unlock();
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
	FLOAT* t, FLOAT* u, FLOAT* v )
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

