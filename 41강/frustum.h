#pragma once

#include "global.h"


class CFrustum
{
public:
	CFrustum() {}
	virtual ~CFrustum() {}

protected:
	Vector3 m_Vtx[ 8];		// frustum ���� ��ǥ
	Vector3 m_vPos;			// ī�޶� ��ġ
	Plane	m_Plane[ 6];	// frustum�� 6�� ���

public:
	// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
	BOOL Make( Matrix44* pmatViewProj );
	// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	BOOL IsIn( Vector3* pv );
	// �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
	// TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	BOOL IsInSphere( Vector3* pv, float radius );
	void Render();
	Vector3* GetPos() { return &m_vPos; }

};

