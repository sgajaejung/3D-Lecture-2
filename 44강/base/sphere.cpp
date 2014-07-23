#include "base.h"
#include "sphere.h"


using namespace graphic;


cSphere::cSphere()
{

}

cSphere::cSphere(const float radius, const int numSegments, const int numSlices)
{
	Create(radius, numSegments, numSlices);
}

cSphere::~cSphere()
{

}


void cSphere::Render(const Matrix44 &tm)
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


void cSphere::Create(const float radius, const int nSegments, const int nRings)
{
	if (m_vtxBuff.GetVertexCount() > 0)
		return;

	const int dwVertices = ( nRings + 1 ) * ( nSegments + 1 ) ;
	const int dwIndices = 2 * nRings * ( nSegments + 1 ) ;
	m_vtxBuff.Create(dwVertices, sizeof(sVertexNormDiffuse), sVertexNormDiffuse::FVF);
	m_idxBuff.Create(dwIndices);

	sVertexNormDiffuse *pVertex = (sVertexNormDiffuse*)m_vtxBuff.Lock();
	WORD *pIndices = (WORD*)m_idxBuff.Lock();



	// Establish constants used in sphere generation
	FLOAT fDeltaRingAngle = ( D3DX_PI / nRings );
	FLOAT fDeltaSegAngle = ( 2.0f * D3DX_PI / nSegments );

	WORD wVerticeIndex = 0 ; 
	// Generate the group of rings for the sphere
	for( int ring = 0; ring < nRings + 1 ; ring++ )
	{
		FLOAT r0 = sinf ( ring * fDeltaRingAngle );
		FLOAT y0 = cosf ( ring * fDeltaRingAngle );

		// Generate the group of segments for the current ring
		for( int seg = 0; seg < nSegments + 1 ; seg++ )
		{
			FLOAT x0 = r0 * sinf( seg * fDeltaSegAngle );
			FLOAT z0 = r0 * cosf( seg * fDeltaSegAngle );

			// Add one vertices to the strip which makes up the sphere
			pVertex->p = Vector3(x0,y0,z0) * radius; // normalized 
			pVertex->n = pVertex->p.Normal(); 
			//pVertex->color = 0xffffffff ; 
			//pVertex->tu = (FLOAT) seg / (FLOAT) nSegments;
			//pVertex->tv = (FLOAT) ring / (FLOAT) nRings;

			pVertex ++;

			// add two indices except for last ring 
			if ( ring != nRings ) 
			{
				* pIndices = wVerticeIndex ; 
				pIndices ++ ;
				* pIndices = wVerticeIndex + ( WORD ) ( nSegments + 1 ) ; 
				pIndices ++ ;
				wVerticeIndex ++ ; 
			}

		}// end for seg 

	} // end for ring 


/*
	std::vector<sVertexNormDiffuse> verts;
	WORD wVerticeIndex = 0;
	int count = 0;
	while (count < numSlices)
	{
		const float phi = MATH_PI / numSlices * count;
		int count2  = 0;
		while( count2 < numSegments )
		{
			const float theta =  MATH_2PI / numSegments * count2;
			const float xzRadius = fabsf( radius * cosf( phi ) );

			sVertexNormDiffuse v;
			v.p.x = xzRadius * cosf( theta );
			v.p.y = radius * sinf( phi );
			v.p.z = xzRadius * sinf( theta );

			const float fRcpLen = 
				1.0f / sqrtf( (v.p.x * v.p.x) + (v.p.y * v.p.y) + (v.p.z * v.p.z) );
			v.n.x    = v.p.x * fRcpLen;
			v.n.y    = v.p.y * fRcpLen;
			v.n.z    = v.p.z * fRcpLen;

			*vertices = v;
			if  (count != numSlices) 
			{
				*indices = wVerticeIndex ; 
				indices++ ;
				*indices = wVerticeIndex + (WORD)(numSegments + 1);
				indices++ ;
				wVerticeIndex ++ ; 
			} 

			verts.push_back( v );
			count2++;
			vertices++;
		}
		count++;
	}
/**/

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
}

