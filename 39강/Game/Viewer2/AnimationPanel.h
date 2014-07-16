#pragma once
#include "afxcmn.h"
#include "PanelBase.h"


// CAnimationPanel 대화 상자입니다.

class CAnimationPanel : public CPanelBase
									   , public common::iObserver
{
public:
	CAnimationPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationPanel();
	enum { IDD = IDD_DIALOG_ANIMATION };

	virtual void Update() override;


protected:
	void UpdateAnimationInfo();
	void UpdateAnimationTree();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_FilePath;
	CString m_FileName;
	int m_StartFrame;
	int m_EndFrame;
	CTreeCtrl m_AniTree;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
