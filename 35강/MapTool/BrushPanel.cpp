// BrushPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "BrushPanel.h"
#include "afxdialogex.h"


// CBrushPanel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBrushPanel, CDialogEx)

CBrushPanel::CBrushPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBrushPanel::IDD, pParent)
{

}

CBrushPanel::~CBrushPanel()
{
}

void CBrushPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBrushPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBrushPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBrushPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CBrushPanel 메시지 처리기입니다.


void CBrushPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CBrushPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
