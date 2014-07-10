
// MfcControlDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MfcControl.h"
#include "MfcControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMfcControlDlg 대화 상자




CMfcControlDlg::CMfcControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMfcControlDlg::IDD, pParent)
	, m_Check(FALSE)
	, m_Static1(_T(""))
	, m_EditNum(0)
	, m_Radio1(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_Check);
	DDX_Text(pDX, IDC_STATIC_1, m_Static1);
	DDX_Text(pDX, IDC_EDIT1, m_EditNum);
	DDV_MinMaxInt(pDX, m_EditNum, 0, 100);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_STATIC_PICKTURE, m_Picture);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_SPIN1, m_Spin);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CMfcControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CMfcControlDlg::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMfcControlDlg::OnSelchangeCombo1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMfcControlDlg::OnSelchangeList1)
	ON_BN_CLICKED(IDC_RADIO1, &CMfcControlDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMfcControlDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMfcControlDlg::OnBnClickedRadio3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMfcControlDlg::OnDeltaposSpin1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMfcControlDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, &CMfcControlDlg::OnSelchangingTree1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMfcControlDlg::OnSelchangeTab1)
END_MESSAGE_MAP()


// CMfcControlDlg 메시지 처리기

BOOL CMfcControlDlg::OnInitDialog()
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
	m_Combo.InsertString(0, L"1");
	m_Combo.InsertString(1, L"2");
	m_Combo.InsertString(2, L"3");
	m_Combo.InsertString(3, L"4");
	m_Combo.InsertString(4, L"5");
	m_Combo.InsertString(5, L"6");
	m_Combo.InsertString(6, L"7");
	m_Combo.SetCurSel(0);


	m_ListBox.InsertString(0, L"Add String1");
	m_ListBox.InsertString(1, L"Add String2");
	m_ListBox.InsertString(2, L"Add String3");
	m_ListBox.InsertString(3, L"Add String4");

	CRect cr;
	m_Picture.GetWindowRect(cr);	

	m_Spin.SetRange(100, 200);

	m_Slider.SetRange(0, 500);


	HTREEITEM hParent = m_Tree.InsertItem(L"Parent");
	m_Tree.InsertItem(L"Child1", hParent);
	m_Tree.InsertItem(L"Child2", hParent);
	HTREEITEM hChild =  m_Tree.InsertItem(L"Child3", hParent);
	m_Tree.InsertItem(L"Child4", hChild);

	m_Tab.InsertItem(0, L"Tab1" );
	m_Tab.InsertItem(1, L"Tab2" );
	m_Tab.InsertItem(2, L"Tab3" );

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMfcControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMfcControlDlg::OnPaint()
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
HCURSOR CMfcControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMfcControlDlg::OnBnClickedCheck1()
{
	UpdateData();	

	if (m_Check)
	{
		m_Static1 = "Check";
	}
	else
	{
		m_Static1 = "UnCheck";
	}

	UpdateData(FALSE);
}


void CMfcControlDlg::OnSelchangeCombo1()
{
	int sel = m_Combo.GetCurSel();
	CString str;
	m_Combo.GetLBText(sel, str);
	AfxMessageBox(str);	
}

void CMfcControlDlg::OnSelchangeList1()
{
	int sel = m_ListBox.GetCurSel();
	CString str;
	m_ListBox.GetText(sel, str);
	AfxMessageBox(str);	
}


void CMfcControlDlg::OnBnClickedRadio1()
{
	UpdateData();
}


void CMfcControlDlg::OnBnClickedRadio2()
{
	
}


void CMfcControlDlg::OnBnClickedRadio3()
{
	
}


void CMfcControlDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	pNMUpDown->iDelta; // change value
	pNMUpDown->iPos; // current pos
	int p = m_Spin.GetPos32();
	int b = m_Spin.GetBase();

	*pResult = 0;
}


void CMfcControlDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int p = m_Slider.GetPos();

	*pResult = 0;
}


void CMfcControlDlg::OnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	CString str = m_Tree.GetItemText(hItem);	
	AfxMessageBox(str);
	*pResult = 0;
}


void CMfcControlDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_Tab.GetCurSel();

	*pResult = 0;
}
