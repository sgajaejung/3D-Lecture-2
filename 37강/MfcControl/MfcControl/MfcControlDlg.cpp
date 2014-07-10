
// MfcControlDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MfcControl.h"
#include "MfcControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CMfcControlDlg ��ȭ ����




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


// CMfcControlDlg �޽��� ó����

BOOL CMfcControlDlg::OnInitDialog()
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMfcControlDlg::OnPaint()
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int p = m_Slider.GetPos();

	*pResult = 0;
}


void CMfcControlDlg::OnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	CString str = m_Tree.GetItemText(hItem);	
	AfxMessageBox(str);
	*pResult = 0;
}


void CMfcControlDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int sel = m_Tab.GetCurSel();

	*pResult = 0;
}
