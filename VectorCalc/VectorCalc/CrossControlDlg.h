// CCrossControlDlg ��ȭ �����Դϴ�.
#pragma once


class CCrossProductView;
class CCrossControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCrossControlDlg)

public:
	CCrossControlDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCrossControlDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CROSS_CONTROL };
	
	void SetCrossProductView(CCrossProductView *view) { m_pCrossView = view; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


protected:
	CCrossProductView *m_pCrossView;
	CString m_x1, m_y1, m_z1;
	CString m_x2, m_y2, m_z2;
	CString m_x3, m_y3, m_z3;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnChangeEdit();
};
