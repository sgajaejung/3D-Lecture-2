
#include "math.h"


bool Vector2::IsEmpty() const
{
	return (x==0) && (y==0);
}

float Vector2::Length() const
{
	return sqrt(x*x + y*y);
}


Vector2 Vector2::Normal() const
{
	const float len = Length();
	if (len < 0.001f)
		return Vector2(0,0);
	return *this / len;
}


void Vector2::Normalize()
{
	*this = Normal();
}


Vector2 Vector2::operator + () const
{
	return *this;
}


Vector2 Vector2::operator - () const
{
	return Vector2(-x, -y);
}


Vector2 Vector2::operator + ( const Vector2& rhs ) const
{
	return Vector2(x+rhs.x, y+rhs.y);
}


Vector2 Vector2::operator - ( const Vector2& rhs ) const
{
	return Vector2(x-rhs.x, y-rhs.y);
}


Vector2& Vector2::operator += ( const Vector2& rhs )
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}


Vector2& Vector2::operator -= ( const Vector2& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}


Vector2& Vector2::operator *= ( const Vector2& rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}


Vector2& Vector2::operator /= ( const Vector2& rhs )
{
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}


Vector2 Vector2::operator * ( const Matrix44& rhs ) const
{
	float	RHW = 1.0f / (x*rhs._13 + y*rhs._23 + rhs._33);
	if (RHW >= FLT_MAX)
		return Vector2(0,0);

	Vector2 v;
	v.x = (x*rhs._11 + y*rhs._21 + rhs._31) * RHW;
	v.y = (x*rhs._12 + y*rhs._22 + rhs._32) * RHW;
	return v;
}


Vector2& Vector2::operator *= ( Matrix44& rhs )
{
	float	RHW = 1.0f / (x * rhs._13 + y * rhs._23 + rhs._33);
	if (RHW >= FLT_MAX)
	{
		*this = Vector2(0,0);
		return *this;
	}

	Vector2 v;
	v.x = (x * rhs._11 + y * rhs._21 + rhs._31) * RHW;
	v.y = (x * rhs._12 + y * rhs._22 + rhs._32) * RHW;
	*this = v;
	return *this;
}