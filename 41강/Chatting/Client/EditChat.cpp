// EditChat.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EditChat.h"


// CEditChat
CEditChat::CEditChat()
{

}

CEditChat::~CEditChat()
{
}


BEGIN_MESSAGE_MAP(CEditChat, CEdit)
END_MESSAGE_MAP()



// CEditChat �޽��� ó�����Դϴ�.
BOOL CEditChat::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam)
		{
			GetParent()->PostMessage(WM_CHATENTER, 0, 0);
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
