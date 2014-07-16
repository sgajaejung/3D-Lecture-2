
#include "stdafx.h"
#include "mesh.h"

using namespace graphic;


cMesh::cMesh(const int id, const sRawMesh &rawMesh) : 
	cNode(id)
{
	CreateMesh(rawMesh.vertices, rawMesh.normals, rawMesh.tex, rawMesh.indices);
	CreateMaterials(rawMesh);
	CreateAttributes(rawMesh);
}

cMesh::cMesh(const int id, const sRawBone &rawBone) : 
	cNode(id)
{
	CreateMesh(rawBone.vertices, rawBone.normals, rawBone.tex, rawBone.indices);
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


// 매터리얼 생성.
void cMesh::CreateMaterials(const sRawMesh &rawMesh)
{
	m_textures.reserve(rawMesh.mtrls.size());
	m_mtrls.reserve(rawMesh.mtrls.size());

	BOOST_FOREACH (auto &mtrl, rawMesh.mtrls)
	{
		m_mtrls.push_back(cMaterial());
		m_mtrls.back().Init(mtrl);

		m_textures.push_back( cResourceManager::Get()->LoadTexture(mtrl.texture) );
	}
}


// 속성버퍼 초기화.
void cMesh::CreateAttributes(const sRawMesh &rawMesh)
{
	m_attributes = rawMesh.attributes;

}


// Animation
bool cMesh::Move(const float elapseTime)
{
	return true;
}


// Render
void cMesh::Render(const Matrix44 &parentTm)
{
	if (m_attributes.empty())
	{
		if (!m_mtrls.empty())
			m_mtrls[ 0].Bind();
		if (!m_textures.empty())
			m_textures[ 0]->Bind(0);

		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&tm );
		GetDevice()->DrawIndexedPrimitive( 
			D3DPT_TRIANGLELIST, 0, 0, 
			m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());
	}
	else
	{
		m_vtxBuff.Bind();
		m_idxBuff.Bind();

		const Matrix44 tm = m_localTM * m_aniTM * m_TM * parentTm;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&tm );

		for (u_int i=0; i < m_attributes.size(); ++i)
		{
			const int mtrlId = m_attributes[ i].attribId;
			if ((int)m_mtrls.size() <= mtrlId)
				continue;
			
			m_mtrls[ mtrlId].Bind();
			if (m_textures[ mtrlId])
				m_textures[ mtrlId]->Bind(0);

			GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
				m_vtxBuff.GetVertexCount(), 
				m_attributes[ i].faceStart*3, m_attributes[ i].faceCount);
		}
	}
}


// Render Bounding Box
void cMesh::RenderBBox()
{

}
