
// MapToolDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapToolDlg.h"
#include "afxdialogex.h"
#include "MapView.h"
#include <MMSystem.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int REAL_WINDOW_WIDTH = WINDOW_WIDTH+18;
const int REAL_WINDOW_HEIGHT = WINDOW_HEIGHT+115;




CMapToolDlg::CMapToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapToolDlg::IDD, pParent)
,	m_loop(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMapToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMapToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMapToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMapToolDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMapToolDlg �޽��� ó����

BOOL CMapToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	MoveWindow(CRect(0,0,REAL_WINDOW_WIDTH,REAL_WINDOW_HEIGHT));

	// Create Main Model View
	m_mapView = new CMapView();
	m_mapView->Create(NULL, _T("CView"), WS_CHILDWINDOW, 
		CRect(0,25, WINDOW_WIDTH, WINDOW_HEIGHT+25), this, 0);

	m_mapView->Init();
	m_mapView->ShowWindow(SW_SHOW);


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMapToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMapToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMapToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMapToolDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CMapToolDlg::OnBnClickedCancel()
{
	m_loop = false;
	CDialogEx::OnCancel();
}


void CMapToolDlg::MainLoop()
{
	while (m_loop)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) 
			{
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		const int curT = timeGetTime();
		static int oldT = curT;
		const int elapseT = curT - oldT;
		const float t = elapseT * 0.001f;
		oldT = curT;

		if (m_mapView)
		{
			m_mapView->Update(t);
			m_mapView->Render();
		}

		Sleep(0);
	}

}
