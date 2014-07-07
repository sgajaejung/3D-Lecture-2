// MainPanelTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "MainPanelTab.h"
#include "afxdialogex.h"
#include "ModelPanel.h"
#include "AnimationPanel.h"


// CMainPanelTab 대화 상자입니다.

CMainPanelTab::CMainPanelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanelTab::IDD, pParent)
,	m_modelPanel(NULL)
,	m_aniPanel(NULL)
{

}

CMainPanelTab::~CMainPanelTab()
{
}

void CMainPanelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CMainPanelTab, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainPanelTab::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainPanelTab::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainPanelTab::OnSelchangeTab)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainPanelTab 메시지 처리기입니다.


void CMainPanelTab::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CMainPanelTab::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


BOOL CMainPanelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Tab.InsertItem(0, L"Model");
	m_Tab.InsertItem(1, L"Animation");

	CRect cr;
	GetClientRect(cr);

	m_modelPanel = new CModelPanel(this);
	m_modelPanel->Create(CModelPanel::IDD, this);
	m_modelPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_modelPanel->ShowWindow(SW_SHOW);

	m_aniPanel = new CAnimationPanel(this);
	m_aniPanel->Create(CAnimationPanel::IDD, this);
	m_aniPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_aniPanel->ShowWindow(SW_HIDE);


	cController::Get()->AddObserver(m_modelPanel);
	cController::Get()->AddObserver(m_aniPanel);

	return TRUE;
}


void CMainPanelTab::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_Tab.GetCurSel())
	{
	case 0:
		m_modelPanel->ShowWindow(SW_SHOW);
		m_aniPanel->ShowWindow(SW_HIDE);
		break;

	case 1:
		m_aniPanel->ShowWindow(SW_SHOW);
		m_modelPanel->ShowWindow(SW_HIDE);
		break;
	}

	*pResult = 0;
}


void CMainPanelTab::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_Tab.GetSafeHwnd())
	{
		m_Tab.MoveWindow(0, 0, cx, cy);
		m_modelPanel->MoveWindow(CRect(0, 25, cx, cy));
		m_aniPanel->MoveWindow(CRect(0, 25, cx, cy));
	}
}
