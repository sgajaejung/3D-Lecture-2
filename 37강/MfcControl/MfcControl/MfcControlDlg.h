
// MfcControlDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMfcControlDlg 대화 상자
class CMfcControlDlg : public CDialogEx
{
// 생성입니다.
public:
	CMfcControlDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MFCCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
