// DlgCalc4x4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MatrixCalc.h"
#include "DlgCalc4x4.h"
#include "afxdialogex.h"
#include "EditMatrix4x4.h"
#include "EditVector4.h"


// CDlgCalc4x4 대화 상자입니다.
CDlgCalc4x4::CDlgCalc4x4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalc4x4::IDD, pParent)
	, m_Angle1(0)
	, m_Angle2(0)
{

}

CDlgCalc4x4::~CDlgCalc4x4()
{
}

void CDlgCalc4x4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FIRST, m_ComboFirst);
	DDX_Control(pDX, IDC_COMBO_SECOND, m_ComboSecond);
	DDX_Text(pDX, IDC_EDIT_ANGLE1, m_Angle1);
	DDX_Text(pDX, IDC_EDIT_ANGLE2, m_Angle2);
}


BEGIN_MESSAGE_MAP(CDlgCalc4x4, CDialogEx)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_FIRST, &CDlgCalc4x4::OnCbnSelchangeComboFirst)
	ON_CBN_SELCHANGE(IDC_COMBO_SECOND, &CDlgCalc4x4::OnCbnSelchangeComboSecond)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CDlgCalc4x4::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDCANCEL, &CDlgCalc4x4::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgCalc4x4::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_ANGLE1, &CDlgCalc4x4::OnEnChangeEditAngle1)
	ON_EN_CHANGE(IDC_EDIT_ANGLE2, &CDlgCalc4x4::OnEnChangeEditAngle2)
END_MESSAGE_MAP()


// CDlgCalc4x4 메시지 처리기입니다.
BOOL CDlgCalc4x4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	const int WIDTH = 230;
	const int HEIGHT = 120;

	const int RESULT_X = 190;
	const int RESULT_Y = 180;
	CRect rr(RESULT_X, RESULT_Y, RESULT_X+WIDTH, RESULT_Y+HEIGHT);

	m_pMatrixResult = new CEditMatrix4x4(this);
	m_pMatrixResult->Create(CEditMatrix4x4::IDD, this);
	m_pMatrixResult->MoveWindow(rr);
	m_pMatrixResult->ShowWindow(SW_SHOW);

	m_pVectorResult = new CEditVector4(this);
	m_pVectorResult->Create(CEditVector4::IDD, this);
	m_pVectorResult->MoveWindow(rr);


	//---------------------------------------------------------------------------------
	// First 
	m_ComboFirst.AddString("Matrix44");
	m_ComboFirst.AddString("Matrix44 Identity");
	m_ComboFirst.AddString("Matrix44 Rotate X");
	m_ComboFirst.AddString("Matrix44 Rotate Y");
	m_ComboFirst.AddString("Matrix44 Rotate Z");
	m_ComboFirst.AddString("Matrix44 Scale");
	m_ComboFirst.AddString("Vector4");
	m_ComboFirst.SetCurSel(0);

	{
		const int X = 10;
		const int Y = 40;
		CRect r(X, Y, X+WIDTH, Y+HEIGHT);

		CEditMatrix4x4 *pEditMat44 = new CEditMatrix4x4(this);
		pEditMat44->Create(CEditMatrix4x4::IDD, this);
		pEditMat44->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_2 = new CEditMatrix4x4(this);
		pEditMat44_2->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_2->MoveWindow(r);
		pEditMat44_2->SetIdentity();

		CEditMatrix4x4 *pEditMat44_3 = new CEditMatrix4x4(this);
		pEditMat44_3->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_3->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_4 = new CEditMatrix4x4(this);
		pEditMat44_4->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_4->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_5 = new CEditMatrix4x4(this);
		pEditMat44_5->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_5->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_6 = new CEditMatrix4x4(this);
		pEditMat44_6->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_6->MoveWindow(r);

		CEditVector4 *pEditVector4 = new CEditVector4(this);
		pEditVector4->Create(CEditVector4::IDD, this);
		pEditVector4->MoveWindow(r);

		m_First.resize(VAR1_MAX);
		m_First[ VAR1_MATRIX] = pEditMat44;
		m_First[ VAR1_IDENTITY] = pEditMat44_2;
		m_First[ VAR1_MAT_ROTATEX] = pEditMat44_3;
		m_First[ VAR1_MAT_ROTATEY] = pEditMat44_4;
		m_First[ VAR1_MAT_ROTATEZ] = pEditMat44_5;
		m_First[ VAR1_MAT_SCALE] = pEditMat44_6;
		m_First[ VAR1_VECTOR4] = pEditVector4;

		m_First[0]->ShowWindow(SW_SHOW);
	}
	//---------------------------------------------------------------------------------


	//---------------------------------------------------------------------------------
	// Second
	m_ComboSecond.AddString("Matrix44");
	m_ComboSecond.AddString("Matrix44 Identity");
	m_ComboSecond.AddString("Matrix44 Rotate X");
	m_ComboSecond.AddString("Matrix44 Rotate Y");
	m_ComboSecond.AddString("Matrix44 Rotate Z");
	m_ComboSecond.AddString("Matrix44 Scale");
	m_ComboSecond.SetCurSel(0);

	{
		const int X = 380;
		const int Y = 40;
		CRect r(X, Y, X+WIDTH, Y+HEIGHT);

		CEditMatrix4x4 *pEditMat44 = new CEditMatrix4x4(this);
		pEditMat44->Create(CEditMatrix4x4::IDD, this);
		pEditMat44->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_2 = new CEditMatrix4x4(this);
		pEditMat44_2->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_2->MoveWindow(r);
		pEditMat44_2->SetIdentity();

		CEditMatrix4x4 *pEditMat44_3 = new CEditMatrix4x4(this);
		pEditMat44_3->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_3->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_4 = new CEditMatrix4x4(this);
		pEditMat44_4->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_4->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_5 = new CEditMatrix4x4(this);
		pEditMat44_5->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_5->MoveWindow(r);

		CEditMatrix4x4 *pEditMat44_6 = new CEditMatrix4x4(this);
		pEditMat44_6->Create(CEditMatrix4x4::IDD, this);
		pEditMat44_6->MoveWindow(r);

		m_Second.resize(VAR2_MAX);
		m_Second[ VAR2_MATRIX] = pEditMat44;
		m_Second[ VAR2_IDENTITY] = pEditMat44_2;
		m_Second[ VAR2_MAT_ROTATEX] = pEditMat44_3;
		m_Second[ VAR2_MAT_ROTATEY] = pEditMat44_4;
		m_Second[ VAR2_MAT_ROTATEZ] = pEditMat44_5;
		m_Second[ VAR2_MAT_SCALE] = pEditMat44_6;

		m_Second[0]->ShowWindow(SW_SHOW);
	}
	//---------------------------------------------------------------------------------

	GetDlgItem(IDC_EDIT_ANGLE1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ANGLE1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ANGLE2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ANGLE2)->ShowWindow(SW_HIDE);
	return TRUE;
}


