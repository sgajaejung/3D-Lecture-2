#pragma once


class CMapView : public CView
{
public:
	CMapView();
	virtual ~CMapView();

	bool Init();
	void Render();
	void Update(float elapseT);
	void UpdateCamera();


public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	bool m_dxInit;
	string m_filePath;
	Matrix44 m_rotateTm;
	Vector3 m_camPos;
	Vector3 m_lookAtPos;
	Matrix44 m_matView;
	Matrix44 m_matProj;

	bool m_LButtonDown;
	bool m_RButtonDown;
	bool m_MButtonDown;
	CPoint m_curPos;
	graphic::cGrid m_grid;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
