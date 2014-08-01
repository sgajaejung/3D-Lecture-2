
#include "stdafx.h"
#include "base.h"
#include "grid.h"

using namespace graphic;


cGrid::cGrid()
{
}

cGrid::~cGrid()
{

}


void cGrid::Create( const int rowCellCount, const int colCellCount, const float cellSize )
{
	// init member
	m_rowCellCount = rowCellCount;
	m_colCellCount = colCellCount;
	m_cellSize = cellSize;

	// Init Grid
	const int rowVtxCnt  = rowCellCount+1;
	const int colVtxCnt  = colCellCount+1;
	const int cellCnt = rowCellCount * colCellCount;
	const int vtxCount= rowVtxCnt * colVtxCnt;

	m_vtxBuff.Create( vtxCount, sizeof(sVertexDiffuse), sVertexDiffuse::FVF);
	{
		sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock();
		const float startx = -cellSize*(rowCellCount/2);
		const float starty = cellSize*(colCellCount/2);
		const float endx = startx + cellSize*rowCellCount;
		const float endy = starty - cellSize*colCellCount;

		const float uCoordIncrementSize = 1.0f / (float)colCellCount;
		const float vCoordIncrementSize = 1.0f / (float)rowCellCount;

		int i=0;
		for (float y=starty; y >= endy; y -= cellSize, ++i)
		{
			int k=0;
			for (float x=startx; x <= endx; x += cellSize, ++k )
			{
				int index = (i * colVtxCnt) + k;
				vertices[ index].p = Vector3(x, 0.1f, y);
				vertices[ index].c = 0xffcccccc;
			}
		}
		m_vtxBuff.Unlock();
	}	


	m_idxBuff.Create( cellCnt*2 );
	{
		WORD *indices = (WORD*)m_idxBuff.Lock();
		int baseIndex = 0;
		for( int i=0; i < rowCellCount; ++i )
		{
			for( int k=0; k < colCellCount; ++k )
			{
				indices[ baseIndex] = (i * colVtxCnt) + k;
				indices[ baseIndex + 1] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 2] = ((i+1) * colVtxCnt) + k;

				indices[ baseIndex + 3] = ((i+1) * colVtxCnt) + k;
				indices[ baseIndex + 4] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 5] = ((i+1) * colVtxCnt) + k + 1;

				// next quad
				baseIndex += 6;
			}
		}	
		m_idxBuff.Unlock();
	}

}


void cGrid::Render()
{
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());
}


bool IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v )
{
	// Find vectors for two edges sharing vert0
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	D3DXVECTOR3 pvec;
	D3DXVec3Cross( &pvec, &dir, &edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot( &edge1, &pvec );

	D3DXVECTOR3 tvec;
	if( det > 0 )
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if( det < 0.0001f )
		return FALSE;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot( &tvec, &pvec );
	if( *u < 0.0f || *u > det )
		return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross( &qvec, &tvec, &edge1 );

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot( &dir, &qvec );
	if( *v < 0.0f || *u + *v > det )
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot( &edge2, &qvec );
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return TRUE;
}


bool cGrid::Pick(int x, int y, const Vector3 &orig, const Vector3 &dir,
	OUT Vector3 &out)
{
	sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock();
	WORD *indices = (WORD*)m_idxBuff.Lock();

	const int size = m_idxBuff.GetFaceCount()*3;
	for( int i=0; i < size; i+=3 )
	{
		const Vector3 v1 = vertices[ indices[ i+0]].p;
		const Vector3 v2 = vertices[ indices[ i+1]].p;
		const Vector3 v3 = vertices[ indices[ i+2]].p;

		float t, u, v;
		if (IntersectTriangle(
			*(D3DXVECTOR3*)&orig,
			*(D3DXVECTOR3*)&dir,
			*(D3DXVECTOR3*)&v1,
			*(D3DXVECTOR3*)&v2,
			*(D3DXVECTOR3*)&v3,
			&t, &u, &v))
		{
			out = orig + dir * t;
		}
	}

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
	return true;
}
