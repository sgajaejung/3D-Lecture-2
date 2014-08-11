
#include "../base/base.h"
#include "Utility.h"
#include <sstream>


namespace graphic
{
	void MakeAxis( const float length, DWORD xcolor, DWORD ycolor, DWORD zcolor, vector<sVertexDiffuse> &out );
	bool InitFont();

	vector<sVertexDiffuse> g_axis;
	ID3DXFont *g_font = NULL;
	string g_fpsText;
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

	DWORD lighting;
	GetDevice()->GetRenderState( D3DRS_LIGHTING, &lighting );
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

	Matrix44 identity;
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&identity );
	GetDevice()->SetTexture(0, NULL);
	GetDevice()->SetFVF( sVertexDiffuse::FVF );
	GetDevice()->DrawPrimitiveUP( D3DPT_LINELIST, 3, &g_axis[0], sizeof(sVertexDiffuse) );

	GetDevice()->SetRenderState( D3DRS_LIGHTING, lighting );
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


void graphic::RenderFPS(int timeDelta)
{
	static int incT = 0;
	static int frame = 0;

	++frame;
	incT += timeDelta;

	if (incT > 1000)
	{
		std::stringstream ss;
		ss << "fps: " << frame;
		frame = 0;
		incT = 0;
		g_fpsText = ss.str();
	}

	if (!g_font)
		InitFont();

	RECT rc = {10,10,200,200};
	g_font->DrawTextA( NULL, g_fpsText.c_str(), -1, &rc,
		DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f ) );
}


bool graphic::InitFont()
{
	HRESULT hr = D3DXCreateFontA( GetDevice(), 18, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림", &g_font );
	if (FAILED(hr))
		return false;

	return true;
}
