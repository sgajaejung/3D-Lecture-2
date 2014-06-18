
#include "stdafx.h"
#include "Renderer.h"
#include "../base/DxInit.h"

using namespace graphic;


// ·»´õ·¯ ÃÊ±âÈ­.
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
{

}

cRenderer::~cRenderer()
{
	SAFE_RELEASE(m_pDevice);
}


// DirectX Device °´Ã¼ »ý¼º.
bool cRenderer::CreateDirectX(HWND hWnd, const int width, const int height)
{
	if (!InitDirectX(hWnd, width, height, m_pDevice))
		return false;

	m_hWnd = hWnd;
	return true;
}


void cRenderer::Update(const float elapseT)
{

}
