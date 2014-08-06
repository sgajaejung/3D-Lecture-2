
// Alpha Blending Code

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
const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
const int WINPOS_X = 0; //�ʱ� ������ ���� ��ġ X
const int WINPOS_Y = 0; //�ʱ� ������ ���� ��ġ Y

POINT g_CurPos;
bool g_LButtonDown = false;
bool g_RButtonDown = false;
Matrix44 g_LocalTm;
Vector3 g_camPos(0,100,-200);
Vector3 g_lookAtPos(0,0,0);
Matrix44 g_matProj;
Matrix44 g_matView;

graphic::cLine g_line( D3DCOLOR_XRGB(0,255,0) );
graphic::cGrid2 g_grid;
const int TEXTURE_SIZE = 100;
const float CELL_SIZE = 50.f;
const int COL_CELL_COUNT = 16;
const int ROW_CELL_COUNT = 16;



LPDIRECT3DDEVICE9 graphic::GetDevice()
{
	return g_pDevice;
}


// �ݹ� ���ν��� �Լ� ������ Ÿ��
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
	wchar_t className[32] = L"Paint Texture";
	wchar_t windowName[32] = L"Paint Texture";

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

	if (!graphic::InitDirectX(hWnd, WINSIZE_X, WINSIZE_Y, g_pDevice))
	{
		return 0;
	}

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
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
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

		RenderFPS(timeDelta);

		Matrix44 tm = g_LocalTm;
		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&tm);

		g_grid.Render();

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


		//������ ��
		g_pDevice->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}
}


bool InitVertexBuffer()
{
	g_grid.Create(ROW_CELL_COUNT, COL_CELL_COUNT, CELL_SIZE, 1.f);
	g_grid.GetTexture().Create(TEXTURE_SIZE, TEXTURE_SIZE, D3DFMT_A8R8G8B8);

	// ī�޶�, ������� ����
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

	sVertexNormTex *vertices = (sVertexNormTex*)g_grid.GetVertexBuffer().Lock();
	WORD *indices = (WORD*)g_grid.GetIndexBuffer().Lock();

	const int size = g_grid.GetIndexBuffer().GetFaceCount()*3;
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

			//���콺 ��ŷ�� ���ؼ� 3�����󿡼� ��ǥ�� ����ٸ�, �ؽ��� uv ��ǥ�� ��ȯ�Ѵ�.
			float tu, tv;
			GetTextureUV(pos, tu, tv);
			Brush(tu, tv);
		}
	}

	g_grid.GetVertexBuffer().Unlock();
	g_grid.GetIndexBuffer().Unlock();
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
	g_grid.GetTexture().Lock(lockrect);

	const float innerRadius = 10;
	const float outterRadius = 30;
	const float innerAlpha = 1.f;
	const float outterAlpha = 0.f;

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
				*ppixel = D3DCOLOR_XRGB(color, color, color);
			}
			else if (len <= outterRadius)
			{
				// ����
				const float width = outterRadius - innerRadius;
				const float delta = 1.f - ((len - innerRadius) / width);
				const int color = (int)((((innerAlpha - outterAlpha) * delta) + outterAlpha) * 255.f);

				const int dest = *ppixel & 0xff;
				if (color > dest)
					*ppixel = D3DCOLOR_XRGB(color, color, color);
			}
		}
	}

	g_grid.GetTexture().Unlock();
}


// 3���� ���� ���� ��ǥ�� �ؽ��� uv ��ǥ�� ��ȯ�Ѵ�.
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
