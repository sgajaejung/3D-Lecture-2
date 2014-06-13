#include <windows.h>
#include <string>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../math/Math.h"
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#include <vector>
#include <map>

#include "../../wxMemMonitorLib/wxMemMonitor.h"
MEMORYMONITOR_INNER_PROCESS();

using namespace std;

LPDIRECT3DDEVICE9	g_pDevice = NULL;
const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y


DECLARE_TYPE_NAME(sGlobal)
struct sGlobal : public memmonitor::Monitor<sGlobal, TYPE_NAME(sGlobal)>
{
	// 모델 출력.
	LPDIRECT3DVERTEXBUFFER9 vb; // 버텍스 버퍼
	LPDIRECT3DINDEXBUFFER9 ib; // 인덱스 버퍼
	int vtxSize;
	int faceSize;
	D3DMATERIAL9 mtrl;
	D3DLIGHT9 light;
	IDirect3DTexture9 *texture;
	int addressMode;

	// 법선 출력
	LPDIRECT3DVERTEXBUFFER9 fnvb; // 면 법선 버퍼
	int faceNormalVtxSize;
	LPDIRECT3DVERTEXBUFFER9 vnvb; // 면 법선 버퍼
	int vertexNormalVtxSize;

	// 회전
	POINT curPos;
	bool lButtonDown;
	Matrix44 localTm;

	enum RENDER_MODE 
	{ 
		RENDER_MESH, 
		RENDER_FACE_NORMAL, 
		RENDER_VERTEX_NORMAL 
	};
	RENDER_MODE mode;
};
sGlobal *global;


// 버텍스 구조체
struct Vertex
{
	Vertex() {}
	Vertex(float x0, float y0, float z0) : p(Vector3(x0, y0, z0)), n(Vector3(0,0,0)) {}
	Vector3 p;
	Vector3 n;
	float u, v;
	static const DWORD FVF;
};
//버텍스 구조체 포맷.
const DWORD Vertex::FVF  = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
bool InitDirectX(HWND hWnd);
bool InitVertexBuffer();
void Render(int timeDelta);
bool ReadModelFile( const string &fileName, LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize,  LPDIRECT3DINDEXBUFFER9 &idxBuff, int &faceSize );
void ComputeNormals(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int vtxSize,  LPDIRECT3DINDEXBUFFER9 idxBuff, int faceSize);
bool CreateFaceNormalLine( 
	LPDIRECT3DVERTEXBUFFER9 srcVtxBuff, int srcVtxSize,  LPDIRECT3DINDEXBUFFER9 srcIdxBuff, int srcFaceSize,
	LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize );
bool CreateVertexNormalLine( 
	LPDIRECT3DVERTEXBUFFER9 srcVtxBuff, int srcVtxSize,  LPDIRECT3DINDEXBUFFER9 srcIdxBuff, int srcFaceSize,
	LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize );



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

	if (!memmonitor::Init(memmonitor::INNER_PROCESS,hInstance,"config_memmonitor.json" ))
	{
		MessageBoxA(NULL, memmonitor::GetLastError().c_str(), "ERROR", MB_OK);
	}

	global = new sGlobal;
	global->lButtonDown = false;
	global->mode = sGlobal::RENDER_MESH;
	global->addressMode = D3DTADDRESS_WRAP;

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
	if (global->vb)
		global->vb->Release();
	if (global->ib)
		global->ib->Release();
	if (global->fnvb)
		global->fnvb->Release();
	if (global->texture)
		global->texture->Release();
	delete global;
	memmonitor::Cleanup();
	return 0;
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
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
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


bool InitVertexBuffer()
{
	ReadModelFile("../../media/teapot.dat", global->vb, global->vtxSize, global->ib, global->faceSize);
	CreateFaceNormalLine(global->vb, global->vtxSize, global->ib, global->faceSize, global->fnvb, global->faceNormalVtxSize);
	CreateVertexNormalLine(global->vb, global->vtxSize, global->ib, global->faceSize, global->vnvb, global->vertexNormalVtxSize);
	D3DXCreateTextureFromFileA(g_pDevice, "../../media/강소라2.jpg", &global->texture);


	ZeroMemory(&global->mtrl, sizeof(global->mtrl));
	global->mtrl.Ambient = D3DXCOLOR(1,1,1,1);
	global->mtrl.Diffuse = D3DXCOLOR(1,1,1,1);
	global->mtrl.Specular = D3DXCOLOR(1,1,1,1);
	global->mtrl.Emissive = D3DXCOLOR(0,0,0,1);
	global->mtrl.Power = 32.f;

	D3DXCOLOR color(1,1,1,1);
	ZeroMemory(&global->light, sizeof(global->light));
	global->light.Type = D3DLIGHT_DIRECTIONAL;
	global->light.Ambient = color * 0.4f;
	global->light.Diffuse = color;
	global->light.Specular = color * 0.6f;
	global->light.Direction = *(D3DXVECTOR3*)&Vector3(0,0,1);
	g_pDevice->SetLight(0, &global->light); // 광원 설정.


	Matrix44 V;
	Vector3 dir = Vector3(0,0,0)-Vector3(0,0,-500);
	dir.Normalize();
	V.SetView(Vector3(0,0,-500), dir, Vector3(0,1,0));
	g_pDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);

	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	g_pDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

	g_pDevice->LightEnable(0, true);

	//g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	//g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true) ;

	return true;
}


