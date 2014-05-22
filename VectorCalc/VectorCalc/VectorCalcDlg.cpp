
#include "stdafx.h"
#include "VectorCalc.h"
#include "VectorCalcDlg.h"
#include "afxdialogex.h"
#include "DotProductView.h"
#include "DotControlDlg.h"
#include "CrossControlDlg.h"
#include "CrossProductView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CVectorCalcDlg::CVectorCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVectorCalcDlg::IDD, pParent)
,	m_pDotView(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVectorCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CALC, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CVectorCalcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CVectorCalcDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVectorCalcDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CALC, &CVectorCalcDlg::OnSelchangeTabCalc)
	ON_WM_ENTERIDLE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CVectorCalcDlg 메시지 처리기
BOOL CVectorCalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	m_TabCtrl.InsertItem(0, "Dot Product" );
	m_TabCtrl.InsertItem(1, "Cross Product" );

	const int WIDTH = 400;
	const int HEIGHT = 400;
	m_pDotView = new CDotProductView();
	m_pDotView->Create(NULL, "CDotProductView",  WS_CHILDWINDOW, CRect(10, 30, 10+WIDTH, 30+HEIGHT), &m_TabCtrl, 1000);
	m_pDotView->ShowWindow(SW_SHOW);

	m_pDotControl = new CDotControlDlg(&m_TabCtrl);
	m_pDotControl->Create(CDotControlDlg::IDD, &m_TabCtrl);
	m_pDotControl->MoveWindow(420, 30, 240, 180);
	m_pDotControl->ShowWindow(SW_SHOW);
	m_pDotControl->SetDotProductView(m_pDotView);


	m_pCrossView = new CCrossProductView();
	m_pCrossView->Create(NULL, "CCrossProductView",  WS_CHILDWINDOW, CRect(10, 30, 10+WIDTH, 30+HEIGHT), &m_TabCtrl, 1000);
	m_pCrossView->ShowWindow(SW_HIDE);

	m_pCrossControl = new CCrossControlDlg(&m_TabCtrl);
	m_pCrossControl->Create(CCrossControlDlg::IDD, &m_TabCtrl);
	m_pCrossControl->MoveWindow(420, 30, 240, 180);
	m_pCrossControl->ShowWindow(SW_HIDE);
	m_pCrossControl->SetCrossProductView(m_pCrossView);

	return TRUE;
}


void CVectorCalcDlg::OnPaint()
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

HCURSOR CVectorCalcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVectorCalcDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CVectorCalcDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CVectorCalcDlg::OnSelchangeTabCalc(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	const int tab = m_TabCtrl.GetCurSel();
	switch (tab)
	{
	case 0:
		m_pCrossView->ShowWindow(SW_HIDE);
		m_pCrossControl->ShowWindow(SW_HIDE);
		m_pDotView->ShowWindow(SW_SHOW);
		m_pDotControl->ShowWindow(SW_SHOW);
		break;

	case 1:
		m_pCrossView->ShowWindow(SW_SHOW);
		m_pCrossControl->ShowWindow(SW_SHOW);
		m_pDotView->ShowWindow(SW_HIDE);
		m_pDotControl->ShowWindow(SW_HIDE);
		break;
	}

}


BOOL CVectorCalcDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	DWORD QueueStatus;
	LRESULT resValue = 0;
	bool OnIdleRetVal = true;

	if (message == WM_ENTERIDLE) 
	{
		//OnIdleRetVal = OnIdle((UINT)wParam);
		OnEnterIdle((UINT)wParam, this);
		//if(!OnIdleRetVal)
		//	wParam = 0;
	} else
	{
		resValue = CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
	}

	QueueStatus = GetQueueStatus(QS_ALLINPUT);

	if (HIWORD(QueueStatus) == 0)
	{
		PostMessage(WM_ENTERIDLE, wParam + (OnIdleRetVal ? 1 : 0), 0);
	}

	return resValue;
	//return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}


void CVectorCalcDlg::OnEnterIdle(UINT nWhy, CWnd* pWho)
{
	CDialogEx::OnEnterIdle(nWhy, pWho);

	const int tab = m_TabCtrl.GetCurSel();
	switch (tab)
	{
	case 0:
		if (m_pDotView )
			m_pDotView ->Render();
		break;

	case 1:
		if (m_pCrossView)
			m_pCrossView->Render();
		break;
	}
}


void CVectorCalcDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_pDotView->DestroyWindow();
	m_pDotControl->DestroyWindow();
	delete m_pDotControl;
	m_pCrossView->DestroyWindow();
	m_pCrossControl->DestroyWindow();
	delete m_pCrossControl;
}
