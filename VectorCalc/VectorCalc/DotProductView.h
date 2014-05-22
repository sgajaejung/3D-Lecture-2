#pragma once

#include "3DView.h"


class CDotProductView : public C3DView
{
public:
	CDotProductView();
	virtual ~CDotProductView();


public:
	void UpdateVector(const Vector3 &v1, const Vector3 &v2);
	virtual void Render() override;


protected:
	Matrix44 m_matProjection;
	Matrix44 m_matView;
	Matrix44 m_matViewPort;
	vector<Vector3> m_axises;
	vector<Vector3> m_dir0;
	vector<Vector3> m_dir1;

	HPEN m_bluePen;
	HPEN m_redPen;
	DECLARE_MESSAGE_MAP()
};