void CDlgCalc4x4::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_pMatrixResult->DestroyWindow();
	delete m_pMatrixResult;

	m_pVectorResult->DestroyWindow();
	delete m_pVectorResult;

	for (int i=0; i < (int)m_First.size(); ++i)
	{
		m_First[ i]->DestroyWindow();
		delete m_First[ i];
	}
	m_First.clear();


	for (int i=0; i < (int)m_Second.size(); ++i)
	{
		m_Second[ i]->DestroyWindow();
		delete m_Second[ i];
	}
	m_Second.clear();
}


void CDlgCalc4x4::OnCbnSelchangeComboFirst()
{
	for (int i=0; i < (int)m_First.size(); ++i)
		m_First[ i]->ShowWindow(SW_HIDE);
	
	const FIRST_VARIABLE type = (FIRST_VARIABLE)m_ComboFirst.GetCurSel();
	if (VAR1_IDENTITY == type)
	{
		CommonDataInterface *dataInterface = dynamic_cast<CommonDataInterface *>(m_First[ m_ComboFirst.GetCurSel()]);
		if (NULL != dataInterface)
			dataInterface->SetIdentity();
	}

	switch (type)
	{
	case VAR1_MATRIX:
	case VAR1_IDENTITY:
	case VAR1_VECTOR4:
		GetDlgItem(IDC_EDIT_ANGLE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ANGLE1)->ShowWindow(SW_HIDE);
		break;

	case VAR1_MAT_ROTATEX:
	case VAR1_MAT_ROTATEY:
	case VAR1_MAT_ROTATEZ:
	case VAR1_MAT_SCALE:
		GetDlgItem(IDC_EDIT_ANGLE1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ANGLE1)->ShowWindow(SW_SHOW);
		break;
	}

	m_First[ m_ComboFirst.GetCurSel()]->ShowWindow(SW_SHOW);
}


