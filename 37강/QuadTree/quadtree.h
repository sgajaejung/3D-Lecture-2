
#if !defined(__QUADTREE_H__)
#define __QUADTREE_H__


class cQuadTree
{
public:
	cQuadTree( int cx, int cy );
	cQuadTree( cQuadTree *parent );
	virtual ~cQuadTree();

	enum CONER_TYPE { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT }; // �̿���� ó���� �����

protected:
	cQuadTree*	m_pChild[ 4];		/// QuadTree�� 4���� �ڽĳ��
	int m_nCenter;			/// QuadTree�� ������ ù��° ��
	int m_nCorner[ 4];		/// QuadTree�� ������ �ι�° ��
									///    TopLeft(TL)      TopRight(TR)
									///              0------1
									///              |      |
									///              |      |
									///              2------3
									/// BottomLeft(BL)      BottomRight(BR)
public:
	BOOL Build();
	int GenerateIndex( LPVOID pIB );

protected:
	cQuadTree* AddChild( int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR );	
	BOOL SetCorners( int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR );
	BOOL SubDivide();
	BOOL IsVisible();
	int	GenTriIndex( int nTriangles, LPVOID pIndex );
	void Destroy();
	void GetCorner( int& _0, int& _1, int& _2, int& _3 ) // �ڳ� �ε��� ���� ���´�.
		{ _0 = m_nCorner[0]; _1 = m_nCorner[1]; _2 = m_nCorner[2]; _3 = m_nCorner[3]; }

	BOOL BuildQuadTree();

};

#endif // __QUADTREE_H__
