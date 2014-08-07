
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "../Network/network.h"

#pragma comment( lib, "winmm.lib" )

using namespace std;

const int WINSIZE_X = 300; //�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 300; //�ʱ� ������ ���� ũ��
const int WINPOS_X = 0; //�ʱ� ������ ���� ��ġ X
const int WINPOS_Y = 0; //�ʱ� ������ ���� ��ġ Y


struct sClient
{
	string name;
	string ip;
	SOCKET sock;
};

SOCKET g_svrSock;
map<SOCKET, sClient> g_clients;
HWND g_hWnd;


// �ݹ� ���ν��� �Լ� ������ Ÿ��
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
void MainLoop(int timeDelta);


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Server";
	wchar_t windowName[32] = L"Server";

	//������ Ŭ���� ���� ����
	//���� �̷��� ������ ����ڴ� ��� ����
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.cbWndExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//������ ������
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
	
	g_hWnd = hWnd;


	if (!network::LaunchServer(10000, g_svrSock))
	{
		return 0;
	}

	OutputDebugStringA( "Server Launched \n");


	//�޽��� ����ü
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	int oldT = GetTickCount();
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
			const int curT = timeGetTime();
			const int elapseT = curT - oldT;
			oldT = curT;
			MainLoop(elapseT);
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
			::DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


void AcceptClient()
{
	const timeval t = {0, 10}; // 10 millisecond
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(g_svrSock, &readSockets);

	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		for (u_int i=0; i < readSockets.fd_count; ++i)
		{
			// accept(��û�� ���� ����, ���� Ŭ���̾�Ʈ �ּ�)
			SOCKET remoteSocket = accept(readSockets.fd_array[ i], NULL, NULL);
			if (remoteSocket == INVALID_SOCKET)
			{
				return;
			}

			if (g_clients.end() == g_clients.find(remoteSocket))
			{
				// get ip address
				sockaddr_in addr;
				int len = sizeof(addr);
				memset(&addr,0,sizeof(addr));
				getpeername( remoteSocket, (sockaddr*)&addr, &len );
				string ip = inet_ntoa(addr.sin_addr);

				sClient client;
				client.ip = ip;
				client.sock = remoteSocket;
				g_clients[ remoteSocket] = client;
			}
		}
	}
}


void DisplayClient()
{
	HDC hdc = GetDC(g_hWnd);

	Rectangle(hdc, 0, 0, 500, 500);

	std::stringstream ss;
	ss << "client count = " << g_clients.size();
	const string str = ss.str();
	TextOutA(hdc, 10, 10, str.c_str(), str.length());

	int y = 25;
	auto it = g_clients.begin();
	while (g_clients.end() != it)
	{
		std::stringstream ss;
		ss << "ip = " << it->second.ip;
		const string str = ss.str();
		TextOutA(hdc, 10, y, str.c_str(), str.length());

		++it;
		y += 20;
	}

	ReleaseDC(g_hWnd, hdc);
}


void MakeSessionFdset( OUT fd_set &set)
{
	FD_ZERO(&set);

	auto it = g_clients.begin();
	while (g_clients.end() != it)
	{
		FD_SET(it->second.sock, &set);
		++it;
	}
}


void SendAll(char buff[128])
{
	auto it = g_clients.begin();
	while (g_clients.end() != it)
	{
		send(it->second.sock, buff, 128, 0);
		++it;
	}
}


//���� ����.
void MainLoop(int timeDelta)
{
	DisplayClient();

	AcceptClient();

	const timeval t = {0, 10}; // 10 millisecond
	fd_set readSockets;
	MakeSessionFdset(readSockets);

	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		for (u_int i=0; i < readSockets.fd_count; ++i)
		{
			char buff[ 128];
			const int result = recv(readSockets.fd_array[ i], buff, sizeof(buff), 0);
			if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ������ ����ٴ� �ǹ̴�.
			{
				g_clients.erase(readSockets.fd_array[ i]);
			}
			else
			{
				SendAll(buff);
			}
		}
	}
}
