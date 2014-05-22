
// VectorCalcDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

class CDotProductView;
class CDotControlDlg;
class CCrossControlDlg;
class CCrossProductView;

class CVectorCalcDlg : public CDialogEx
{
// 생성입니다.
public:
	CVectorCalcDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VECTORCALC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CDotProductView *m_pDotView;
	CDotControlDlg *m_pDotControl;

	CCrossProductView *m_pCrossView;
	CCrossControlDlg *m_pCrossControl;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CTabCtrl m_TabCtrl;
	afx_msg void OnSelchangeTabCalc(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnDestroy();
};
