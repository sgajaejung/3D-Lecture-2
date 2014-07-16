#pragma once


// CPanelBase ��ȭ �����Դϴ�.

class CPanelBase : public CDialogEx
{
public:
	CPanelBase(UINT id, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPanelBase();


protected:
	void ExpandAll(CTreeCtrl &treeCtrl);
	HTREEITEM FindTree( CTreeCtrl &treeCtrl, const wstring &text );
	void MoveTreeWindow(CTreeCtrl &treeCtrl, int cx, int cy);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
