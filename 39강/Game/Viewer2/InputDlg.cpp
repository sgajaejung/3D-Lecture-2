// InputDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "InputDlg.h"
#include "afxdialogex.h"


// CInputDlg ��ȭ �����Դϴ�.

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDlg::IDD, pParent)
	, m_label(_T(""))
	, m_value(_T(""))
{

}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_label);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_value);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputDlg �޽��� ó�����Դϴ�.
