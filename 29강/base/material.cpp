
#include "base.h"
#include "material.h"

using namespace graphic;


cMaterial::cMaterial()
{
	InitRed();

}

cMaterial::~cMaterial()
{

}


// 메터리얼 초기화.
void cMaterial::Init(const Vector4 &ambient, 
	const Vector4 &diffuse,
	const Vector4 &specular,
	const Vector4 &emmisive,
	const float pow)
{
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_mtrl.Ambient = *(D3DXCOLOR*)&ambient;
	m_mtrl.Diffuse = *(D3DXCOLOR*)&diffuse;
	m_mtrl.Specular = *(D3DXCOLOR*)&specular;
	m_mtrl.Emissive = *(D3DXCOLOR*)&emmisive;
	m_mtrl.Power = 0.f;
}


void cMaterial::InitWhite()
{
	Init(Vector4(1,1,1,1), Vector4(1,1,1,1), Vector4(1,1,1,1));
}


void cMaterial::InitRed()
{
	Init(Vector4(1,0,0,1), Vector4(1,0,0,1), Vector4(1,0,0,1));
}


void cMaterial::InitBlue()
{
	Init(Vector4(0,0,1,1), Vector4(0,0,1,1), Vector4(0,0,1,1));
}


void cMaterial::Bind()
{
	graphic::GetDevice()->SetMaterial(&m_mtrl);
}
