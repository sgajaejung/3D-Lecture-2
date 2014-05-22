
#include "stdafx.h"
#include "3DView.h"


// C3DView
C3DView::C3DView()
{

}

C3DView::~C3DView()
{

}

BEGIN_MESSAGE_MAP(C3DView, CView)
END_MESSAGE_MAP()


// C3DView �׸����Դϴ�.

void C3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


void C3DView::RenderVertices(HDC hdc, const vector<Vector3> &vertices, const Matrix44 &tm)
{
	for (unsigned int i=0; i < vertices.size(); ++i)
	{
		Vector3 p = vertices[ i] * tm;

		if (0 == i)
			MoveToEx(hdc, (int)p.x, (int)p.y, NULL);
		else
			LineTo(hdc, (int)p.x, (int)p.y);
	}
}
