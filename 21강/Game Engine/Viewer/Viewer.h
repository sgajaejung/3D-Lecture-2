
// Viewer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CViewerApp:
// �� Ŭ������ ������ ���ؼ��� Viewer.cpp�� �����Ͻʽÿ�.
//

class CViewerApp : public CWinApp
{
public:
	CViewerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CViewerApp theApp;
