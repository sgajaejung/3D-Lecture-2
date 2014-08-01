// MapView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapView.h"
#include "../../etc/Utility.h"

using namespace graphic;

const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기


// CMapView
CMapView::CMapView() :
	m_dxInit(false)
,	m_RButtonDown(false)
{

}

CMapView::~CMapView()
{
}

BEGIN_MESSAGE_MAP(CMapView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMapView 그리기입니다.

void CMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMapView 진단입니다.

#ifdef _DEBUG
void CMapView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapView 메시지 처리기입니다.
LPDIRECT3DDEVICE9 g_pDevice;
LPDIRECT3DDEVICE9 graphic::GetDevice()
{
	return g_pDevice;
}


bool CMapView::Init()
{
	if (!graphic::InitDirectX(m_hWnd, WINSIZE_X, WINSIZE_Y, g_pDevice))
	{
		return 0;
	}

	m_camPos = Vector3(100,100,-500);
	m_lookAtPos = Vector3(0,0,0);
	UpdateCamera();

	m_matProj.SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_matProj) ;

	graphic::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	graphic::GetDevice()->LightEnable (
		0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
		true); // true = 활성화 ， false = 비활성화


	m_grid.Create(64,64,50.f);
	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));
	m_cube.SetColor( 0xFF0000FF );
	m_dxInit = true;
	return true;
}


void CMapView::Render()
{
	if (!m_dxInit)
		return;

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

		//RenderFPS(timeDelta);
		GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);

		Matrix44 matIdentity;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matIdentity);
		m_grid.Render();
		RenderAxis();

		m_cube.Render(matIdentity);

		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}

}


void CMapView::Update(float elapseT)
{

}


void CMapView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_RButtonDown)
	{
		CPoint pos = point  - m_curPos;
		m_curPos = point;

		{ // rotate Y-Axis
			Quaternion q(Vector3(0,1,0), pos.x * 0.005f); 
			Matrix44 m = q.GetMatrix();
			m_camPos *= m;
		}

		{ // rotate X-Axis
			Quaternion q(Vector3(1,0,0), pos.y * 0.005f); 
			Matrix44 m = q.GetMatrix();
			m_camPos *= m;
		}

		UpdateCamera();
	}
	else
	{
		Vector3 orig, dir, pickPos;
		GetRay(point.x, point.y, orig, dir);
		if (m_grid.Pick(point.x, point.y, orig, dir, pickPos))
		{
			Matrix44 mT;
			mT.SetTranslate(pickPos);
			m_cube.SetTransform(mT);
		}
	}

	CView::OnMouseMove(nFlags, point);
}


void CMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}


void CMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonUp(nFlags, point);
}


void CMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CView::OnRButtonDown(nFlags, point);
}


void CMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CView::OnRButtonUp(nFlags, point);
}


void CMapView::UpdateCamera()
{
	Vector3 dir = m_lookAtPos - m_camPos;
	dir.Normalize();
	m_matView.SetView(m_camPos, dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_matView);
}


void CMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB)
	{
		static bool flag = false;
		GetDevice()->SetRenderState(D3DRS_CULLMODE, flag);
		GetDevice()->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
		flag = !flag;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMapView::GetRay(int sx, int sy, Vector3 &orig, Vector3 &dir)
{
	const float x =  ((sx * 2.0F / WINSIZE_X) - 1.0F );
	const float y = -((sy * 2.0F / WINSIZE_Y) - 1.0F );

	Vector3 v;
	v.x = x / m_matProj._11;
	v.y = y / m_matProj._22;
	v.z =  1.0F;

	Matrix44 m = m_matView.Inverse();

	dir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	dir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	dir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	dir.Normalize();

	orig.x = m._41;
	orig.y = m._42;
	orig.z = m._43;
}
