
// Viewer2.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CViewer2App:
// �� Ŭ������ ������ ���ؼ��� Viewer2.cpp�� �����Ͻʽÿ�.
//

class CViewer2App : public CWinApp
{
public:
	CViewer2App();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CViewer2App theApp;