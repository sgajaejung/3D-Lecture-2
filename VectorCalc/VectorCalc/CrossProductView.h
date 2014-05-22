#pragma once

#include "3DView.h"


class CCrossProductView : public C3DView
{
public:
	CCrossProductView();
	virtual ~CCrossProductView();


public:
	void UpdateVector(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
	virtual void Render() override;


protected:
	Matrix44 m_matProjection;
	Matrix44 m_matView;
	Matrix44 m_matViewPort;
	vector<Vector3> m_axises;
	vector<Vector3> m_dir0;
	vector<Vector3> m_dir1;
	vector<Vector3> m_dir2;

	HPEN m_bluePen;
	HPEN m_redPen;
	HPEN m_greenPen;
	DECLARE_MESSAGE_MAP()
};
