#pragma once

#include "math/math.h"


class CommonDataInterface
{
public:
	virtual void SetIdentity() {}
	virtual Vector3 GetVecto3r() { return Vector3();}
	virtual Vector4 GetVector4() { return Vector4();}
	virtual Matrix44 GetMatrix() { return Matrix44();}
	virtual void SetVector3(const Vector3 &v) {}
	virtual void SetVector4(const Vector4 &v) {}
	virtual void SetMatrix(const Matrix44 &m) {}

};
