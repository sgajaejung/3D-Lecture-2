// MainPanelTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanelTab.h"
#include "afxdialogex.h"


// CMainPanelTab 대화 상자입니다.
CMainPanelTab::CMainPanelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanelTab::IDD, pParent)
{

}

CMainPanelTab::~CMainPanelTab()
{
}

void CMainPanelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainPanelTab, CDialogEx)
END_MESSAGE_MAP()


// CMainPanelTab 메시지 처리기입니다.
