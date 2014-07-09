// TopPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TopPanel.h"
#include "TopPanelTab.h"


// CTopPanel

CTopPanel::CTopPanel()
{

}

CTopPanel::~CTopPanel()
{
}


BEGIN_MESSAGE_MAP(CTopPanel, CMiniFrameWnd)
END_MESSAGE_MAP()


// CTopPanel 메시지 처리기입니다.
void CTopPanel::Init()
{
	CRect cr;
	GetClientRect(cr);

	m_mainPanel = new CTopPanelTab(this);
	m_mainPanel->Create( CTopPanelTab::IDD, this );
	m_mainPanel->MoveWindow(cr);
	m_mainPanel->ShowWindow(SW_SHOW);
}