//
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::DestroyWindow(hWnd);
			break;

		case VK_TAB:
			{
				static bool flag = false;
				g_pDevice->SetRenderState(D3DRS_CULLMODE, flag? D3DCULL_CCW : D3DCULL_NONE);
				g_pDevice->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
				flag = !flag;
			}
			break;

		case '1':
			global->mode = sGlobal::RENDER_MESH;
			break;

		case '2':
			global->mode = sGlobal::RENDER_FACE_NORMAL;
			break;

		case '3':
			global->mode = sGlobal::RENDER_VERTEX_NORMAL;
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			global->lButtonDown = true;
			global->curPos.x = LOWORD(lParam);
			global->curPos.y = HIWORD(lParam);
		}
		break;

	case WM_LBUTTONUP:
		global->lButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		if (global->lButtonDown)
		{
			POINT pos = {LOWORD(lParam), HIWORD(lParam)};
			const int x = pos.x - global->curPos.x;
			const int y = pos.y - global->curPos.y;
			global->curPos = pos;

			Matrix44 mat1;
			mat1.SetRotationY( -x * 0.01f );
			Matrix44 mat2;
			mat2.SetRotationX( -y * 0.01f );

			global->localTm *= (mat1 * mat2);
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

		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&global->localTm);

		g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, global->addressMode) ;
		g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, global->addressMode) ;

		g_pDevice->SetMaterial(&global->mtrl);

		g_pDevice->SetTexture(0, global->texture);
		g_pDevice->SetStreamSource( 0, global->vb, 0, sizeof(Vertex) );
		g_pDevice->SetIndices(global->ib);
		g_pDevice->SetFVF( Vertex::FVF );
		g_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, global->vtxSize, 0, global->faceSize);

		switch (global->mode)
		{
		case sGlobal::RENDER_FACE_NORMAL:
			g_pDevice->SetStreamSource( 0, global->fnvb, 0, sizeof(Vertex) );
			g_pDevice->DrawPrimitive( D3DPT_LINELIST, 0, global->faceNormalVtxSize/2);
			break;
		case sGlobal::RENDER_VERTEX_NORMAL:
			g_pDevice->SetStreamSource( 0, global->vnvb, 0, sizeof(Vertex) );
			g_pDevice->DrawPrimitive( D3DPT_LINELIST, 0, global->vertexNormalVtxSize/2);
			break;
		}

		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}
}


