
#include "../../base/base.h"
#include "Utility.h"


namespace graphic
{
	void MakeAxis( const float length, DWORD xcolor, DWORD ycolor, DWORD zcolor, vector<sVertexDiffuse> &out );

	vector<sVertexDiffuse> g_axis;
}

using namespace graphic;


// x, y, z 축을 출력한다.
void graphic::RenderAxis()
{
	if (!GetDevice())
		return;

	if (g_axis.empty())
		MakeAxis(500.f,  D3DXCOLOR(1,0,0,0),  D3DXCOLOR(0,1,0,0),  D3DXCOLOR(0,0,1,0), g_axis);

	GetDevice()->SetRenderState( D3DRS_ZENABLE, FALSE);
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	Matrix44 identity;
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&identity );
	GetDevice()->SetFVF( sVertexDiffuse::FVF );
	GetDevice()->DrawPrimitiveUP( D3DPT_LINELIST, 3, &g_axis[0], sizeof(sVertexDiffuse) );
	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	GetDevice()->SetRenderState( D3DRS_ZENABLE, TRUE);
}


// x, y, z 축을 만든다.
void graphic::MakeAxis( const float length, DWORD xcolor, DWORD ycolor, DWORD zcolor, 
	vector<sVertexDiffuse> &out )
{
	if (!out.empty())
		return;

	out.reserve(6);

	sVertexDiffuse v;

	// x axis
	v.p = Vector3( 0.f, 0.f, 0.f );
	v.c = xcolor;
	out.push_back(v);

	v.p = Vector3( length, 0.f, 0.f );
	v.c = xcolor;
	out.push_back(v);

	// y axis
	v.p = Vector3( 0.f, 0.f, 0.f );
	v.c = ycolor;
	out.push_back(v);

	v.p = Vector3( 0.f, length, 0.f );
	v.c = ycolor;
	out.push_back(v);

	// z axis
	v.p = Vector3( 0.f, 0.f, 0.f );
	v.c = zcolor;
	out.push_back(v);

	v.p = Vector3( 0.f, 0.f, length );
	v.c = zcolor;
	out.push_back(v);
}

