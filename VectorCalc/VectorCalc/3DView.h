#pragma once



class C3DView : public CView
{
public:
	C3DView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~C3DView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.


public:
	void RenderVertices(HDC hdc, const vector<Vector3> &vertices, const Matrix44 &tm);
	virtual void Render() {}
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.


protected:
	DECLARE_MESSAGE_MAP()
};
