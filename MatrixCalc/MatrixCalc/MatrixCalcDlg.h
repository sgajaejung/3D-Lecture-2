
// MatrixCalcDlg.h : ��� ����
//

#pragma once

class CDlgCalc4x4;


// CMatrixCalcDlg ��ȭ ����
class CMatrixCalcDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMatrixCalcDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MATRIXCALC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	CDlgCalc4x4 *m_pDlgCalc4x4;


	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
