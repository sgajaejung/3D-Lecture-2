#pragma once


// CTopPanel 프레임입니다.
class CMainPanel;
class CTopPanel : public CMiniFrameWnd
{
public:
	CTopPanel();
	virtual ~CTopPanel();

	void Init();
	CMainPanel *m_mainPanel;

protected:
	DECLARE_MESSAGE_MAP()
};

