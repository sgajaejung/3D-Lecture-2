
// MfcControlDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMfcControlDlg ��ȭ ����
class CMfcControlDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMfcControlDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFCCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_Check;
	afx_msg void OnBnClickedCheck1();
	CString m_Static1;
	int m_EditNum;
	CComboBox m_Combo;
	afx_msg void OnSelchangeCombo1();
	CListBox m_ListBox;
	afx_msg void OnSelchangeList1();
	CStatic m_Picture;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	BOOL m_Radio1;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	CSpinButtonCtrl m_Spin;
	CSliderCtrl m_Slider;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_Tree;
	afx_msg void OnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_Tab;
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
