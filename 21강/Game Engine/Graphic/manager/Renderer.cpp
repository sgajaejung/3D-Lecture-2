
#include "stdafx.h"
#include "Renderer.h"
#include "../base/DxInit.h"

using namespace graphic;


// ������ �ʱ�ȭ.
bool graphic::InitRenderer(HWND hWnd, const int width, const int height)
{
	if (!cRenderer::Get()->CreateDirectX(hWnd, width, height))
		return false;

	return true;
}

void graphic::ReleaseRenderer()
{
	cRenderer::Release();
}



////////////////////////////////////////////////////////////////////////////////////////////////
// Renderer

cRenderer::cRenderer() :
	m_pDevice(NULL)
,	m_font(NULL)
,	m_elapseTime(0)
,	m_fps(0)
{
	m_fpsText = "fps : 0";

}

cRenderer::~cRenderer()
{
	SAFE_RELEASE(m_font);
	SAFE_RELEASE(m_pDevice);
}


// DirectX Device ��ü ����.
bool cRenderer::CreateDirectX(HWND hWnd, const int width, const int height)
{
	if (!InitDirectX(hWnd, width, height, m_pDevice))
		return false;

	HRESULT hr = D3DXCreateFontA( m_pDevice, 18, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "����", &m_font );
	if (FAILED(hr))
		return false;

	m_hWnd = hWnd;
	return true;
}


// x, y, z ���� ����Ѵ�.
void cRenderer::RenderAxis()
{
	RET(!m_pDevice);

	if (m_axis.empty())
		MakeAxis(500.f,  D3DXCOLOR(1,0,0,0),  D3DXCOLOR(0,1,0,0),  D3DXCOLOR(0,0,1,0), m_axis);

	m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	Matrix44 identity;
	m_pDevice->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&identity );
	m_pDevice->SetFVF( sVertexDiffuse::FVF );
	m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 3, &m_axis[0], sizeof(sVertexDiffuse) );
	m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}


// FPS ���.
void cRenderer::RenderFPS()
{
	RET(!m_font);

	RECT rc = {10,10,200,200};
	m_font->DrawTextA( NULL, m_fpsText.c_str(), -1, &rc,
		DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f ) );
}


// �׸��� ���.
void cRenderer::RenderGrid()
{
	static int gridSize = 0;
	if (m_grid.empty())
	{
		MakeGrid(50, 20, D3DXCOLOR(0.8f,0.8f,0.8f,1), m_grid);
		gridSize = m_grid.size() / 2;
	}

	if (gridSize > 0)
	{
		m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		Matrix44 identity;
		m_pDevice->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&identity );
		m_pDevice->SetFVF( sVertexDiffuse::FVF );
		m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, gridSize, &m_grid[0], sizeof(sVertexDiffuse) );
		m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	}
}



void cRenderer::Update(const float elapseT)
{
	// fps ��� ---------------------------------------
	++m_fps;
	m_elapseTime += elapseT;
	if( 1.f <= m_elapseTime )
	{
		m_fpsText = format("fps: %d", m_fps );
		m_fps = 0;
		m_elapseTime = 0;
	}
	//--------------------------------------------------
}


// ���ڹ��� ���ؽ��� �����. 
// width = ���� �ϳ��� �� ũ�� (�� �簢���̹Ƿ� ���ڰ��� �ϳ��� �޴´�)
// count = ���� ���� ���� ����
void cRenderer::MakeGrid( const float width, const int count, DWORD color, vector<sVertexDiffuse> &out )
{
	if (out.empty())
	{
		out.reserve(count * 4);
		const Vector3 start(count/2 * -width, 0, count/2 * width);

		for (int i=0; i < count+1; ++i)
		{
			sVertexDiffuse vtx;
			vtx.p = start;
			vtx.p.x += (i * width);
			vtx.c = color;
			out.push_back(vtx);

			vtx.p += Vector3(0,0,-width*count);
			out.push_back(vtx);
		}

		for (int i=0; i < count+1; ++i)
		{
			sVertexDiffuse vtx;
			vtx.p = start;
			vtx.p.z -= (i * width);
			vtx.c = color;
			out.push_back(vtx);

			vtx.p += Vector3(width*count,0,0);
			out.push_back(vtx);
		}
	}
}


// x, y, z ���� �����.
void cRenderer::MakeAxis( const float length, DWORD xcolor, DWORD ycolor, DWORD zcolor, 
	vector<sVertexDiffuse> &out )
{
	RET(!out.empty());

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

