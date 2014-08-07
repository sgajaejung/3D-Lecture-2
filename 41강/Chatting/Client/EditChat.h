#pragma once


class CEditChat : public CEdit
{
public:
	CEditChat();
	virtual ~CEditChat();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


