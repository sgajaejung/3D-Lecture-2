
#include "base.h"
#include "cubetex.h"

using namespace graphic;


cCubeTex::cCubeTex()
{
}

cCubeTex::cCubeTex(const Vector3 &vMin, const Vector3 &vMax )
{
	InitCube();
	SetCube(vMin, vMax);
}


void cCubeTex::InitCube()
{
	if (m_vtxBuff.GetVertexCount() > 0)
		return;

	//        4 --- 5
	//      / |  |  /|
	//   0 --- 1   |
	//   |   6-|- -7
	//   | /     | /
	//   2 --- 3
	//
	Vector3 vertices[8] = {
		Vector3(-1,1,-1), Vector3(1,1,-1), Vector3(-1,-1,-1), Vector3(1,-1,-1),
		Vector3(-1,1, 1), Vector3(1,1, 1), Vector3(-1,-1,1), Vector3(1,-1,1),
	};
	Vector2 texuv[8] = {
		Vector2(0,0), Vector2(1,0), Vector2(0,1), Vector2(1,1),
		Vector2(0,1), Vector2(1,1), Vector2(0,0), Vector2(1,0),
	};
	
	WORD indices[36] = {
		// front
		0, 3, 2,
		0 ,1, 3,
		// back
		5, 6, 7,
		5, 4, 6,
		// top
		4, 1, 0,
		4, 5, 1,
		// bottom
		2, 7, 6,
		2, 3, 7,
		// left
		4, 2, 6,
		4, 0, 2,
		// right
		1, 7, 3,
		1, 5, 7,
	};

	m_vtxBuff.Create(8, sizeof(sVertexTex), sVertexTex::FVF);
	m_idxBuff.Create(12);

	sVertexTex *vbuff = (sVertexTex*)m_vtxBuff.Lock();
	WORD *ibuff = (WORD*)m_idxBuff.Lock();

	for (int i=0; i < 8; ++i)
	{
		vbuff[ i].p = vertices[ i];
		vbuff[ i].u = texuv[ i].x;
		vbuff[ i].v = texuv[ i].y;
	}

	for (int i=0; i < 36; ++i)
		ibuff[ i] = indices[ i];

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
}


void cCubeTex::SetCube(const Vector3 &vMin, const Vector3 &vMax )
{
	if (m_vtxBuff.GetVertexCount() <= 0)
		InitCube();

	//        4 --- 5
	//      / |  |  /|
	//   0 --- 1   |
	//   |   6-|- -7
	//   | /     | /
	//   2 --- 3
	//
	// min = index 2
	// max = index 5

	Vector3 vertices[8] = {
		Vector3(vMin.x, vMax.y, vMin.z), 
		Vector3(vMax.x, vMax.y, vMin.z), 
		Vector3(vMin.x, vMin.y, vMin.z), 
		Vector3(vMax.x, vMin.y, vMin.z),
		Vector3(vMin.x, vMax.y, vMax.z), 
		Vector3(vMax.x, vMax.y, vMax.z),
		Vector3(vMin.x, vMin.y, vMax.z), 
		Vector3(vMax.x, vMin.y, vMax.z),
	};

	sVertexTex *vbuff = (sVertexTex*)m_vtxBuff.Lock();
	for (int i=0; i < 8; ++i)
		vbuff[ i].p = vertices[ i];
	m_vtxBuff.Unlock();

	m_min = vMin;
	m_max = vMax;
}


void cCubeTex::Render(const Matrix44 &tm)
{
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	Matrix44 mat = m_tm * tm;
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&mat );
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
		m_vtxBuff.GetVertexCount(), 0, 12);

	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
}
