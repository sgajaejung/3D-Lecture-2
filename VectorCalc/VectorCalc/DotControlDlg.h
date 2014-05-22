#pragma once


// CDotControlDlg 대화 상자입니다.
class CDotProductView;
class CDotControlDlg : public CDialogEx
{
public:
	CDotControlDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDotControlDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DOT_CONTROL };

	void SetDotProductView(CDotProductView *view) { m_pDotView = view; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	CDotProductView *m_pDotView; // reference
	CString m_x1, m_y1, m_z1;
	CString m_x2, m_y2, m_z2;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnChangeEdit();
	CString m_result;
};
