// BrushPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "BrushPanel.h"
#include "afxdialogex.h"


// CBrushPanel ��ȭ �����Դϴ�.

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


// CBrushPanel �޽��� ó�����Դϴ�.


void CBrushPanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}


void CBrushPanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}
