// TopPanelTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TopPanelTab.h"
#include "afxdialogex.h"
#include "TerrainPanel.h"
#include "BrushPanel.h"



// CTopPanelTab 대화 상자입니다.

CTopPanelTab::CTopPanelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTopPanelTab::IDD, pParent)
{

}

CTopPanelTab::~CTopPanelTab()
{
}

void CTopPanelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PANEL, m_Tab);
}


BEGIN_MESSAGE_MAP(CTopPanelTab, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTopPanelTab::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTopPanelTab::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PANEL, &CTopPanelTab::OnSelchangeTabPanel)
END_MESSAGE_MAP()


// CTopPanelTab 메시지 처리기입니다.


void CTopPanelTab::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnOK();
}


void CTopPanelTab::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnCancel();
}


BOOL CTopPanelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Tab.InsertItem(0, L"Terrain1");
	m_Tab.InsertItem(1, L"Terrain2");
	m_Tab.InsertItem(2, L"Terrain3");


	CRect cr;
	GetClientRect(cr);

	m_terrainPanel = new CTerrainPanel(this);
	m_terrainPanel->Create( CTerrainPanel::IDD, this);
	m_terrainPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_terrainPanel->ShowWindow(SW_SHOW);


	m_brushPanel = new CBrushPanel(this);
	m_brushPanel->Create( CBrushPanel::IDD, this);
	m_brushPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	//m_brushPanel->ShowWindow(SW_SHOW);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTopPanelTab::OnSelchangeTabPanel(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_Tab.GetCurSel())
	{
	case 0:
		m_terrainPanel->ShowWindow(SW_SHOW);
		m_brushPanel->ShowWindow(SW_HIDE);
		break;

	case 1:
		m_terrainPanel->ShowWindow(SW_HIDE);
		m_brushPanel->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}
