// CEditMatrix4x4 ��ȭ �����Դϴ�.
//
#pragma once

#include "CommonDataInterface.h"

class CEditMatrix4x4 : public CDialogEx, public CommonDataInterface
{
	DECLARE_DYNAMIC(CEditMatrix4x4)

public:
	CEditMatrix4x4(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEditMatrix4x4();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MATRIX_4X4 };

	virtual void SetIdentity() override;
	virtual void SetMatrix(const Matrix44 &m) override;
	virtual Matrix44 GetMatrix() override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	float m_11, m_12, m_13, m_14;
	float m_21, m_22, m_23, m_24;
	float m_31, m_32, m_33, m_34;
	float m_41, m_42, m_43, m_44;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
