
#if !defined(__QUADTREE_H__)
#define __QUADTREE_H__


class cQuadTree
{
public:
	cQuadTree( int cx, int cy );
	cQuadTree( cQuadTree *parent );
	virtual ~cQuadTree();

	enum CONER_TYPE { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT }; // 이웃노드 처리용 상수값

protected:
	cQuadTree*	m_pChild[ 4];		/// QuadTree의 4개의 자식노드
	int m_nCenter;			/// QuadTree에 보관할 첫번째 값
	int m_nCorner[ 4];		/// QuadTree에 보관할 두번째 값
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
	void GetCorner( int& _0, int& _1, int& _2, int& _3 ) // 코너 인덱스 값을 얻어온다.
		{ _0 = m_nCorner[0]; _1 = m_nCorner[1]; _2 = m_nCorner[2]; _3 = m_nCorner[3]; }

	BOOL BuildQuadTree();

};

#endif // __QUADTREE_H__
