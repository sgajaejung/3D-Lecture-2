// TopPanelTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TopPanelTab.h"
#include "afxdialogex.h"
#include "TerrainPanel.h"
#include "BrushPanel.h"



// CTopPanelTab ��ȭ �����Դϴ�.

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


// CTopPanelTab �޽��� ó�����Դϴ�.


void CTopPanelTab::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnOK();
}


void CTopPanelTab::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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
