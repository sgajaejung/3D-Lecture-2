
#include "stdafx.h"
#include "CrossProductView.h"

const float VECTOR_LENGTH = 100;

// CCrossProductView
CCrossProductView::CCrossProductView()
{
	//                   Y    Z
	//                   |   /
	//                   | /
	//    -----------------------> X
	const float axisLineLength = 200.f;
	m_axises.reserve(8);
	m_axises.push_back( Vector3(0,0,0) );
	m_axises.push_back( Vector3(axisLineLength,0,0) ); // x-axis
	m_axises.push_back( Vector3(0,0,0) );
	m_axises.push_back( Vector3(0,axisLineLength,0) ); // y-axis
	m_axises.push_back( Vector3(0,0,0) );
	m_axises.push_back( Vector3(0,0,axisLineLength) ); // z-axis

	m_dir0.push_back( Vector3(0,0,0) );
	m_dir0.push_back( Vector3(-1,0,1).Normal() * VECTOR_LENGTH );

	m_dir1.push_back( Vector3(0,0,0) );
	m_dir1.push_back( Vector3(1,0,1).Normal() * VECTOR_LENGTH );

	m_dir2.push_back( Vector3(0,0,0) );
	m_dir2.push_back( Vector3(0,1,0).Normal() * VECTOR_LENGTH );

	Vector3 cameraLookat(0,0,0);
	Vector3 cameraPos(-95, 200, -175);
	Vector3 dir = cameraLookat - cameraPos;
	dir.Normalize();
	m_matView.SetView(cameraPos, dir, Vector3(0,1,0));
	m_matProjection.SetProjection( MATH_PI / 4.f, 1.0f, 1.0f, 100.0f );

	const float width = 400.f;
	const float height = 400.f;
	m_matViewPort.SetIdentity();
	m_matViewPort._11 = width/2;
	m_matViewPort._22 = -height/2;
	m_matViewPort._33 = 0;
	m_matViewPort._41 = width/2;
	m_matViewPort._42 = height/2;
	m_matViewPort._43 = 0;

	m_bluePen = CreatePen(PS_SOLID, 2, RGB(0,0,255));
	m_redPen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
	m_greenPen = CreatePen(PS_SOLID, 2, RGB(0,255,0));
}

CCrossProductView::~CCrossProductView()
{
	DeleteObject(m_bluePen);
	DeleteObject(m_redPen);

}

BEGIN_MESSAGE_MAP(CCrossProductView, C3DView)
END_MESSAGE_MAP()


void CCrossProductView::UpdateVector(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
	m_dir0[ 1] = v1 * VECTOR_LENGTH;
	m_dir1[ 1] = v2 * VECTOR_LENGTH;
	m_dir2[ 1] = v3 * VECTOR_LENGTH;
}

void CCrossProductView::Render()
{
	CDC *pDc = GetDC();
	HDC hdc = pDc->GetSafeHdc();

	RECT rc;
	GetClientRect(&rc);
	HDC memDc = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right-rc.left, rc.bottom-rc.top);
	HBITMAP hbmOld = (HBITMAP)SelectObject(memDc, hbmMem);
	HBRUSH hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	FillRect(memDc, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);

	const Matrix44 vpv = m_matView * m_matProjection * m_matViewPort;

	RenderVertices(memDc, m_axises, vpv);

	HPEN oldPen = (HPEN)SelectObject(memDc, m_redPen);
	RenderVertices(memDc, m_dir0, vpv);
	SelectObject(memDc, m_bluePen);	
	RenderVertices(memDc, m_dir1, vpv);
	SelectObject(memDc, m_greenPen);	
	RenderVertices(memDc, m_dir2, vpv);
	SelectObject(memDc, oldPen);

	BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, memDc, 0, 0, SRCCOPY);
	SelectObject(memDc, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(memDc);

	ReleaseDC(pDc);
}