bool ReadModelFile( const string &fileName, LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize,  
	LPDIRECT3DINDEXBUFFER9 &idxBuff, int &faceSize )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >>exporterVersion;
	if (exporterVersion != "EXPORTER_V1")
		return false;

	string vtx, eq;
	fin >> vtx >> eq >> vtxSize;

	if (vtxSize <= 0)
		return  false;

	vector<Vertex> tempVtxBuff;
	tempVtxBuff.reserve(vtxSize + vtxSize/2);

	float num1, num2, num3;
	for (int i = 0; i < vtxSize; i++)
	{
		fin >> num1 >> num2 >> num3;
		tempVtxBuff.push_back( Vertex(num1, num2, num3) );
	}


	// 인덱스 버퍼 초기화.
	string idx;
	fin >> idx >> eq >> faceSize;

	vector<int> tempIdxBuff;
	tempIdxBuff.reserve(faceSize);

	if (faceSize > 0)
	{
		int num1, num2, num3;
		for (int i = 0; i < faceSize*3; i+=3)
		{
			fin >> num1 >> num2 >> num3;
			tempIdxBuff.push_back(num1);
			tempIdxBuff.push_back(num2);
			tempIdxBuff.push_back(num3);
		}
	}

	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;

	if (numNormal > 0)
	{
		float num1, num2, num3;
		vector<int> vertCount(vtxSize, 0);
		for (int i = 0; i < numNormal; i++)
		{
			fin >> num1 >> num2 >> num3;
			Vector3 n(num1, num2, num3);

			// 법선벡터의 평균을 구해서 할당한다.
			for (int k=0; k < 3; ++k)
			{
				const int vtxIdx = tempIdxBuff[ i*3 + k];
				tempVtxBuff[ vtxIdx].n += n;
				++vertCount[ vtxIdx];
			}
		}

		for (int i=0; i < vtxSize; ++i)
		{
			tempVtxBuff[ i].n /= (float)vertCount[ i];
			tempVtxBuff[ i].n.Normalize();
		}
	}


	string tex;
	int numTex;
	fin >> tex >> eq >> numTex;

	if (numTex > 0)
	{
		float fnum1, fnum2;
		vector<Vector3> texVertices(numTex);
		for (int i = 0; i < numTex; i++)
		{
			fin >> fnum1 >> fnum2;
			texVertices[ i] = Vector3(fnum1, fnum2, 0);
		}

		string strTexFace;
		int numTexFace;
		fin >> strTexFace >> eq >> numTexFace;

		vector<int> texFaces;
		texFaces.reserve(numTexFace*3);
		if (numTexFace > 0)
		{
			int num1, num2, num3;
			for (int i=0; i < numTexFace; ++i)
			{
				fin >> num1 >> num2 >> num3;
				texFaces.push_back( num1 );
				texFaces.push_back( num2 );
				texFaces.push_back( num3 );
			}
		}

		map<int, vector<int> > vtxIdxMap; // vertex index, vertex index array
		for (int i=0; i < vtxSize; ++i)
		{
			vector<int> varray;
			varray.reserve(4);
			varray.push_back(i);
			vtxIdxMap[ i] = varray;
		}

		// 텍스쳐 좌표를 버텍스 버퍼에 저장한다. 
		// 버텍스 버퍼의 uv 값이 초기화 되지 않았다면, 초기화 한다.
		// 버텍스에 하나 이상의 uv값이 존재한다면, 버텍스를 추가하고, 인덱스버퍼를 수정한다.
		for (int i=0; i < (int)texFaces.size(); ++i)
		{
			const Vector3 tex = texVertices[ texFaces[ i]];
			const int vtxIdx = tempIdxBuff[ i];

			bool isFind = false;
			for (int k=0; k < (int)vtxIdxMap[ vtxIdx].size(); ++k)
			{
				const int subVtxIdx = vtxIdxMap[ vtxIdx][ k];

				// 텍스쳐 좌표가 버텍스 버퍼에 저장되어 있다면, index buffer 값을 해당 vertex index 로
				// 설정 한다.
				if ((-100 == tempVtxBuff[ subVtxIdx].u) &&
					(-100 == tempVtxBuff[ subVtxIdx].v))
				{
					tempVtxBuff[ subVtxIdx].u = tex.x;
					tempVtxBuff[ subVtxIdx].v = tex.y;
					isFind = true;
					break;
				}
				else if ((tex.x == tempVtxBuff[ subVtxIdx].u) && 
					(tex.y == tempVtxBuff[ subVtxIdx].v))
				{
					tempIdxBuff[ i] = subVtxIdx;
					isFind = true;
					break;
				}
			}

			// 버텍스 버퍼에 없는 uv 좌표라면, 새 버텍스를 버텍스버퍼에 추가한다.
			// 인덱스 버퍼에도 새로 추가된 버텍스 인덱스값을 넣는다.
			if (!isFind)
			{
				Vertex v = tempVtxBuff[ vtxIdx];
				v.u = tex.x;
				v.v = tex.y;
				tempVtxBuff.push_back(v);
				const int newVtxIdx = tempVtxBuff.size()-1;
				vtxIdxMap[ vtxIdx].push_back( newVtxIdx );
				tempIdxBuff[ i] = newVtxIdx;
			}
		}
	}


	// 버텍스 버퍼 생성.
	if (FAILED(g_pDevice->CreateVertexBuffer( tempVtxBuff.size() * sizeof(Vertex),
		D3DUSAGE_WRITEONLY, Vertex::FVF,
		D3DPOOL_MANAGED, &vtxBuff, NULL)))
	{
		return false;
	}
	// 버텍스 버퍼 초기화.
	Vertex* vertices;
	if (FAILED(vtxBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0)))
		return false;
	for (int i = 0; i < (int)tempVtxBuff.size(); i++)
		vertices[ i] = tempVtxBuff[ i];
	vtxBuff->Unlock();

	// 인덱스 버퍼 생성.
	if (FAILED(g_pDevice->CreateIndexBuffer(tempIdxBuff.size()*sizeof(WORD), 
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&idxBuff, NULL)))
	{
		return false;
	}
	WORD *indices = NULL;
	idxBuff->Lock(0, 0, (void**)&indices, 0);
	for (int i = 0; i < (int)tempIdxBuff.size(); ++i)
		indices[ i] = tempIdxBuff[ i];
	idxBuff->Unlock();

	ComputeNormals(vtxBuff, vtxSize, idxBuff, faceSize);
	return true;
}


