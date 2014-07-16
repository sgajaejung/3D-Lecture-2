
#include "stdafx.h"
#include "window.h"

using namespace framework;


cWindow::cWindow(LPD3DXSPRITE sprite, const int id, const string &name) : // name="window"
	graphic::cSprite(sprite, id, name)
{
}

cWindow::~cWindow()
{
}


// �޼��� �̺�Ʈ ó��.
bool cWindow::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{

	// child window message process
	BOOST_REVERSE_FOREACH (auto &node, m_children)
	{
		if (cWindow *childWnd = dynamic_cast<cWindow*>(node))
		{
			if (childWnd->MessageProc(message, wParam, lParam))
				break;
		}
	}

	return false;
}


// �̺�Ʈ �ڵ鷯 ȣ��
bool cWindow::DispatchEvent( cEvent &event )
{
	cEventHandler::SearchEventTable(event);
	if (!event.IsSkip() && m_parent) // �̺�Ʈ�� ��ŵ���� �ʾҴٸ� �θ�� �ö󰡼� �����Ѵ�.
	{
		if (cWindow *parent = dynamic_cast<cWindow*>(m_parent))
			parent->SearchEventTable(event);
	}
	return true;
}

