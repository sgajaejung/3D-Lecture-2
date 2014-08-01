// MainPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanel.h"
#include "afxdialogex.h"

CMainPanel *g_mainPanel = NULL;


// CMainPanel ��ȭ �����Դϴ�.
CMainPanel::CMainPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanel::IDD, pParent)
{
	g_mainPanel = this;
}

CMainPanel::~CMainPanel()
{
}

void CMainPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CUBE, m_CubeList);
}


BEGIN_MESSAGE_MAP(CMainPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMainPanel �޽��� ó�����Դϴ�.


void CMainPanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}


void CMainPanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}
