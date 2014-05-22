// CrossControlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "VectorCalc.h"
#include "CrossControlDlg.h"
#include "afxdialogex.h"
#include "CrossProductView.h"


// CCrossControlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCrossControlDlg, CDialogEx)

CCrossControlDlg::CCrossControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCrossControlDlg::IDD, pParent)
	, m_x1("-1"), m_y1("0"), m_z1("1")
	, m_x2("1"), m_y2("0"), m_z2("1")
	, m_x3("0"), m_y3("1"), m_z3("0")
	,	m_pCrossView(NULL)
{

}

CCrossControlDlg::~CCrossControlDlg()
{
}

void CCrossControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_x1);
	DDX_Text(pDX, IDC_EDIT2, m_y1);
	DDX_Text(pDX, IDC_EDIT3, m_z1);
	DDX_Text(pDX, IDC_EDIT4, m_x2);
	DDX_Text(pDX, IDC_EDIT5, m_y2);
	DDX_Text(pDX, IDC_EDIT6, m_z2);
	DDX_Text(pDX, IDC_EDIT7, m_x3);
	DDX_Text(pDX, IDC_EDIT8, m_y3);
	DDX_Text(pDX, IDC_EDIT9, m_z3);
}


BEGIN_MESSAGE_MAP(CCrossControlDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCrossControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCrossControlDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CCrossControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CCrossControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CCrossControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT4, &CCrossControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &CCrossControlDlg::OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT6, &CCrossControlDlg::OnChangeEdit)
END_MESSAGE_MAP()


// CCrossControlDlg 메시지 처리기입니다.
void CCrossControlDlg::OnBnClickedOk()
{
}


void CCrossControlDlg::OnBnClickedCancel()
{
}


void CCrossControlDlg::OnChangeEdit()
{
	UpdateData();
	Vector3 v1((float)atof(m_x1.GetBuffer(0)), (float)atof(m_y1.GetBuffer(0)), (float)atof(m_z1.GetBuffer(0)));
	Vector3 v2((float)atof(m_x2.GetBuffer(0)), (float)atof(m_y2.GetBuffer(0)), (float)atof(m_z2.GetBuffer(0)));
	Vector3 v3 = v1.Normal().CrossProduct(v2.Normal());

	v1.Normalize();
	v2.Normalize();
	v3.Normalize();
	if (m_pCrossView)
		m_pCrossView->UpdateVector(v1, v2, v3);

	m_x3.Format("%f", v3.x);
	m_y3.Format("%f", v3.y);
	m_z3.Format("%f", v3.z);
	UpdateData(FALSE);
}
