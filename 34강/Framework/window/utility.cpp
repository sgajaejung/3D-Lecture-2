
#include "stdafx.h"
#include "utility.h"

using namespace framework;


HWND framework::InitWindow(HINSTANCE hInstance, const wstring &windowName, const RECT &windowRect, int nCmdShow, WNDPROC WndProc)
{
	const int X = windowRect.left;
	const int Y = windowRect.top;
	const int WIDTH = windowRect.right - windowRect.left;
	const int HEIGHT = windowRect.bottom - windowRect.top;

	//������ Ŭ���� ���� ����
	//���� �̷��� ������ ����ڴ� ��� ����
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.cbWndExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		//������ ������
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//�������� Ŀ����� ����
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//����������ܸ�� ����
	WndClass.hInstance = hInstance;				//���α׷��ν��Ͻ��ڵ� 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//������ ���ν��� �Լ� ������
	WndClass.lpszMenuName = NULL;						//�޴��̸� ������ NULL
	WndClass.lpszClassName = windowName.c_str();				//���� �ۼ��ϰ� �ִ� ������ Ŭ������ �̸�
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//������ �׸��� ��� ���� ( ����� ����ɶ� ȭ�鰻�� CS_HREDRAW | CS_VREDRAW )

	//������ �ۼ��� ������ Ŭ�������� ���
	RegisterClass( &WndClass );

	//������ ����
	//������ ������ �ڵ��� �������� g_hWnd �� �޴´�.
	HWND hWnd = CreateWindow(
		windowName.c_str(),				//�����Ǵ� �������� Ŭ�����̸�
		windowName.c_str(),				//������ Ÿ��Ʋ�ٿ� ��µǴ� �̸�
		WS_OVERLAPPEDWINDOW,	//������ ��Ÿ�� WS_OVERLAPPEDWINDOW
		X,				//������ ���� ��ġ X 
		Y,				//������ ���� ��ġ Y
		WIDTH,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		HEIGHT,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		GetDesktopWindow(),		//�θ� ������ �ڵ� ( ���α׷����� �ֻ��� ������� NULL �Ǵ� GetDesktopWindow() )
		NULL,					//�޴� ID ( �ڽ��� ��Ʈ�� ��ü�� �������ΰ�� ��Ʈ�� ID �� ��	
		hInstance,				//�� �����찡 ���� ���α׷� �ν��Ͻ� �ڵ�
		NULL					//�߰� ���� NULL ( �Ű���� )
		);

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, WIDTH, HEIGHT};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.


	const int width = rcClient.right - rcClient.left;
	const int height = rcClient.bottom - rcClient.top;
	const int screenCX = GetSystemMetrics(SM_CXSCREEN);
	const int screenCY = GetSystemMetrics(SM_CYSCREEN);
	const int x = screenCX/2 - width/2;
	const int y = screenCY/2 - height/2;


	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE );

	MoveWindow(hWnd, x, y, width, height, TRUE);

	ShowWindow( hWnd, nCmdShow );
	
	return hWnd;
}
