
#include "stdafx.h"
#include "mesh.h"

using namespace graphic;


cMesh::cMesh(const int id, const sRawMesh &rawMesh) : 
	cNode(id)
{
	CreateMesh(rawMesh.vertices, rawMesh.normals, rawMesh.tex, rawMesh.indices);
	m_mtrl.Init( rawMesh.mtrl );

	if (!rawMesh.mtrl.texture.empty())
		m_texture.Create( rawMesh.mtrl.texture );
}

cMesh::cMesh(const int id, const sRawBone &rawBone) : 
	cNode(id)
{
	CreateMesh(rawBone.vertices, rawBone.normals, rawBone.tex, rawBone.indices);
	m_mtrl.InitWhite();
}

cMesh::~cMesh()
{

}


void cMesh::CreateMesh( const vector<Vector3> &vertices, 
	const vector<Vector3> &normals, 
	const vector<Vector3> &tex,
	const vector<int> &indices )
{
	const bool isTexture = !tex.empty();

	// 버텍스 버퍼 생성.
	if (m_vtxBuff.Create(vertices.size(), sizeof(sVertexNormTex), sVertexNormTex::FVF))
	{
		sVertexNormTex* pv = (sVertexNormTex*)m_vtxBuff.Lock();
		for (u_int i = 0; i < vertices.size(); i++)
		{
			pv[ i].p = vertices[ i];
			pv[ i].n = normals[ i];
			if (isTexture)
			{
				pv[ i].u = tex[ i].x;
				pv[ i].v = tex[ i].y;
			}
		}
		m_vtxBuff.Unlock();
	}

	// 인덱스 버퍼 생성.
	if (m_idxBuff.Create(indices.size()))
	{
		WORD *pi = (WORD*)m_idxBuff.Lock();
		for (u_int i = 0; i < indices.size(); ++i)
			pi[ i] = indices[ i];
		m_idxBuff.Unlock();
	}
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
	m_texture.Bind(0);
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
