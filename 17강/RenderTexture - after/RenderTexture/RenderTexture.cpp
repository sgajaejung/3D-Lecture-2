#include <windows.h>
#include <string>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../math/Math.h"
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

#include "../../wxMemMonitorLib/wxMemMonitor.h"
MEMORYMONITOR_INNER_PROCESS();

using namespace std;

LPDIRECT3DDEVICE9	g_pDevice = NULL;
const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
const int WINPOS_X = 0; //�ʱ� ������ ���� ��ġ X
const int WINPOS_Y = 0; //�ʱ� ������ ���� ��ġ Y


DECLARE_TYPE_NAME(sGlobal)
struct sGlobal : public memmonitor::Monitor<sGlobal, TYPE_NAME(sGlobal)>
{
	// �� ���.
	LPDIRECT3DVERTEXBUFFER9 vb; // ���ؽ� ����
	LPDIRECT3DINDEXBUFFER9 ib; // �ε��� ����
	int vtxSize;
	int faceSize;
	D3DMATERIAL9 mtrl;
	D3DLIGHT9 light;
	IDirect3DTexture9 *texture;
	int addressMode;

	// ���� ���
	LPDIRECT3DVERTEXBUFFER9 fnvb; // �� ���� ����
	int faceNormalVtxSize;
	LPDIRECT3DVERTEXBUFFER9 vnvb; // �� ���� ����
	int vertexNormalVtxSize;

	// ȸ��
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


// ���ؽ� ����ü
struct Vertex
{
	Vertex() {}
	Vertex(float x0, float y0, float z0) : p(Vector3(x0, y0, z0)), n(Vector3(0,0,0)) {}
	Vector3 p;
	Vector3 n;
	float u, v;
	static const DWORD FVF;
};
//���ؽ� ����ü ����.
const DWORD Vertex::FVF  = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



// �ݹ� ���ν��� �Լ� ������ Ÿ��
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

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
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

	// �ϵ���� ������ ������ �ͼ� �ڽ��� ���� ���μ��� Ÿ���� ������
	D3DCAPS9 caps;

	//Direct3D9 ��ü ���� ���� ī���� �ϵ���� ������ ������ �´�.
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,			//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
		&caps						//����̽� ������ �޾ƿ� D3DCAPS9 ������
		);

	//������� ó������� ������ �÷��� ��
	int vertexProcessing = 0;

	//���� ��ġ�� ���� ���� �ϵ���� ����� �����Ѱ�
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	//3. D3DPRESENT_PARAMETERS ����ü ������ ����
	//���� �̷��� Device �� ����ٴٶ�� ����

	D3DPRESENT_PARAMETERS d3dpp;		
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//���� ���ۿ� ���׽� ���� ũ�� ���� 24bit ���ٽ� ���� 8 ��Ʈ
	d3dpp.BackBufferCount = 1;						//����� ���� �׳� 1��
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//����� ���� ���� ����8��Ʈ,����8��Ʈ,�׸�8��Ʈ,���8��Ʈ
	d3dpp.BackBufferHeight = WINSIZE_Y;				//����� �ȼ� ũ��
	d3dpp.BackBufferWidth = WINSIZE_X;				//����� �ȼ� ũ��
	d3dpp.EnableAutoDepthStencil = true;						//�ڵ� ���̹��� ��� ���� ( �׳� true )
	d3dpp.Flags = 0;						//�߱� �÷��� ( �ϴ� 0 )
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	//ȭ�� �ֻ��� ( �׳� D3DPRESENT_RATE_DEFAULT ����� �ֻ����� ���Ͻ� )
	d3dpp.hDeviceWindow = hWnd;					//Device �� ��µ� ������ �ڵ�
	d3dpp.MultiSampleQuality = 0;						//��Ƽ ���ø� ��
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//��Ƽ ���ø� Ÿ�� 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//ȭ�� ���� ���� ( �׳� D3DPRESENT_INTERVAL_ONE ����� �ֻ����� ���Ͻ� )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ȭ�� ���� ü�� ���
	d3dpp.Windowed = true;						//������ ����? ( �̰� false �� Ǯ��ũ�� �ȴ�! )

	//4. Device ����
	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,					//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,						//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
		hWnd,								//����̽��� ����� ������ �ڵ�
		vertexProcessing,					//���� ó�� ��Ŀ� ���� �÷���
		&d3dpp,								//�տ��� ������ D3DPRESENT_PARAMETERS ����ü ������
		&g_pDevice							//���� ����̽� ����������
		)))
	{
		//����̽� ���� ����
		d3d9->Release(); // Deivce �� ����� ���� ������ Direct3D9 ��ü�� ����
		d3d9 = NULL;

		MessageBoxA( hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK );
		return false;
	}

	//����̽� ���� ����
	d3d9->Release(); // Deivce �� ��������� �� ���̻� �ʿ���� ( �������! )
	d3d9 = NULL;
	return true;
}


