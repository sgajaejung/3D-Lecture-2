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
	DECLARE_MESSAGE_MAP()
	CMainPanelTab *m_mainPanel;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
