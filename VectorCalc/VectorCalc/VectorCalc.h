
// VectorCalc.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CVectorCalcApp:
// �� Ŭ������ ������ ���ؼ��� VectorCalc.cpp�� �����Ͻʽÿ�.
//

class CVectorCalcApp : public CWinApp
{
public:
	CVectorCalcApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CVectorCalcApp theApp;