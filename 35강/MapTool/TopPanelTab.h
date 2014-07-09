#pragma once
#include "afxcmn.h"


// CTopPanelTab 대화 상자입니다.
class CTerrainPanel;
class CBrushPanel;
class CTopPanelTab : public CDialogEx
{
public:
	CTopPanelTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTopPanelTab();
	enum { IDD = IDD_TOPPANELTAB };


protected:
	CTerrainPanel *m_terrainPanel;
	CBrushPanel *m_brushPanel;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;
	afx_msg void OnSelchangeTabPanel(NMHDR *pNMHDR, LRESULT *pResult);
};
