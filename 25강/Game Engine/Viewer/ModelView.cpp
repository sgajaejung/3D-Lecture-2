// ModelView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "ModelView.h"


// CModelView
CModelView::CModelView() : 
	m_RButtonDown(false)
{

}

CModelView::~CModelView()
{
	SAFE_RELEASE(m_Mesh);
	SAFE_RELEASE(m_CloneMesh);
}

BEGIN_MESSAGE_MAP(CModelView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CModelView 그리기입니다.

void CModelView::OnDraw(CDC* pDC)
{
}


// CModelView 진단입니다.

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


// CModelView 메시지 처리기입니다.


void CModelView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
}

void CModelView::Render()
{
	//화면 청소
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		graphic::GetDevice()->BeginScene();

		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();


		m_Mtrl.Bind();
		m_Mesh->DrawSubset(0);


		//랜더링 끝
		graphic::GetDevice()->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}


void CModelView::Init()
{
	D3DXCreateBox(graphic::GetDevice(), 200, 200, 200, &m_Mesh, NULL);
	//D3DXCreateSphere(g_pDevice, 100, 10, 10, &m_Mesh, NULL);
	//D3DXCreateCylinder(g_pDevice, 100, 150, 200, 10, 10, &m_Mesh, NULL);
	//D3DXCreateTeapot(g_pDevice, &m_Mesh, NULL);
	//D3DXCreatePolygon(g_pDevice, 100, 20, &m_Mesh, NULL);
	//D3DXCreateTorus(g_pDevice, 50, 100, 20, 10, &m_Mesh, NULL);



	LPDIRECT3DVERTEXBUFFER9 vtxBuff;
	m_Mesh->GetVertexBuffer(&vtxBuff);
	LPDIRECT3DINDEXBUFFER9 idxBuff;
	m_Mesh->GetIndexBuffer(&idxBuff);

	const int fvf = m_Mesh->GetFVF();
	const int numVertices = m_Mesh->GetNumVertices();
	const int numBytesPerVertex = m_Mesh->GetNumBytesPerVertex();
	const int numFace = m_Mesh->GetNumFaces();
	const int option = m_Mesh->GetOptions(); // D3DXMESH_SYSTEMMEM, D3DXMESH_MANAGED, 


	void *vertices;
	m_Mesh->LockVertexBuffer(0, &vertices);
	WORD *indices;
	m_Mesh->LockIndexBuffer(0, (void**)&indices);


	vector<int> adjInfo(numFace*3, 0);
	m_Mesh->GenerateAdjacency(0.0, (DWORD*)&adjInfo[0]);

	vector<int> optAdjInfo(numFace*3, 0);
	m_Mesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)&adjInfo[0],
		(DWORD*)&optAdjInfo[0],
		NULL,
		NULL);


	DWORD numAttr;
	m_Mesh->GetAttributeTable( NULL, &numAttr );

	vector<D3DXATTRIBUTERANGE> attrRangeBuff(numAttr);
	m_Mesh->GetAttributeTable( &attrRangeBuff[0], &numAttr );


	m_Mesh->CloneMeshFVF(m_Mesh->GetOptions(),
		D3DFVF_XYZ | D3DFVF_NORMAL,
		graphic::GetDevice(),
		&m_CloneMesh
		);





	m_Mtrl.InitBlue();

	Vector4 color(1,1,1,1);
	m_Light.Init(graphic::cLight::LIGHT_DIRECTIONAL,
		color * 0.4f, color, color *0.6f, Vector3(0,-1,0));
	m_Light.Bind(0);

	m_camPos = Vector3(100,100,-500);
	m_lookAtPos = Vector3(0,0,0);
	UpdateCamera();

	const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
	const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
	Matrix44 proj;
	proj.SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

	graphic::GetDevice()->LightEnable (
		0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
		true); // true = 활성화 ， false = 비활성화
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
