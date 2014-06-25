
// ViewerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "ModelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewerDlg::IDD, pParent)
,	m_bLoop(true)
,	m_pModelView(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CViewerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CViewerDlg::OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_FILE_LIST, &CViewerDlg::OnItemchangedFileList)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CViewerDlg 메시지 처리기

BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	MoveWindow(0, 0, 1024, 768);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_pModelView = new CModelView();
	m_pModelView->Create(NULL, _T("CView"), WS_CHILDWINDOW, 
		CRect(0,0, 800, 600), this, 0);
	graphic::cRenderer::Get()->CreateDirectX(
		m_pModelView->GetSafeHwnd(), 800, 600);

	m_pModelView->Init();
	m_pModelView->ShowWindow(SW_SHOW);

	m_FileList.InsertColumn(0, L"Path");
	m_FileList.SetColumnWidth(0, 300);
	m_FileList.InsertItem(0, L"Test1");
	m_FileList.InsertItem(1, L"Test2");
	m_FileList.InsertItem(2, L"Test3");
	m_FileList.InsertItem(3, L"Test4");

	DragAcceptFiles(TRUE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CViewerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			int a = 0;
		}
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


BOOL CViewerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString str;
	ClientToScreen(&point);
	str.Format(L"%d, %d", point.x, point.y);
	//	AfxMessageBox(str);	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CViewerDlg::MainProc()
{
	while( m_bLoop )
	{
		MSG msg;
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0)) 
			{
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		if (m_pModelView)
			m_pModelView->Render();

		Sleep(0);
	}
}

void CViewerDlg::OnBnClickedOk()
{
	m_bLoop = false;
	CDialogEx::OnOK();
}


void CViewerDlg::OnBnClickedCancel()
{
	m_bLoop = false;
	CDialogEx::OnCancel();
}


void CViewerDlg::OnItemchangedFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	CString str = m_FileList.GetItemText(pNMLV->iItem, 0);

}


void CViewerDlg::OnDropFiles(HDROP hDropInfo)
{
	HDROP hdrop = hDropInfo;
	WCHAR filePath[ MAX_PATH];
	const UINT size = DragQueryFile(hdrop, 0, filePath, MAX_PATH);
	if (size == 0) 
		return;// handle error...

	m_FileList.InsertItem(m_FileList.GetItemCount(), filePath);

	CDialogEx::OnDropFiles(hDropInfo);
}
