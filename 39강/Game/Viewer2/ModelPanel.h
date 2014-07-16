#pragma once
#include "afxcmn.h"
#include "PanelBase.h"


namespace graphic  {
	class cBoneNode;
}


// CModelPanel 대화 상자입니다.
class CModelPanel : public CPanelBase
								, public common::iObserver
{
public:
	CModelPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CModelPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL };

	virtual void Update() override;


protected:
	void UpdateModelInfo();
	void UpdateMeshInfo();
	void UpdateMaterialInfo();
	void UpdateBoneInfo();
	void UpdateRawBoneInfo();
	void MakeBoneTree(HTREEITEM hParent,  graphic::cBoneNode *node);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_MeshTree;
	CTreeCtrl m_MaterialTree;
	CTreeCtrl m_RawBoneTree;
	CTreeCtrl m_BoneTree;
	CString m_FilePath;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_FileName;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPanelSearch();
};
