#pragma once


// CBrushPanel ��ȭ �����Դϴ�.

class CBrushPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CBrushPanel)

public:
	CBrushPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBrushPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_BRUSH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
