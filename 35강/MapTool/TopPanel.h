#pragma once


// CTopPanel 프레임입니다.
class CTopPanelTab;
class CTopPanel : public CMiniFrameWnd
{
public:
	CTopPanel();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTopPanel();

	void Init();

protected:
	CTopPanelTab *m_mainPanel;


protected:
	DECLARE_MESSAGE_MAP()
};


