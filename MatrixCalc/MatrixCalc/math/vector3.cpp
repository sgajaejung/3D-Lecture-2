
#include "stdafx.h"
#include "math.h"


float Vector3::Length() const
{
	return sqrt(x*x + y*y + z*z);
}


Vector3 Vector3::Normal() const
{
	const float len = Length();
	if (len < 0.001f)
		return Vector3(0,0,0);
	return *this / len;
}


void Vector3::Normalize()
{
	*this = Normal();
}


Vector3 Vector3::operator + () const
{
	return *this;
}


Vector3 Vector3::operator - () const
{
	return Vector3(-x, -y, -z);
}


Vector3 Vector3::operator + ( const Vector3& rhs ) const
{
	return Vector3(x+rhs.x, y+rhs.y, z+rhs.z);
}


Vector3 Vector3::operator - ( const Vector3& rhs ) const
{
	return Vector3(x-rhs.x, y-rhs.y, z-rhs.z);
}


Vector3& Vector3::operator += ( const Vector3& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}


Vector3& Vector3::operator -= ( const Vector3& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}


Vector3& Vector3::operator *= ( const Vector3& rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}


Vector3& Vector3::operator /= ( const Vector3& rhs )
{
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}


Vector3 Vector3::operator * ( const Matrix44& rhs ) const
{
	float	RHW = 1.0f / (x * rhs._14 + y * rhs._24 + z * rhs._34 + rhs._44);
	if (RHW >= FLT_MAX)
		return Vector3(0,0,0);

	Vector3 v;
	v.x = (x * rhs._11 + y * rhs._21 + z * rhs._31 + rhs._41 ) * RHW;
	v.y = (x * rhs._12 + y * rhs._22 + z * rhs._32 + rhs._42 ) * RHW;
	v.z = (x * rhs._13 + y * rhs._23 + z * rhs._33 + rhs._43 ) * RHW;
	return v;
}


Vector3& Vector3::operator *= ( Matrix44& rhs )
{
	float	RHW = 1.0f / (x * rhs._14 + y * rhs._24 + z * rhs._34 + rhs._44);
	if (RHW >= FLT_MAX)
	{
		*this = Vector3(0,0,0);
		return *this;
	}

	Vector3 v;
	v.x = (x * rhs._11 + y * rhs._21 + z * rhs._31 + rhs._41 ) * RHW;
	v.y = (x * rhs._12 + y * rhs._22 + z * rhs._32 + rhs._42 ) * RHW;
	v.z = (x * rhs._13 + y * rhs._23 + z * rhs._33 + rhs._43 ) * RHW;
	*this = v;
	return *this;
}
