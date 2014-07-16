
#include "stdafx.h"
//#include "global.h"
#include "frustum.h"


/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	5.0f



//-----------------------------------------------------------------------------//
// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
//-----------------------------------------------------------------------------//
BOOL CFrustum::Make( Matrix44* pmatViewProj )
{
	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// m_Vtx�� �� ���������� ��谪�� �־�д�.
	m_Vtx[0].x = -1.0f;	m_Vtx[0].y = -1.0f;	m_Vtx[0].z = 0.0f;
	m_Vtx[1].x =  1.0f;	m_Vtx[1].y = -1.0f;	m_Vtx[1].z = 0.0f;
	m_Vtx[2].x =  1.0f;	m_Vtx[2].y = -1.0f;	m_Vtx[2].z = 1.0f;
	m_Vtx[3].x = -1.0f;	m_Vtx[3].y = -1.0f;	m_Vtx[3].z = 1.0f;
	m_Vtx[4].x = -1.0f;	m_Vtx[4].y =  1.0f;	m_Vtx[4].z = 0.0f;
	m_Vtx[5].x =  1.0f;	m_Vtx[5].y =  1.0f;	m_Vtx[5].z = 0.0f;
	m_Vtx[6].x =  1.0f;	m_Vtx[6].y =  1.0f;	m_Vtx[6].z = 1.0f;
	m_Vtx[7].x = -1.0f;	m_Vtx[7].y =  1.0f;	m_Vtx[7].z = 1.0f;

	// view * proj�� ������� ���Ѵ�.
	Matrix44 matInv = pmatViewProj->Inverse();

	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
	// �׷��Ƿ�, m_Vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	for( int i = 0; i < 8; i++ )
		m_Vtx[i] = m_Vtx[ i] * matInv;

	// 0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
	m_vPos = ( m_Vtx[0] + m_Vtx[5] ) / 2.0f;

	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
//	D3DXPlaneFromPoints(&m_plane[0], m_Vtx+4, m_Vtx+7, m_Vtx+6);	// �� ���(top)
//	D3DXPlaneFromPoints(&m_plane[1], m_Vtx  , m_Vtx+1, m_Vtx+2);	// �� ���(bottom)
//	D3DXPlaneFromPoints(&m_plane[2], m_Vtx  , m_Vtx+4, m_Vtx+5);	// �� ���(near)

	m_Plane[3].Init( m_Vtx[ 2], m_Vtx[ 6], m_Vtx[ 7] );	// �� ���(far)
	m_Plane[4].Init( m_Vtx[ 0], m_Vtx[ 3], m_Vtx[ 7] );	// �� ���(left)
	m_Plane[5].Init( m_Vtx[ 1], m_Vtx[ 5], m_Vtx[ 6] );	// �� ���(right)
	
	return TRUE;
}


//-----------------------------------------------------------------------------//
// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------//
BOOL CFrustum::IsIn( Vector3* pv )
{
	float dist;

	dist = m_Plane[ 3].Distance( *pv );
	// plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
	if( dist > PLANE_EPSILON ) return FALSE;
	dist = m_Plane[ 4].Distance( *pv );
	// plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
	if( dist > PLANE_EPSILON ) return FALSE;
	dist = m_Plane[ 5].Distance( *pv );
	// plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
	if( dist > PLANE_EPSILON ) return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------//
// �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
// TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------//
BOOL CFrustum::IsInSphere( Vector3* pv, float radius )
{
	float dist;

	dist = m_Plane[ 3].Distance( *pv );
	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
	if( dist > (radius+PLANE_EPSILON) ) return FALSE;
	dist = m_Plane[ 4].Distance( *pv );
	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
	if( dist > (radius+PLANE_EPSILON) ) return FALSE;
	dist = m_Plane[ 5].Distance( *pv );
	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
	if( dist > (radius+PLANE_EPSILON) ) return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------//
// ���
//-----------------------------------------------------------------------------//
void CFrustum::Render()
{
	WORD	index[] = { 0, 1, 2,
						0, 2, 3,
						4, 7, 6,
						4, 6, 5,
						1, 5, 6,
						1, 6, 2,
						0, 3, 7,
						0, 7, 4,
						0, 4, 5,
						0, 5, 1,
						3, 7, 6,
						3, 6, 2 };

    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

	typedef struct tagVTX
	{
		Vector3 p;
	} VTX;

	VTX vtx[8];

	for( int i = 0 ; i < 8 ; i++ )
		vtx[i].p = m_Vtx[i];

	g_pDevice->SetFVF( D3DFVF_XYZ );
	g_pDevice->SetStreamSource( 0, NULL, 0, sizeof(VTX) );
	g_pDevice->SetTexture( 0, NULL );
	g_pDevice->SetIndices( 0 );
	g_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	g_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	g_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// �Ķ������� ��,�� ����� �׸���.
    g_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    g_pDevice->SetMaterial( &mtrl );
	g_pDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	// ������� ��,�� ����� �׸���.
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    g_pDevice->SetMaterial( &mtrl );
	g_pDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+4*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	// ���������� ��,�� ����� �׸���.
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    g_pDevice->SetMaterial( &mtrl );
	g_pDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+8*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    g_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

}

