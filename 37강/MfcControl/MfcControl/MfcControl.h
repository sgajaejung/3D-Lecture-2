
// MfcControl.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMfcControlApp:
// �� Ŭ������ ������ ���ؼ��� MfcControl.cpp�� �����Ͻʽÿ�.
//

class CMfcControlApp : public CWinApp
{
public:
	CMfcControlApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMfcControlApp theApp;