void CDlgCalc4x4::OnCbnSelchangeComboSecond()
{
	for (int i=0; i < (int)m_Second.size(); ++i)
		m_Second[ i]->ShowWindow(SW_HIDE);

	const SECOND_VARIABLE type = (SECOND_VARIABLE)m_ComboSecond.GetCurSel();
	if (VAR2_IDENTITY == type)
	{
		CommonDataInterface *dataInterface = dynamic_cast<CommonDataInterface *>(m_Second[ m_ComboSecond.GetCurSel()]);
		if (NULL != dataInterface)
			dataInterface->SetIdentity();
	}

	switch (type)
	{
	case VAR2_MATRIX:
	case VAR2_IDENTITY:
		GetDlgItem(IDC_EDIT_ANGLE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ANGLE2)->ShowWindow(SW_HIDE);
		break;

	case VAR2_MAT_ROTATEX:
	case VAR2_MAT_ROTATEY:
	case VAR2_MAT_ROTATEZ:
	case VAR2_MAT_SCALE:
		GetDlgItem(IDC_EDIT_ANGLE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ANGLE2)->ShowWindow(SW_SHOW);
		break;
	}	

	m_Second[ m_ComboSecond.GetCurSel()]->ShowWindow(SW_SHOW);
}


void CDlgCalc4x4::OnBnClickedButtonCalc()
{
	FIRST_VARIABLE first = (FIRST_VARIABLE)m_ComboFirst.GetCurSel();
	SECOND_VARIABLE second = (SECOND_VARIABLE)m_ComboSecond.GetCurSel();
	CommonDataInterface *firstData = dynamic_cast<CommonDataInterface *>(m_First[ m_ComboFirst.GetCurSel()]);
	CommonDataInterface *secondData = dynamic_cast<CommonDataInterface *>(m_Second[ m_ComboSecond.GetCurSel()]);
	if (!firstData || !secondData)
		return;

	m_First[ m_ComboFirst.GetCurSel()]->UpdateData();
	m_Second[ m_ComboSecond.GetCurSel()]->UpdateData();

	switch (first)
	{
	case VAR1_MATRIX:
	case VAR1_IDENTITY:
	case VAR1_MAT_ROTATEX:
	case VAR1_MAT_ROTATEY:
	case VAR1_MAT_ROTATEZ:
	case VAR1_MAT_SCALE:
		{
			Matrix44 mat1 = firstData->GetMatrix();
			Matrix44 mat2 = secondData->GetMatrix();
			Matrix44 m = mat1 * mat2;
			m_pVectorResult->ShowWindow(SW_HIDE);
			m_pMatrixResult->ShowWindow(SW_SHOW);
			m_pMatrixResult->SetMatrix(m);
		}
		break;
		
	case VAR1_VECTOR4:
		{
			Vector4 p = firstData->GetVector4();
			Matrix44 mat2 = secondData->GetMatrix();
			Vector4 v = p * mat2;
			m_pVectorResult->ShowWindow(SW_SHOW);
			m_pMatrixResult->ShowWindow(SW_HIDE);
			m_pVectorResult->SetVector4(v);
		}
		break;
	}
}


void CDlgCalc4x4::OnEnChangeEditAngle1()
{
	UpdateData();

	FIRST_VARIABLE first = (FIRST_VARIABLE)m_ComboFirst.GetCurSel();
	CommonDataInterface *firstData = dynamic_cast<CommonDataInterface *>(m_First[ m_ComboFirst.GetCurSel()]);
	if (!firstData)
		return;

	m_First[ m_ComboFirst.GetCurSel()]->UpdateData();

	Matrix44 mat;
	switch (first)
	{
	case VAR1_MAT_ROTATEX:
		mat.SetRotationX(m_Angle1);
		break;

	case VAR1_MAT_ROTATEY:
		mat.SetRotationY(m_Angle1);
		break;

	case VAR1_MAT_ROTATEZ:
		mat.SetRotationZ(m_Angle1);
		break;

	case VAR1_MAT_SCALE:
		mat.SetScale(Vector3(m_Angle1, m_Angle1, m_Angle1));
		break;
	}

	firstData->SetMatrix(mat);
}


void CDlgCalc4x4::OnEnChangeEditAngle2()
{
	UpdateData();

	SECOND_VARIABLE second = (SECOND_VARIABLE)m_ComboSecond.GetCurSel();
	CommonDataInterface *secondData = dynamic_cast<CommonDataInterface *>(m_Second[ m_ComboSecond.GetCurSel()]);
	if (!secondData)
		return;

	m_Second[ m_ComboSecond.GetCurSel()]->UpdateData();

	Matrix44 mat;
	switch (second)
	{
	case VAR2_MAT_ROTATEX:
		mat.SetRotationX(m_Angle2);
		break;

	case VAR2_MAT_ROTATEY:
		mat.SetRotationY(m_Angle2);
		break;

	case VAR2_MAT_ROTATEZ:
		mat.SetRotationZ(m_Angle2);
		break;

	case VAR2_MAT_SCALE:
		mat.SetScale(Vector3(m_Angle2, m_Angle2, m_Angle2));
		break;
	}

	secondData->SetMatrix(mat);
}


void CDlgCalc4x4::OnBnClickedCancel()
{
}

void CDlgCalc4x4::OnBnClickedOk()
{
}

