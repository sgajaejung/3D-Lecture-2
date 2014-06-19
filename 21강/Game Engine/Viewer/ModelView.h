#pragma once


// CModelView 뷰입니다.

class CModelView : public CView
{
public:
	CModelView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CModelView();

	void Render();

	CPoint m_mousePos;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
};
