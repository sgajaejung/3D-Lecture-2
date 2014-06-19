
// ViewerDlg.h : ��� ����
//

#pragma once

class CModelView;
// CViewerDlg ��ȭ ����
class CViewerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CViewerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	void MainProc();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	bool m_bLoop;
	CModelView *m_pModelView;

	// ������ �޽��� �� �Լ�
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
};
