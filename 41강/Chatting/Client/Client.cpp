
// Client.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include <sstream>
#include <string>
#include "afxwin.h"
#include "EditChat.h"
using std::string;
using std::wstring;


// CClientApp
class CClientApp : public CWinApp
{
public:
	CClientApp();
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CClientApp theApp;


// CClientDlg
class CClientDlg : public CDialogEx
{
public:
	CClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	enum { IDD = IDD_CLIENT_DIALOG };

	SOCKET m_socket;
	void MainLoop();

protected:
	void PacketProcess();
	void ParsePacket(char buff[128]);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	HICON m_hIcon;
	bool m_loop;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl m_Ip;
	int m_Port;
	afx_msg void OnBnClickedButtonConnect();
	CListBox m_ChatList;
	CEditChat m_ChatInput;
public:
	afx_msg void OnBnClickedButtonSend();
};



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientApp
BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp 생성

CClientApp::CClientApp()
{
}

CClientApp theApp;


BOOL CClientApp::InitInstance()
{
	CWinApp::InitInstance();

	CClientDlg *dlg;
	dlg = new CClientDlg();
	dlg->Create( CClientDlg::IDD );

	m_pMainWnd = dlg;
	dlg->ShowWindow(SW_SHOW);
	dlg->MainLoop();

	delete dlg;
	return FALSE;
}



///////////////////////////////////////////////////////////////////////////////////////////////
// ClientDlg

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
	, m_Port(10000)
	, m_socket(0)
	, m_loop(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_Ip);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Control(pDX, IDC_LIST_CHAT, m_ChatList);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_ChatInput);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CClientDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClientDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CClientDlg 메시지 처리기

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	m_Ip.SetAddress(127, 0, 0, 1);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


//------------------------------------------------------------------------
// 멀티바이트 문자를 유니코드로 변환
//------------------------------------------------------------------------
std::wstring str2wstr(const std::string &str)
{
	int len;
	int slength = (int)str.length() + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//------------------------------------------------------------------------
// 유니코드를 멀티바이트 문자로 변환
//------------------------------------------------------------------------
std::string wstr2str(const std::wstring &wstr)
{
	const int slength = (int)wstr.length() + 1;
	const int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, 0, 0, NULL, FALSE);
	char* buf = new char[len];
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, buf, len, NULL, FALSE);
	std::string r(buf);
	delete[] buf;
	return r;
}


void CClientDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CClientDlg::OnBnClickedCancel()
{
	m_loop = false;
	CDialogEx::OnCancel();
}


void CClientDlg::MainLoop()
{
	//메시지 구조체
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	while (m_loop)
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
			PacketProcess();
		}
	}
}


void CClientDlg::PacketProcess()
{
	const timeval t = {0, 10}; // 10 millisecond
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(m_socket, &readSockets);

	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		char buff[ 128];
		const int result = recv( readSockets.fd_array[ 0], buff, sizeof(buff), 0);
		if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 접속이 끊겼다는 의미다.
		{
			m_ChatList.InsertString(m_ChatList.GetCount(), L"서버와 연결이 끊김" );
			closesocket(m_socket);
		}
		else
		{
			ParsePacket(buff);
		}
	}
}


void CClientDlg::ParsePacket(char buff[128])
{
	using namespace network;
	sPacketHeader *header = (sPacketHeader*)buff;

	switch (header->protocol)
	{
	case PROTOCOL::LOGIN:
		break;

	case PROTOCOL::CHATTING:
		{
			const sChatProtocol *protocol = (sChatProtocol*)buff;
			const wstring wstr = str2wstr(protocol->msg);
			m_ChatList.InsertString(m_ChatList.GetCount(), wstr.c_str());
		}
		break;
	}
}


void CClientDlg::OnBnClickedButtonConnect()
{
	UpdateData();

	DWORD address;
	m_Ip.GetAddress(address);

	std::stringstream ss;
	ss << ((address & 0xff000000) >> 24) << "." 
		<< ((address & 0x00ff0000) >> 16) << "." 
		<< ((address & 0x0000ff00) >> 8) << "." 
		<< (address & 0x000000ff);

	const string ip = ss.str();
	
	if (network::LaunchClient(ip, m_Port, m_socket))
	{
		m_ChatList.InsertString(m_ChatList.GetCount(), L"접속 성공");
	}
	else
	{
		m_ChatList.InsertString(m_ChatList.GetCount(), L"접속 실패");
	}
}


void CClientDlg::OnBnClickedButtonSend()
{
	CString text;
	m_ChatInput.GetWindowText(text);
	m_ChatInput.SetWindowText(L"");
	UpdateData();

	const wstring wstr = text;
	const string str = wstr2str(wstr);

	network::sChatProtocol chat;
	ZeroMemory(&chat, sizeof(chat));
	chat.header.protocol = network::PROTOCOL::CHATTING;
	strcpy_s(chat.msg, sizeof(chat.msg), str.c_str());

	char buff[ 128];
	ZeroMemory(buff, sizeof(buff));
	memcpy(buff, &chat, sizeof(chat));

	const int result = send(m_socket, buff, sizeof(buff), 0);
	if (result == INVALID_SOCKET)
	{	
		m_ChatList.InsertString(m_ChatList.GetCount(), L"서버와 접속이 끊김");
	}
}
