#pragma once
#include "afxwin.h"


// CMainPanel ��ȭ �����Դϴ�.

class CMainPanel : public CDialogEx
{
public:
	CMainPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainPanel();

	void InsertCube( graphic::cCube &cube );




// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_CubeList;
	afx_msg void OnSelchangeListCube();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
};
