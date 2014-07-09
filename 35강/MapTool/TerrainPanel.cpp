// TerrainPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainPanel.h"
#include "afxdialogex.h"


// CTerrainPanel 대화 상자입니다.
CTerrainPanel::CTerrainPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTerrainPanel::IDD, pParent)
{

}

CTerrainPanel::~CTerrainPanel()
{
}

void CTerrainPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTerrainPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTerrainPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTerrainPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTerrainPanel 메시지 처리기입니다.


void CTerrainPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void CTerrainPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}
