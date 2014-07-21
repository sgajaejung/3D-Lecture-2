
#include "base.h"
#include "sphere.h"


using namespace graphic;


cSphere::cSphere()
{

}

cSphere::cSphere(const float radian, const int numSlice)
{
	InitSphere(radian, numSlice);
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


void cSphere::InitSphere(const float radian, const int numSlice)
{

}
