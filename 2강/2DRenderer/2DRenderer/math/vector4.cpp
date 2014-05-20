
#include "stdafx.h"
#include "math.h"


Vector4 Vector4::operator * ( const Matrix44& rhs ) const
{
	Vector4 v;
	v.x = x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41;
	v.y = x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42;
	v.z = x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43;
	v.w = x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44;
	return v;
}


Vector4& Vector4::operator *= ( Matrix44& rhs )
{
	Vector4 v;
	v.x = x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41;
	v.y = x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42;
	v.z = x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43;
	v.w = x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44;
	*this = v;
	return *this;
}
