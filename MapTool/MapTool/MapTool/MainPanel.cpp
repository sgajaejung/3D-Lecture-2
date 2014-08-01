// MainPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanel.h"
#include "afxdialogex.h"
#include "MapView.h"
#include <fstream>

CMainPanel *g_mainPanel = NULL;


// CMainPanel 대화 상자입니다.
CMainPanel::CMainPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanel::IDD, pParent)
{
	g_mainPanel = this;
}

CMainPanel::~CMainPanel()
{
}

void CMainPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CUBE, m_CubeList);
}


BEGIN_MESSAGE_MAP(CMainPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainPanel::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_CUBE, &CMainPanel::OnSelchangeListCube)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMainPanel::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMainPanel::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CMainPanel 메시지 처리기입니다.


void CMainPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CMainPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CMainPanel::InsertCube( graphic::cCube &cube )
{
	CString str;
	str.Format( L"%d", m_CubeList.GetCount() );
	m_CubeList.InsertString( m_CubeList.GetCount(), str );
}


void CMainPanel::OnSelchangeListCube()
{
	int idx = m_CubeList.GetCurSel();
	g_mapView->SelectCube( idx );
}


void CMainPanel::OnBnClickedButtonSave()
{
	using namespace std;

	ofstream fs;
	fs.open("mapdata.mda", ios_base::binary );

	int cubeSize = g_cubes.size();
	fs.write( (char*)&cubeSize, sizeof(int) );

	for (int i=0; i < (int)g_cubes.size(); ++i)
	{
		fs.write( (char*)&g_cubes[ i]->GetMin(), sizeof(Vector3) );
		fs.write( (char*)&g_cubes[ i]->GetMax(), sizeof(Vector3) );
		fs.write( (char*)&g_cubes[ i]->GetTransform(), sizeof(Matrix44) );
	}

	AfxMessageBox(L"Save Complete");
}


void CMainPanel::OnBnClickedButtonLoad()
{
	using namespace std;

	for (int i=0; i < (int)g_cubes.size(); ++i)
		delete g_cubes[ i];
	g_cubes.clear();
	while (0 < m_CubeList.GetCount())
		m_CubeList.DeleteString(0);

	ifstream fs;
	fs.open("mapdata.mda", ios_base::binary );

	int cubeSize;
	fs.read( (char*)&cubeSize, sizeof(int) );

	for (int i=0; i <cubeSize; ++i)
	{
		Vector3 _min, _max;
		Matrix44 tm;
		fs.read( (char*)&_min, sizeof(Vector3) );
		fs.read( (char*)&_max, sizeof(Vector3) );
		fs.read( (char*)&tm, sizeof(Matrix44) );

		graphic::cCube *newCube = new graphic::cCube();
		newCube->SetCube(_min, _max);
		newCube->SetTransform(tm);
		newCube->SetColor( 0xFF0000 );
		g_cubes.push_back( newCube );

		CString str;
		str.Format( L"%d", m_CubeList.GetCount() );
		m_CubeList.InsertString( m_CubeList.GetCount(), str );
	}
}
