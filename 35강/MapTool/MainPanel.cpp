// MainPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanel.h"
#include "MainPanelTab.h"


// CMainPanel

CMainPanel::CMainPanel()
{

}

CMainPanel::~CMainPanel()
{
}


BEGIN_MESSAGE_MAP(CMainPanel, CMiniFrameWnd)
END_MESSAGE_MAP()


// CMainPanel 메시지 처리기입니다.
void CMainPanel::Init()
{
	CRect cr;
	GetClientRect(cr);

	m_mainPanel = new CMainPanelTab(this);
	m_mainPanel->Create( CMainPanelTab::IDD, this );
	m_mainPanel->MoveWindow(cr);
	m_mainPanel->ShowWindow(SW_SHOW);

}
