// EditChat.cpp : 구현 파일입니다.
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



// CEditChat 메시지 처리기입니다.
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
