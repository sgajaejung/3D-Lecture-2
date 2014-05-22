// CCrossControlDlg 대화 상자입니다.
#pragma once


class CCrossProductView;
class CCrossControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCrossControlDlg)

public:
	CCrossControlDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCrossControlDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CROSS_CONTROL };
	
	void SetCrossProductView(CCrossProductView *view) { m_pCrossView = view; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
