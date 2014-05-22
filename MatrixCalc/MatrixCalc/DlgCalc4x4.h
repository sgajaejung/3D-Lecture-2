// CDlgCalc4x4 대화 상자입니다.
#pragma once
#include "afxwin.h"

class CEditVector4;
class CEditMatrix4x4;
class CDlgCalc4x4 : public CDialogEx
{
public:
	CDlgCalc4x4(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCalc4x4();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_4X4 };

	enum FIRST_VARIABLE {
		VAR1_MATRIX,
		VAR1_IDENTITY,
		VAR1_MAT_ROTATEX,
		VAR1_MAT_ROTATEY,
		VAR1_MAT_ROTATEZ,
		VAR1_MAT_SCALE,
		VAR1_VECTOR4,
		VAR1_MAX
	};

	enum SECOND_VARIABLE {
		VAR2_MATRIX,
		VAR2_IDENTITY,
		VAR2_MAT_ROTATEX,
		VAR2_MAT_ROTATEY,
		VAR2_MAT_ROTATEZ,
		VAR2_MAT_SCALE,
		VAR2_MAX
	};


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	CEditMatrix4x4 *m_pMatrixResult;
	CEditVector4 *m_pVectorResult;
	vector<CDialogEx*> m_First;
	vector<CDialogEx*> m_Second;
	CComboBox m_ComboFirst;
	CComboBox m_ComboSecond;
	float m_Angle1;
	float m_Angle2;


public:
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboFirst();
	afx_msg void OnCbnSelchangeComboSecond();
	afx_msg void OnBnClickedButtonCalc();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditAngle1();
	afx_msg void OnEnChangeEditAngle2();
};
