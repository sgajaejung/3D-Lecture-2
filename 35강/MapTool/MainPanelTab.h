#pragma once


// CMainPanelTab ��ȭ �����Դϴ�.

class CMainPanelTab : public CDialogEx
{
public:
	CMainPanelTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainPanelTab();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MAINPANELTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
