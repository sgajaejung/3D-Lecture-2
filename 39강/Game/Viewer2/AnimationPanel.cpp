// AnimationPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "AnimationPanel.h"
#include "afxdialogex.h"

using namespace graphic;

// CAnimationPanel 대화 상자입니다.

CAnimationPanel::CAnimationPanel(CWnd* pParent /*=NULL*/)
	: CPanelBase(CAnimationPanel::IDD, pParent)
	, m_FilePath(_T(""))
	, m_StartFrame(0)
	, m_EndFrame(0)
	, m_FileName(_T(""))
{

}

CAnimationPanel::~CAnimationPanel()
{
}

void CAnimationPanel::DoDataExchange(CDataExchange* pDX)
{
	CPanelBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_FilePath);
	DDX_Text(pDX, IDC_EDIT_START_TIME, m_StartFrame);
	DDX_Text(pDX, IDC_EDIT_END_TIME, m_EndFrame);
	DDX_Control(pDX, IDC_TREE_ANI, m_AniTree);
	DDX_Text(pDX, IDC_STATIC_FILENAME2, m_FileName);
}


BEGIN_MESSAGE_MAP(CAnimationPanel, CPanelBase)
	ON_BN_CLICKED(IDOK, &CAnimationPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAnimationPanel::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAnimationPanel 메시지 처리기입니다.


void CAnimationPanel::OnBnClickedOk()
{
}


void CAnimationPanel::OnBnClickedCancel()
{
}


void CAnimationPanel::Update() 
{
	UpdateAnimationInfo();
	UpdateAnimationTree();

	UpdateData(FALSE);
}


void CAnimationPanel::UpdateAnimationInfo()
{
	const sRawAniGroup *rawAnies= cResourceManager::Get()->LoadAnimation( 
		cController::Get()->GetCurrentAnimationFileName() );
	RET (!rawAnies);

	 string filePath = cController::Get()->GetCurrentAnimationFileName();
	 m_FilePath = filePath.c_str();
	 m_FileName =common::GetFileName(filePath).c_str();
	RET (rawAnies->anies.empty());	

	m_StartFrame = rawAnies->anies[ 0].start;
	m_EndFrame = rawAnies->anies[ 0].end;
}


// 애니메이션 정보 출력.
void CAnimationPanel::UpdateAnimationTree()
{
	m_AniTree.DeleteAllItems();

	const sRawAniGroup *rawAnies= cResourceManager::Get()->LoadAnimation( 
		cController::Get()->GetCurrentAnimationFileName() );
	RET (!rawAnies);

	const wstring rootStr = formatw( "Animation [%d]", rawAnies->anies.size());
	const HTREEITEM hRoot = m_AniTree.InsertItem(rootStr.c_str());

	for (u_int i=0; i < rawAnies->anies.size(); ++i)
	{
		const sRawAni &ani = rawAnies->anies[ i];
		const wstring str = formatw("track%d", i+1);
		const HTREEITEM hItem = m_AniTree.InsertItem( str.c_str(), hRoot);

		const wstring start = formatw("start = %d", (int)ani.start);
		const wstring end = formatw("end = %d", (int)ani.end);
		const wstring keypos = formatw("keyPos Count = %d", ani.pos.size());
		const wstring keyrot = formatw("keyRot Count = %d", ani.rot.size());
		const wstring keyscale = formatw("keyScale Count = %d", ani.scale.size());

		m_AniTree.InsertItem( start.c_str(), hItem);
		m_AniTree.InsertItem( end.c_str(), hItem);
		m_AniTree.InsertItem( keypos.c_str(), hItem);
		m_AniTree.InsertItem( keyrot.c_str(), hItem);
		m_AniTree.InsertItem( keyscale.c_str(), hItem);
	}

	m_AniTree.Expand(hRoot, TVE_EXPAND);
}


void CAnimationPanel::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	
}
