#pragma once


// CTopPanel �������Դϴ�.
class CTopPanelTab;
class CTopPanel : public CMiniFrameWnd
{
public:
	CTopPanel();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CTopPanel();

	void Init();

protected:
	CTopPanelTab *m_mainPanel;


protected:
	DECLARE_MESSAGE_MAP()
};


