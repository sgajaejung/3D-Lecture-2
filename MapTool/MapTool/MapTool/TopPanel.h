#pragma once


// CTopPanel �������Դϴ�.
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

