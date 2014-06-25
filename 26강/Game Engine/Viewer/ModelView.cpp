// ModelView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer.h"
#include "ModelView.h"


// CModelView
CModelView::CModelView() : 
	m_RButtonDown(false)
,	m_Mesh(NULL)
{

}

CModelView::~CModelView()
{
	SAFE_RELEASE(m_Mesh);
}

BEGIN_MESSAGE_MAP(CModelView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CModelView �׸����Դϴ�.

void CModelView::OnDraw(CDC* pDC)
{
}


// CModelView �����Դϴ�.

#ifdef _DEBUG
void CModelView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CModelView �޽��� ó�����Դϴ�.


void CModelView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
}

void CModelView::Render()
{
	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(150, 150, 150),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();

		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();


		m_Mtrl.Bind();

		for( int i = 0; i < (int)m_Mtrls.size(); ++i )
		{
			m_Mtrls[ i].Bind();
			m_Textures[ i].Bind(0);
			m_Mesh->DrawSubset( i );
		}

		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}


void CModelView::Init()
{
	HRESULT hr;
	ID3DXBuffer *adjBuff = NULL;
	ID3DXBuffer *mtrlBuff = NULL;
	DWORD numMtrls = 0;

	hr = D3DXLoadMeshFromXA( "../media/seat woman.x", D3DXMESH_MANAGED, 
		graphic::GetDevice(), &adjBuff, &mtrlBuff, 0, &numMtrls, &m_Mesh );
	if (FAILED(hr))
		return;

	/////////////////////////////////////////////////////////////////////////////
	if (mtrlBuff && numMtrls > 0)
	{
		D3DXMATERIAL *mtrls = (D3DXMATERIAL*)mtrlBuff->GetBufferPointer();
		m_Textures.reserve( numMtrls );

		for (int i=0; i < (int)numMtrls; ++i)
		{
			mtrls[ i].MatD3D.Ambient = mtrls[ i].MatD3D.Diffuse;
			graphic::cMaterial material;
			material.Init(mtrls[ i].MatD3D);
			m_Mtrls.push_back( material );

			if (mtrls[ i].pTextureFilename)
			{
				string filePath = "../media/";
				filePath += mtrls[ i].pTextureFilename;

				m_Textures.push_back(graphic::cTexture());
				m_Textures.back().Create( filePath );
			}
			else
			{
				m_Textures.push_back(graphic::cTexture());
			}
		}
	}
	mtrlBuff->Release();
	adjBuff->Release();



	m_Mtrl.InitBlue();

	Vector4 color(1,1,1,1);
	m_Light.Init(graphic::cLight::LIGHT_DIRECTIONAL,
		color * 0.4f, color, color *0.6f, Vector3(0,-1,0));
	m_Light.Bind(0);

	m_camPos = Vector3(100,100,-500);
	m_lookAtPos = Vector3(0,0,0);
	UpdateCamera();

	const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	Matrix44 proj;
	proj.SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ
}


void CModelView::UpdateCamera()
{
	Matrix44 V;
	Vector3 dir = m_lookAtPos - m_camPos;
	dir.Normalize();
	V.SetView(m_camPos, dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);
}

void CModelView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	m_RButtonDown = true;
	m_curPos = point;
	CView::OnRButtonDown(nFlags, point);
}


void CModelView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_RButtonDown = false;
	CView::OnRButtonUp(nFlags, point);
}

void CModelView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_RButtonDown)
	{		
		CPoint pos = point - m_curPos;
		m_curPos = point;

		Matrix44 rx;
		rx.SetRotationY( pos.x * 0.005f );

		Matrix44 ry;
		ry.SetRotationX( pos.y * 0.005f );

		Matrix44 m = rx * ry;
		m_camPos *= m;

		UpdateCamera();
	}	

	CView::OnMouseMove(nFlags, point);
}


BOOL CModelView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Vector3 dir = m_lookAtPos - m_camPos;
	dir.Normalize();

	m_camPos += (zDelta < 0)? dir * -50 : dir*50;
	UpdateCamera();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
