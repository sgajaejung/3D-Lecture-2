#pragma once
#include "afxcmn.h"


// CTopPanelTab ��ȭ �����Դϴ�.
class CTerrainPanel;
class CBrushPanel;
class CTopPanelTab : public CDialogEx
{
public:
	CTopPanelTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTopPanelTab();
	enum { IDD = IDD_TOPPANELTAB };


protected:
	CTerrainPanel *m_terrainPanel;
	CBrushPanel *m_brushPanel;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;
	afx_msg void OnSelchangeTabPanel(NMHDR *pNMHDR, LRESULT *pResult);
};
