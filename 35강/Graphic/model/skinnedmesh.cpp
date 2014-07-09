
#include "stdafx.h"
#include "skinnedmesh.h"

using namespace graphic;


cSkinnedMesh::cSkinnedMesh(const int id, const vector<Matrix44> &palette, const sRawMesh &raw) : 
	cMesh(id, raw)
,	m_rawMesh(raw)
,	m_palette(palette)
{

}

cSkinnedMesh::~cSkinnedMesh()
{

}


void cSkinnedMesh::Render(const Matrix44 &parentTm)
{
	ApplyPalette();
	cMesh::Render(parentTm);
}


// ÆÈ·¹Æ® Àû¿ë.
void cSkinnedMesh::ApplyPalette()
{
	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();

	BOOST_FOREACH (const sVertexWeight &weight, m_rawMesh.weights)
	{
		const int vtxIdx = weight.vtxIdx;
		vertices[ vtxIdx].p = Vector3(0,0,0);
		vertices[ vtxIdx].n = Vector3(0,0,0);

		for( int k=0; k < weight.size; ++k )
		{
			const sWeight *w = &weight.w[ k];
			Vector3 v = m_rawMesh.vertices[ vtxIdx] * m_palette[ w->bone];
			Vector3 n = m_rawMesh.normals[ vtxIdx].MultiplyNormal( m_palette[ w->bone] );
			vertices[ vtxIdx].p += v * w->weight;
			vertices[ vtxIdx].n += n * w->weight;
		}
	}

	m_vtxBuff.Unlock();
}