void ComputeNormals(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int vtxSize,  LPDIRECT3DINDEXBUFFER9 idxBuff, int faceSize)
{
	Vertex* vertices;
	vtxBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0);
	WORD *indices = NULL;
	idxBuff->Lock(0, 0, (void**)&indices, 0);

	for (int i=0; i < faceSize*3; i+=3)
	{
		Vector3 p1 = vertices[ indices[ i]].p;
		Vector3 p2 = vertices[ indices[ i+1]].p;
		Vector3 p3 = vertices[ indices[ i+2]].p;

		Vector3 v1 = p2 - p1;
		Vector3 v2 = p3 - p1;
		v1.Normalize();
		v2.Normalize();
		Vector3 n = v1.CrossProduct(v2);
		n.Normalize();

		vertices[ indices[ i]].n += n;
		vertices[ indices[ i+1]].n += n;
		vertices[ indices[ i+2]].n += n;
	}

	for (int i=0; i < vtxSize; ++i)
		vertices[ i].n.Normalize();

	vtxBuff->Unlock();
	idxBuff->Unlock();
}


bool CreateFaceNormalLine( 
	LPDIRECT3DVERTEXBUFFER9 srcVtxBuff, int srcVtxSize,  LPDIRECT3DINDEXBUFFER9 srcIdxBuff, int srcFaceSize,
	LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize )
{
	// 버텍스 버퍼 생성.
	if (FAILED(g_pDevice->CreateVertexBuffer( srcFaceSize * 2 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY, Vertex::FVF,
		D3DPOOL_MANAGED, &vtxBuff, NULL)))
	{
		return false;
	}

	vtxSize =  srcFaceSize * 2;

	Vertex* srcVertices;
	srcVtxBuff->Lock( 0, sizeof(Vertex), (void**)&srcVertices, 0);
	WORD *srcIndices = NULL;
	srcIdxBuff->Lock(0, 0, (void**)&srcIndices, 0);

	Vertex* vertices;
	vtxBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0);

	int cnt = 0;
	for (int i=0; i < srcFaceSize*3; i+=3)
	{
		Vector3 p1 = srcVertices[ srcIndices[ i]].p;
		Vector3 p2 = srcVertices[ srcIndices[ i+1]].p;
		Vector3 p3 = srcVertices[ srcIndices[ i+2]].p;

		Vector3 v1 = p2 - p1;
		Vector3 v2 = p3 - p1;
		v1.Normalize();
		v2.Normalize();
		Vector3 n = v1.CrossProduct(v2);
		n.Normalize();

		Vector3 pos = p1 + p2 + p3;
		pos /= 3.f;
		vertices[ cnt++].p = pos;
		vertices[ cnt++].p = pos + n * 100.f;
	}

	vtxBuff->Unlock();
	srcVtxBuff->Unlock();
	srcIdxBuff->Unlock();
	return true;
}


bool CreateVertexNormalLine( 
	LPDIRECT3DVERTEXBUFFER9 srcVtxBuff, int srcVtxSize,  LPDIRECT3DINDEXBUFFER9 srcIdxBuff, int srcFaceSize,
	LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize )
{
	// 버텍스 버퍼 생성.
	if (FAILED(g_pDevice->CreateVertexBuffer( srcVtxSize * 2 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY, Vertex::FVF,
		D3DPOOL_MANAGED, &vtxBuff, NULL)))
	{
		return false;
	}

	vtxSize =  srcVtxSize * 2;

	Vertex* srcVertices;
	srcVtxBuff->Lock( 0, sizeof(Vertex), (void**)&srcVertices, 0);
	WORD *srcIndices = NULL;
	srcIdxBuff->Lock(0, 0, (void**)&srcIndices, 0);

	Vertex* vertices;
	vtxBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0);

	int cnt = 0;
	for (int i=0; i < srcVtxSize; i++)
	{
		Vector3 p = srcVertices[ i].p;
		Vector3 n = srcVertices[ i].n;
		vertices[ cnt++].p = p;
		vertices[ cnt++].p = p + n * 100.f;
	}

	vtxBuff->Unlock();
	srcVtxBuff->Unlock();
	srcIdxBuff->Unlock();
	return true;
}
