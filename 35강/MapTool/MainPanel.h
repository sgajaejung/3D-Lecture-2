#pragma once


// CMainPanel �������Դϴ�.
class CMainPanelTab;
class CMainPanel : public CMiniFrameWnd
{
public:
	CMainPanel();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMainPanel();

	void Init();


protected:
	CMainPanelTab *m_mainPanel;


protected:
	DECLARE_MESSAGE_MAP()
};


