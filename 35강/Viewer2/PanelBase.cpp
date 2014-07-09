// PanelBase.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "PanelBase.h"
#include "afxdialogex.h"


// CPanelBase ��ȭ �����Դϴ�.

CPanelBase::CPanelBase(UINT id, CWnd* pParent /*=NULL*/)
	: CDialogEx(id, pParent)
{

}

CPanelBase::~CPanelBase()
{
}

void CPanelBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPanelBase, CDialogEx)
END_MESSAGE_MAP()


// CPanelBase �޽��� ó�����Դϴ�.


// ��� Ʈ����带 ��ģ��.
void CPanelBase::ExpandAll(CTreeCtrl &treeCtrl)
{
	HTREEITEM hRoot = treeCtrl.GetRootItem();
	vector<HTREEITEM> items;
	items.reserve(treeCtrl.GetCount());

	items.push_back(hRoot);

	while (!items.empty())
	{
		HTREEITEM hItem = items.back();
		items.pop_back();
		treeCtrl.Expand(hItem, TVE_EXPAND);

		HTREEITEM hfirstChild = treeCtrl.GetChildItem(hItem);
		if (hfirstChild)
		{
			items.push_back(hfirstChild);

			while (HTREEITEM hNextItem = treeCtrl.GetNextSiblingItem(hfirstChild))
			{
				items.push_back(hNextItem);
				hfirstChild = hNextItem;
			}
		}
	}

	treeCtrl.SelectSetFirstVisible(hRoot);
}


void CPanelBase::MoveTreeWindow(CTreeCtrl &treeCtrl, int cx, int cy)
{
	if (treeCtrl.GetSafeHwnd())
	{
		CRect wr;
		treeCtrl.GetWindowRect(wr);
		ScreenToClient(wr);
		treeCtrl.MoveWindow(wr.left, wr.top, cx, wr.Height());
	}
}


HTREEITEM CPanelBase::FindTree( CTreeCtrl &treeCtrl, const wstring &text )
{
	HTREEITEM hRoot = treeCtrl.GetRootItem();
	vector<HTREEITEM> items;
	items.reserve(treeCtrl.GetCount());

	items.push_back(hRoot);

	while (!items.empty())
	{
		HTREEITEM hItem = items.back();
		items.pop_back();
		CString str = treeCtrl.GetItemText(hItem);

		const int idx = str.Find(text.c_str());
		if (idx >= 0)
			return hItem;

		HTREEITEM hfirstChild = treeCtrl.GetChildItem(hItem);
		if (hfirstChild)
		{
			items.push_back(hfirstChild);

			while (HTREEITEM hNextItem = treeCtrl.GetNextSiblingItem(hfirstChild))
			{
				items.push_back(hNextItem);
				hfirstChild = hNextItem;
			}
		}
	}

	return NULL;
}
