
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "../Network/network.h"

#pragma comment( lib, "winmm.lib" )

using namespace std;

const int WINSIZE_X = 300; //초기 윈도우 가로 크기
const int WINSIZE_Y = 300; //초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y


struct sClient
{
	string name;
	string ip;
	SOCKET sock;
};

SOCKET g_svrSock;
map<SOCKET, sClient> g_clients;
HWND g_hWnd;


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
void MainLoop(int timeDelta);


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Server";
	wchar_t windowName[32] = L"Server";

	//윈도우 클레스 정보 생성
	//내가 이러한 윈도를 만들겠다 라는 정보
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.cbWndExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//윈도우 배경색상
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//윈도우의 커서모양 결정
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//윈도우아이콘모양 결정
	WndClass.hInstance = hInstance;				//프로그램인스턴스핸들 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//윈도우 프로시져 함수 포인터
	WndClass.lpszMenuName = NULL;						//메뉴이름 없으면 NULL
	WndClass.lpszClassName = className;				//지금 작성하고 있는 윈도우 클레스의 이름
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//윈도우 그리기 방식 설정 ( 사이즈가 변경될때 화면갱신 CS_HREDRAW | CS_VREDRAW )

	//위에서 작성한 윈도우 클레스정보 등록
	RegisterClass( &WndClass );

	//윈도우 생성
	//생성된 윈도우 핸들을 전역변수 g_hWnd 가 받는다.
	HWND hWnd = CreateWindow(
		className,				//생성되는 윈도우의 클래스이름
		windowName,				//윈도우 타이틀바에 출력되는 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일 WS_OVERLAPPEDWINDOW
		WINPOS_X,				//윈도우 시작 위치 X 
		WINPOS_Y,				//윈도우 시작 위치 Y
		WINSIZE_X,				//윈도우 가로 크기 ( 작업영역의 크기가 아님 )
		WINSIZE_Y,				//윈도우 세로 크기 ( 작업영역의 크기가 아님 )
		GetDesktopWindow(),		//부모 윈도우 핸들 ( 프로그램에서 최상위 윈도우면 NULL 또는 GetDesktopWindow() )
		NULL,					//메뉴 ID ( 자신의 컨트롤 객체의 윈도우인경우 컨트롤 ID 가 된	
		hInstance,				//이 윈도우가 물릴 프로그램 인스턴스 핸들
		NULL					//추가 정보 NULL ( 신경끄자 )
		);

	//윈도우를 정확한 작업영역 크기로 맞춘다
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	ShowWindow( hWnd, nCmdShow );
	
	g_hWnd = hWnd;


	if (!network::LaunchServer(10000, g_svrSock))
	{
		return 0;
	}

	OutputDebugStringA( "Server Launched \n");


	//메시지 구조체
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	int oldT = GetTickCount();
	while (msg.message != WM_QUIT)
	{
		//PeekMessage 는 메시지 큐에 메시지가 없어도 프로그램이 멈추기 않고 진행이 된다.
		//이때 메시지큐에 메시지가 없으면 false 가 리턴되고 메시지가 있으면 true 가 리턴이된다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage( &msg );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
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
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
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
			// accept(요청을 받으 소켓, 선택 클라이언트 주소)
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


//매인 루프.
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
			if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 접속이 끊겼다는 의미다.
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
