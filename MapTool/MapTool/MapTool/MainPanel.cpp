// MainPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanel.h"
#include "afxdialogex.h"

CMainPanel *g_mainPanel = NULL;


// CMainPanel 대화 상자입니다.
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


// CMainPanel 메시지 처리기입니다.


void CMainPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CMainPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
