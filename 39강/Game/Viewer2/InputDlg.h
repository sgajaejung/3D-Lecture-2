#pragma once


// CInputDlg ��ȭ �����Դϴ�.

class CInputDlg : public CDialogEx
{
public:
	CInputDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_label;
	CString m_value;
};