bool InitVertexBuffer()
{
	ReadModelFile("../../media/cube1.dat", global->vb, global->vtxSize, global->ib, global->faceSize);
	CreateFaceNormalLine(global->vb, global->vtxSize, global->ib, global->faceSize, global->fnvb, global->faceNormalVtxSize);
	CreateVertexNormalLine(global->vb, global->vtxSize, global->ib, global->faceSize, global->vnvb, global->vertexNormalVtxSize);
	D3DXCreateTextureFromFileA(g_pDevice, "../../media/���Ҷ�2.jpg", &global->texture);


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
	g_pDevice->SetLight(0, &global->light); // ���� ����.


	Matrix44 V;
	Vector3 dir = Vector3(0,0,0)-Vector3(0,0,-300);
	dir.Normalize();
	V.SetView(Vector3(0,0,-300), dir, Vector3(0,1,0));
	g_pDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);

	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	g_pDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

	g_pDevice->LightEnable(0, true);

	g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	//g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true) ;

	return true;
}


//
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
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

	case WM_DESTROY: //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


//����
void Render(int timeDelta)
{
	//ȭ�� û��
	if (SUCCEEDED(g_pDevice->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(150, 150, 150),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
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

		//������ ��
		g_pDevice->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
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

	string vtx, vtx_eq;
	int numVertices;
	fin >> vtx >> vtx_eq >> numVertices;

	if (numVertices <= 0)
		return  false;

	vtxSize = numVertices;

	// ���ؽ� ���� ����.
	if (FAILED(g_pDevice->CreateVertexBuffer( numVertices * sizeof(Vertex),
		D3DUSAGE_WRITEONLY, Vertex::FVF,
		D3DPOOL_MANAGED, &vtxBuff, NULL)))
	{
		return false;
	}

	// ���ؽ� ���� �ʱ�ȭ.
	Vertex* vertices;
	if (FAILED(vtxBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0)))
		return false;
	float num1, num2, num3;
	for (int i = 0; i < numVertices; i++)
	{
		fin >> num1 >> num2 >> num3;
		vertices[i] = Vertex(num1, num2, num3);
	}

	vertices[0].u = 0;
	vertices[0].v = 0;
	vertices[1].u = 2;
	vertices[1].v = 0;
	vertices[2].u = 2;
	vertices[2].v = 2;
	vertices[3].u = 0;
	vertices[3].v = 2;

	vtxBuff->Unlock();


	string idx, idx_eq;
	int numIndices;
	fin >> idx >> idx_eq >> numIndices;

	if (numIndices <= 0)
		return false;

	faceSize = numIndices;

	if (FAILED(g_pDevice->CreateIndexBuffer(numIndices*3*sizeof(WORD), 
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&idxBuff, NULL)))
	{
		return false;
	}

	WORD *indices = NULL;
	idxBuff->Lock(0, 0, (void**)&indices, 0);
	int num4, num5, num6;
	for (int i = 0; i < numIndices*3; i+=3)
	{
		fin >> num4 >> num5 >> num6;
		indices[ i] = num4;
		indices[ i+1] = num5;
		indices[ i+2] = num6;	
	}
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
	// ���ؽ� ���� ����.
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
	// ���ؽ� ���� ����.
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
