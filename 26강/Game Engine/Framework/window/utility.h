
#pragma once


namespace framework
{

	// ������ ����
	HWND InitWindow(HINSTANCE hInstance, 
		const wstring &windowName, 
		const RECT &windowRect,
		int nCmdShow,
		WNDPROC WndProc);

}
