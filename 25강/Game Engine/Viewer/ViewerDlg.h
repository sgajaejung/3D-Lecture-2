
// ViewerDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

class CModelView;
// CViewerDlg 대화 상자
class CViewerDlg : public CDialogEx
{
// 생성입니다.
public:
	CViewerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	void MainProc();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	bool m_bLoop;
	CModelView *m_pModelView;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_FileList;
	afx_msg void OnItemchangedFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
