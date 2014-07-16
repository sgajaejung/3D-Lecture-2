
#include "stdafx.h"
#include "texture.h"


using namespace graphic;


cTexture::cTexture() :
	m_texture(NULL)
{
}

cTexture::~cTexture()
{
	Clear();
}


bool cTexture::Create(const string &fileName, bool isSizePow2)//isSizePow2=true
{
	if (isSizePow2)
	{
		if (FAILED(D3DXCreateTextureFromFileA(GetDevice(), fileName.c_str(), &m_texture)))
		{
			string path = "../media/";
			path += common::GetFileName(fileName);
		
			if (FAILED(D3DXCreateTextureFromFileA(GetDevice(), path.c_str(), &m_texture)))
				return false;

			return true;
		}
	}
	else
	{
		return CreateEx(fileName);
	}

	return true;
}


bool cTexture::Create(const int width, const int height, const D3DFORMAT format)
{
	if (FAILED(GetDevice()->CreateTexture( width, height, 1, 0, format, 
		D3DPOOL_MANAGED, &m_texture, NULL )))
		return false;

	D3DLOCKED_RECT lockrect;
	m_texture->LockRect( 0, &lockrect, NULL, 0 );
	memset( lockrect.pBits, 0x00, lockrect.Pitch*height );
	m_texture->UnlockRect( 0 );
	return true;
}


// D3DX_DEFAULT_NONPOW2 옵션을 켠 상태에서 텍스쳐를 생성한다.
bool cTexture::CreateEx(const string &fileName)
{
	if (FAILED(D3DXCreateTextureFromFileExA(
		GetDevice(), fileName.c_str(),
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, // option On
		NULL, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		0,
		&m_imageInfo,
		NULL,
		&m_texture)))
	{
		string path = "../media/";
		path += common::GetFileName(fileName);

		if (FAILED(D3DXCreateTextureFromFileExA(
			GetDevice(), path.c_str(),
			D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, // option On
			NULL, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
			0,
			&m_imageInfo,
			NULL,
			&m_texture)))
			return false;
	}
	
	return true;
}


void cTexture::Bind(int stage)
{
	GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	GetDevice()->SetTexture(stage, m_texture);
}


void cTexture::Lock(D3DLOCKED_RECT &out)
{
	m_texture->LockRect( 0, &out, NULL, 0 );
}


void cTexture::Unlock()
{
	m_texture->UnlockRect( 0 );
}


void cTexture::Clear()
{
	SAFE_RELEASE(m_texture);
}
