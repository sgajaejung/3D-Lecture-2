#pragma once


// CMainPanelTab 대화 상자입니다.

class CMainPanelTab : public CDialogEx
{
public:
	CMainPanelTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMainPanelTab();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAINPANELTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
