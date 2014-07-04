
#include "stdafx.h"
#include "mesh.h"

using namespace graphic;


cMesh::cMesh(const int id, const sRawMesh &raw) : 
	cNode(id)
{
	// 버텍스 버퍼 생성.
	if (m_vtxBuff.Create(raw.vertices.size(), sizeof(sVertexNormTex), sVertexNormTex::FVF))
	{
		sVertexNormTex* vertices = (sVertexNormTex*)m_vtxBuff.Lock();
		for (u_int i = 0; i < raw.vertices.size(); i++)
		{
			vertices[ i].p = raw.vertices[ i];
			vertices[ i].n = raw.normals[ i];
		}
		m_vtxBuff.Unlock();
	}

	// 인덱스 버퍼 생성.
	if (m_idxBuff.Create(raw.indices.size()))
	{
		WORD *indices = (WORD*)m_idxBuff.Lock();
		for (u_int i = 0; i < raw.indices.size(); ++i)
			indices[ i] = raw.indices[ i];
		m_idxBuff.Unlock();
	}

	m_mtrl.InitWhite();
}

cMesh::~cMesh()
{

}


// Animation
bool cMesh::Move(const float elapseTime)
{
	return true;
}


// Render
void cMesh::Render(const Matrix44 &parentTm)
{
	m_mtrl.Bind();
	m_vtxBuff.Bind();
	m_idxBuff.Bind();

	const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&tm );
	GetDevice()->DrawIndexedPrimitive( 
		D3DPT_TRIANGLELIST, 0, 0, 
		m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());
}


// Render Bounding Box
void cMesh::RenderBBox()
{

}
