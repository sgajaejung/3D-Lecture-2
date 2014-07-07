
// Viewer2Dlg.cpp : ���� ����
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


// CViewer2Dlg �޽��� ó����

BOOL CViewer2Dlg::OnInitDialog()
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CViewer2Dlg::OnPaint()
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
	
	// �ִϸ��̼� ������ �����ٸ� ������ ũ�⸦ ���δ�.
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
