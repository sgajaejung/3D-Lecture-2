#include "stdafx.h"
#include "rawmesh.h"

using namespace graphic;


sMaterial::sMaterial(const sMaterial &rhs)
{
	operator=(rhs);
}


sMaterial& sMaterial::operator=(const sMaterial &rhs)
{
	if (this != &rhs)
	{
		ambient = rhs.ambient;
		diffuse = rhs.diffuse;
		specular = rhs.specular;
		emissive = rhs.emissive;
		power = rhs.power;
		texture = rhs.texture;
	}
	return *this;
}

