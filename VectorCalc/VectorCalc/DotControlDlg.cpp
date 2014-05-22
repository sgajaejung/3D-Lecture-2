// DotControlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "VectorCalc.h"
#include "DotControlDlg.h"
#include "afxdialogex.h"
#include "DotProductView.h"


// CDotControlDlg 대화 상자입니다.

CDotControlDlg::CDotControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDotControlDlg::IDD, pParent)
	, m_x1("1"), m_y1("0"), m_z1("1")
	, m_x2("-1"), m_y2("0"), m_z2("1")
	, m_pDotView(NULL)
	, m_result(_T("0"))
{
}

CDotControlDlg::~CDotControlDlg()
{
}

void CDotControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_x1);
	DDX_Text(pDX, IDC_EDIT2, m_y1);
	DDX_Text(pDX, IDC_EDIT3, m_z1);
	DDX_Text(pDX, IDC_EDIT4, m_x2);
	DDX_Text(pDX, IDC_EDIT5, m_y2);
	DDX_Text(pDX, IDC_EDIT6, m_z2);
	DDX_Text(pDX, IDC_STATIC_DOT, m_result);
}


BEGIN_MESSAGE_MAP(CDotControlDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDotControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDotControlDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CDotControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CDotControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CDotControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT4, &CDotControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &CDotControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT6, &CDotControlDlg::OnChangeEdit)
END_MESSAGE_MAP()


// CDotControlDlg 메시지 처리기입니다.
void CDotControlDlg::OnBnClickedOk()
{
}


void CDotControlDlg::OnBnClickedCancel()
{

}


void CDotControlDlg::OnChangeEdit()
{
	UpdateData();
	Vector3 v1((float)atof(m_x1.GetBuffer(0)), (float)atof(m_y1.GetBuffer(0)), (float)atof(m_z1.GetBuffer(0)));
	Vector3 v2((float)atof(m_x2.GetBuffer(0)), (float)atof(m_y2.GetBuffer(0)), (float)atof(m_z2.GetBuffer(0)));

	v1.Normalize();
	v2.Normalize();

	if (m_pDotView)
		m_pDotView->UpdateVector(v1, v2);

	const float dot = v1.DotProduct(v2);
	m_result.Format("%f", dot);
	UpdateData(FALSE);
}
