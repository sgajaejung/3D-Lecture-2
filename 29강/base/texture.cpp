
#include "base.h"
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


bool cTexture::Create(const string &fileName)
{
	if (FAILED(D3DXCreateTextureFromFileA(GetDevice(), fileName.c_str(), &m_texture)))
		return false;

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


void cTexture::Bind(int stage)
{
	GetDevice()->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GetDevice()->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GetDevice()->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	GetDevice()->SetTexture(stage, m_texture);
}


void cTexture::Clear()
{
	SAFE_RELEASE(m_texture);
}


void cTexture::Lock(D3DLOCKED_RECT &out)
{
	m_texture->LockRect( 0, &out, NULL, 0 );
}


void cTexture::Unlock()
{
	m_texture->UnlockRect( 0 );
}
