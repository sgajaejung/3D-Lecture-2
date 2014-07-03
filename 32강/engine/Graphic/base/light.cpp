
#include "stdafx.h"
#include "light.h"

using namespace graphic;

cLight::cLight()
{

}


cLight::~cLight()
{

}


void cLight::Init(TYPE type, 
	const Vector4 &ambient,
	const Vector4 &diffuse,
	const Vector4 &specular,
	const Vector3 &direction)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = (D3DLIGHTTYPE)type;
	m_light.Ambient = *(D3DCOLORVALUE*)&ambient;
	m_light.Diffuse = *(D3DCOLORVALUE*)&diffuse;
	m_light.Specular = *(D3DCOLORVALUE*)&specular;
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}


void cLight::Bind(int lightIndex)
{
	GetDevice()->SetLight(lightIndex, &m_light); // ±¤¿ø ¼³Á¤.
}


void cLight::SetDirection( const Vector3 &direction )
{
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}
