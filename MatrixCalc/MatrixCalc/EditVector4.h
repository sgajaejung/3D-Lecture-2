// CEditVector4 ��ȭ �����Դϴ�.
//
#pragma once


#include "CommonDataInterface.h"

class CEditVector4 : public CDialogEx, public CommonDataInterface
{
	//DECLARE_DYNAMIC(CEditVector4)

public:
	CEditVector4(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEditVector4();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VECTOR_4 };

	virtual Vector4 GetVector4() override;
	virtual void SetVector4(const Vector4 &v) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float m_x, m_y, m_z, m_w;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
