
#include "base.h"
#include "grid2.h"

using namespace graphic;


cGrid2::cGrid2()
{
	m_mtrl.InitWhite();
}

cGrid2::~cGrid2()
{

}


void cGrid2::Create( const int rowCellCount, const int colCellCount, const float cellSize,
	const float textureUVFactor)
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

	m_vtxBuff.Create( vtxCount, sizeof(sVertexNormTex), sVertexNormTex::FVF);
	{
		sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
		const float startx = -cellSize*(rowCellCount/2);
		const float starty = cellSize*(colCellCount/2);
		const float endx = startx + cellSize*rowCellCount;
		const float endy = starty - cellSize*colCellCount;

		const float uCoordIncrementSize = 1.0f / (float)colCellCount * textureUVFactor;
		const float vCoordIncrementSize = 1.0f / (float)rowCellCount * textureUVFactor;

		int i=0;
		for (float y=starty; y >= endy; y -= cellSize, ++i)
		{
			int k=0;
			for (float x=startx; x <= endx; x += cellSize, ++k )
			{
				int index = (i * colVtxCnt) + k;
				vertices[ index].p = Vector3(x, 0.1f, y);
				vertices[ index].n = Vector3(0,1,0);
				vertices[ index].u = (float)k*uCoordIncrementSize;
				vertices[ index].v = (float)i*vCoordIncrementSize;
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


void cGrid2::Render(const int stage)
{
	m_mtrl.Bind();
	m_tex.Bind(stage);
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());
}


void cGrid2::RenderShader(cShader &shader)
{
	Matrix44 matIdentity;
	shader.SetMatrix( "mWorld", matIdentity);

	m_mtrl.Bind(shader);
	m_tex.Bind(shader, "Tex");

	shader.Begin();
	shader.BeginPass();

	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());

	shader.EndPass();
	shader.End();
}


// 법선 벡터를 다시 계산한다.
void cGrid2::CalculateNormals()
{
	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	WORD *indices = (WORD*)m_idxBuff.Lock();
	for (int i=0; i < m_idxBuff.GetFaceCount()*3; i+=3)
	{
		sVertexNormTex &p1 = vertices[ indices[ i]];
		sVertexNormTex &p2 = vertices[ indices[ i+1]];
		sVertexNormTex &p3 = vertices[ indices[ i+2]];

		Vector3 v1 = p2.p - p1.p;
		Vector3 v2 = p3.p - p1.p;
		v1.Normalize();
		v2.Normalize();
		Vector3 n = v1.CrossProduct(v2);
		n.Normalize();
		p1.n = n;
		p2.n = n;
		p3.n = n;
	}

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
}


