
#include <windows.h>
#include <d3d9.h>

const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
const int WINPOS_X = 0; //�ʱ� ������ ���� ��ġ X
const int WINPOS_Y = 0; //�ʱ� ������ ���� ��ġ Y


// �ݹ� ���ν��� �Լ� ������ Ÿ�� 
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Sample";
	wchar_t windowName[32] = L"Sample";

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
	WndClass.lpszClassName = className;				//���� �ۼ��ϰ� �ִ� ������ Ŭ������ �̸�
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//������ �׸��� ��� ���� ( ����� ����ɶ� ȭ�鰻�� CS_HREDRAW | CS_VREDRAW )

	//������ �ۼ��� ������ Ŭ�������� ���
	RegisterClass( &WndClass );

	//������ ����
	//������ ������ �ڵ��� �������� g_hWnd �� �޴´�.
	HWND hWnd = CreateWindow(
		className,				//�����Ǵ� �������� Ŭ�����̸�
		windowName,				//������ Ÿ��Ʋ�ٿ� ��µǴ� �̸�
		WS_OVERLAPPEDWINDOW,	//������ ��Ÿ�� WS_OVERLAPPEDWINDOW
		WINPOS_X,				//������ ���� ��ġ X 
		WINPOS_Y,				//������ ���� ��ġ Y
		WINSIZE_X,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		WINSIZE_Y,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		GetDesktopWindow(),		//�θ� ������ �ڵ� ( ���α׷����� �ֻ��� ������� NULL �Ǵ� GetDesktopWindow() )
		NULL,					//�޴� ID ( �ڽ��� ��Ʈ�� ��ü�� �������ΰ�� ��Ʈ�� ID �� ��	
		hInstance,				//�� �����찡 ���� ���α׷� �ν��Ͻ� �ڵ�
		NULL					//�߰� ���� NULL ( �Ű���� )
		);

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	ShowWindow( hWnd, nCmdShow );

	//�޽��� ����ü
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	while (msg.message != WM_QUIT)
	{
		//PeekMessage �� �޽��� ť�� �޽����� ��� ���α׷��� ���߱� �ʰ� ������ �ȴ�.
		//�̶� �޽���ť�� �޽����� ������ false �� ���ϵǰ� �޽����� ������ true �� �����̵ȴ�.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
			DispatchMessage( &msg );  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}
		else
		{

		}
	}

	return 0;
}


//
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;
	case WM_DESTROY: //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

