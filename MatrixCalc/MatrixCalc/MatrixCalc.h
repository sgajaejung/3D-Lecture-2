
// MatrixCalc.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMatrixCalcApp:
// �� Ŭ������ ������ ���ؼ��� MatrixCalc.cpp�� �����Ͻʽÿ�.
//

class CMatrixCalcApp : public CWinApp
{
public:
	CMatrixCalcApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMatrixCalcApp theApp;