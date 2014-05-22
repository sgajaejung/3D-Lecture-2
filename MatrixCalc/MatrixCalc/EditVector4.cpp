// EditVector4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MatrixCalc.h"
#include "EditVector4.h"
#include "afxdialogex.h"
#include "math/math.h"


// CEditVector4 대화 상자입니다.

//IMPLEMENT_DYNAMIC(CEditVector4, CDialogEx)

CEditVector4::CEditVector4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditVector4::IDD, pParent)
	, m_x(0), m_y(0), m_z(0), m_w(1)
{

}

CEditVector4::~CEditVector4()
{
}

void CEditVector4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDX_Text(pDX, IDC_EDIT3, m_z);
	DDX_Text(pDX, IDC_EDIT4, m_w);
}


BEGIN_MESSAGE_MAP(CEditVector4, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditVector4::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditVector4::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEditVector4 메시지 처리기입니다.
Vector4 CEditVector4::GetVector4()
{
	return Vector4(m_x, m_y, m_z, m_w);
}


void CEditVector4::SetVector4(const Vector4 &v)
{
	m_x = v.x;
	m_y = v.y;
	m_z = v.z;
	m_w = v.w;
	UpdateData(FALSE);
}


void CEditVector4::OnBnClickedOk()
{
}


void CEditVector4::OnBnClickedCancel()
{
}
