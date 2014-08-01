#pragma once
#include "afxwin.h"


// CMainPanel 대화 상자입니다.

class CMainPanel : public CDialogEx
{
public:
	CMainPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMainPanel();

	void InsertCube( graphic::cCube &cube );




// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_CubeList;
	afx_msg void OnSelchangeListCube();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
};
