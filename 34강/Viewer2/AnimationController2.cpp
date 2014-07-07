// AnimationController.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "AnimationController2.h"
#include "afxdialogex.h"

using namespace graphic;


// CAnimationController2 ��ȭ �����Դϴ�.
CAnimationController2::CAnimationController2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnimationController2::IDD, pParent)
	, m_StartFrame(0)
	, m_EndFrame(0)
	, m_CurrentFrame(0)
{

}

CAnimationController2::~CAnimationController2()
{
}

void CAnimationController2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_FRAME, m_Slider);
	DDX_Text(pDX, IDC_STATIC_STARTFRAME, m_StartFrame);
	DDX_Text(pDX, IDC_STATIC_ENDFRAME, m_EndFrame);
	DDX_Text(pDX, IDC_EDIT_CURFRAME, m_CurrentFrame);
}


BEGIN_MESSAGE_MAP(CAnimationController2, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAnimationController2::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAnimationController2::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CAnimationController2::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CAnimationController2::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CAnimationController2::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CAnimationController2::OnBnClickedButtonNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FRAME, &CAnimationController2::OnNMCustomdrawSliderFrame)
END_MESSAGE_MAP()


// CAnimationController2 �޽��� ó�����Դϴ�.


void CAnimationController2::OnBnClickedOk()
{
}


void CAnimationController2::OnBnClickedCancel()
{
}


BOOL CAnimationController2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_Slider.SetRange(0, 500, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CAnimationController2::Update() 
{
	UpdateController();

	UpdateData(FALSE);
}


void CAnimationController2::Update(const float elapseT)
{
	RET(!cController::Get()->IsAnimationPlay());

	cCharacter *character = cController::Get()->GetCharacter();
	RET(!character);
	graphic::cBoneMgr *boneMgr = character->GetBoneMgr();
	RET(!boneMgr);
	graphic::cBoneNode *rootNode = boneMgr->GetRoot();
	RET(!rootNode);
	
	m_CurrentFrame = rootNode->GetCurrentFrame();
	m_Slider.SetPos(m_CurrentFrame);

	UpdateData(FALSE);
}


void CAnimationController2::UpdateController()
{
	const sRawAniGroup *rawAnies= cResourceManager::Get()->LoadAnimation( 
		cController::Get()->GetCurrentAnimationFileName() );
	RET (!rawAnies);
	RET (rawAnies->anies.empty());

	m_StartFrame = (int)rawAnies->anies[ 0].start;
	m_EndFrame = (int)rawAnies->anies[ 0].end;

	m_Slider.SetRange(m_StartFrame, m_EndFrame, TRUE);
}


void CAnimationController2::OnBnClickedButtonPlay()
{
	cController::Get()->SetAnimationPlay(true);
}


void CAnimationController2::OnBnClickedButtonPause()
{
	cController::Get()->SetAnimationPlay(false);
}


void CAnimationController2::OnBnClickedButtonPrev()
{
	
}


void CAnimationController2::OnBnClickedButtonNext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CAnimationController2::OnNMCustomdrawSliderFrame(NMHDR *pNMHDR, LRESULT *pResult)
{
	RET(cController::Get()->IsAnimationPlay());

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	const int pos = m_Slider.GetPos();
	cController::Get()->SetCurrentAnimationFrame(pos);
	m_CurrentFrame = pos;

	UpdateData(FALSE);

	*pResult = 0;
}
