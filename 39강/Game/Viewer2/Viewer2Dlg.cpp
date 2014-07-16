
// Viewer2Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Viewer2.h"
#include "Viewer2Dlg.h"
#include "afxdialogex.h"
#include "ModelView.h"
#include "mmsystem.h"
#include "MainPanel.h"
#include "AnimationController2.h"

#pragma comment( lib, "winmm.lib" )


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int REAL_WINDOW_WIDTH = WINDOW_WIDTH+18;
const int REAL_WINDOW_HEIGHT = WINDOW_HEIGHT+115;


CViewer2Dlg::CViewer2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewer2Dlg::IDD, pParent)
,	m_modelView(NULL)
,	m_aniController(NULL)
,	m_loop(true)
,	m_dxInit(false)
, m_WireFrame(FALSE)
, m_RenderBone(FALSE)
, m_RenderMesh(TRUE)
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CViewer2Dlg::~CViewer2Dlg()
{
	m_aniController->DestroyWindow();
	delete m_aniController;
}

void CViewer2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_WIREFRAME, m_WireFrame);
	DDX_Check(pDX, IDC_CHECK_BONE, m_RenderBone);
	DDX_Check(pDX, IDC_CHECK_MESH, m_RenderMesh);
}

BEGIN_MESSAGE_MAP(CViewer2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CViewer2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CViewer2Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_WIREFRAME, &CViewer2Dlg::OnBnClickedCheckWireframe)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK_BONE, &CViewer2Dlg::OnBnClickedCheckBone)
	ON_BN_CLICKED(IDC_CHECK_MESH, &CViewer2Dlg::OnBnClickedCheckMesh)
END_MESSAGE_MAP()


// CViewer2Dlg 메시지 처리기

BOOL CViewer2Dlg::OnInitDialog()
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//-----------------------------------------------------------------------------------------
	// Init
	DragAcceptFiles(TRUE);

	MoveWindow(CRect(0,0,REAL_WINDOW_WIDTH,REAL_WINDOW_HEIGHT));

	// Create Main Model View
	m_modelView = new CModelView();
	m_modelView->Create(NULL, _T("CView"), WS_CHILDWINDOW, 
		CRect(0,25, WINDOW_WIDTH, WINDOW_HEIGHT+25), this, 0);

	// Create Direct
	graphic::cRenderer::Get()->CreateDirectX(
		m_modelView->GetSafeHwnd(), WINDOW_WIDTH, WINDOW_HEIGHT);

	m_dxInit = true;
	m_modelView->Init();
	m_modelView->ShowWindow(SW_SHOW);

	// Create Main Panel
	{
		const int PANEL_WIDTH = 400;
		const int PANEL_HEIGHT = 800;

		CMainPanel *dlg = new CMainPanel();
		const CString StrClassName = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
			LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(COLOR_BTNFACE+1), 
			LoadIcon(NULL, IDI_APPLICATION) );

		dlg->CreateEx(0, StrClassName, L"Panel", 
			WS_POPUP | WS_CAPTION | WS_SYSMENU | MFS_THICKFRAME, CRect(0, 0, PANEL_WIDTH, PANEL_HEIGHT), this );

		dlg->Init();

		// Main Panel Positioning
		{
			const int screenCX = GetSystemMetrics(SM_CXSCREEN);
			const int screenCY = GetSystemMetrics(SM_CYSCREEN);
			const int x = screenCX/2 - REAL_WINDOW_WIDTH/2 + REAL_WINDOW_WIDTH - 100;
			const int y = screenCY/2 - REAL_WINDOW_HEIGHT/2;

			CRect panelR;
			dlg->GetWindowRect(panelR);
			dlg->MoveWindow(x, y, panelR.Width(), panelR.Height());
		}

		dlg->ShowWindow(SW_SHOW);	
	}


	// Create Animation Controller
	{
		m_aniController = new CAnimationController2(this);
		m_aniController->Create(CAnimationController2::IDD, this);
		m_aniController->MoveWindow(0, WINDOW_HEIGHT+20, WINDOW_WIDTH, 300);
		m_aniController->ShowWindow(SW_SHOW);
		cController::Get()->AddObserver(m_aniController);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CViewer2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CViewer2Dlg::OnPaint()
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
HCURSOR CViewer2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CViewer2Dlg::OnBnClickedOk()
{
	m_loop = false;
	CDialogEx::OnOK();
}


void CViewer2Dlg::OnBnClickedCancel()
{
	m_loop = false;
	CDialogEx::OnCancel();
}


void CViewer2Dlg::MainLoop()
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

		if (m_dxInit)
		{
			if (m_modelView)
			{
				m_modelView->Update(t);
				m_modelView->Render();
			}

			if (m_aniController)
			{
				m_aniController->Update(t);
			}
		}

		Sleep(0);
	}
}


void CViewer2Dlg::OnDropFiles(HDROP hDropInfo)
{
	HDROP hdrop = hDropInfo;
	char filePath[ MAX_PATH];
	const UINT size = DragQueryFileA(hdrop, 0, filePath, MAX_PATH);
	if (size == 0) 
		return;// handle error...

	m_modelView->LoadFile(filePath);
	
	// 애니메이션 파일을 열었다면 윈도우 크기를 늘인다.
	const graphic::RESOURCE_TYPE::TYPE type = graphic::cResourceManager::Get()->GetFileKind(filePath);
	if (graphic::RESOURCE_TYPE::ANIMATION == type)
	{
		CRect wr;
		GetWindowRect(wr);
		MoveWindow(wr.left,wr.top,REAL_WINDOW_WIDTH,REAL_WINDOW_HEIGHT+60);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


void CViewer2Dlg::OnBnClickedCheckWireframe()
{
	UpdateData();
	graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, !m_WireFrame);
	graphic::GetDevice()->SetRenderState(D3DRS_FILLMODE, !m_WireFrame? D3DFILL_SOLID : D3DFILL_WIREFRAME);
}


void CViewer2Dlg::OnBnClickedCheckBone()
{
	UpdateData();
	graphic::cCharacter *character = cController::Get()->GetCharacter();
	RET(!character);

	character->SetRenderBone(m_RenderBone? true : false);
}


void CViewer2Dlg::OnBnClickedCheckMesh()
{
	UpdateData();
	graphic::cCharacter *character = cController::Get()->GetCharacter();
	RET(!character);

	character->SetRenderMesh(m_RenderMesh? true : false);
}
