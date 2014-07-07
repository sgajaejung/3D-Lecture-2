#pragma once
#include "afxcmn.h"


// CMainPanelTab ��ȭ �����Դϴ�.
class CModelPanel;
class CAnimationPanel;
class CMainPanelTab : public CDialogEx
{
public:
	CMainPanelTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainPanelTab();
	enum { IDD = IDD_DIALOG_MAINPANEL };


protected:
	CModelPanel *m_modelPanel;
	CAnimationPanel *m_aniPanel;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;
};
