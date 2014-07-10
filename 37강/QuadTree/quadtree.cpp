
#include "stdafx.h"
#include "quadtree.h"
#include "../common/utility.h"

#define _USE_INDEX16


//-----------------------------------------------------------------------------//
// 높이,너비 분할 갯수 설정
//-----------------------------------------------------------------------------//
cQuadTree::cQuadTree( int cx, int cy )
{
	m_pParent = NULL;
	m_nCenter = 0;
	for( int i = 0 ; i < 4 ; i++ )
	{
		m_pChild[i] = NULL;
	}
	m_nCorner[ CORNER_TL] = 0;
	m_nCorner[ CORNER_TR] = cx - 1;
	m_nCorner[ CORNER_BL] = cx * ( cy - 1 );
	m_nCorner[ CORNER_BR] = cx * cy - 1;

	m_nCenter = ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
						  	  m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;
}

cQuadTree::cQuadTree( cQuadTree* pParent )
{
	m_pParent = pParent;
	m_nCenter = 0;
	for(for i = 0 ; i < 4 ; i++ )
	{
		m_pChild[i] = NULL;
		m_nCorner[i] = 0;
	}
}

cQuadTree::~cQuadTree()
{
	Destroy();
}


//-----------------------------------------------------------------------------//
/// 메모리에서 쿼드트리를 삭제한다.
//-----------------------------------------------------------------------------//
void cQuadTree::Destroy()
{
	for( int i = 0 ; i < 4 ; i++ ) 
		SAFE_DELETE( m_pChild[i] );
}


//-----------------------------------------------------------------------------//
/// 4개의 코너값을 셋팅한다.
//-----------------------------------------------------------------------------//
BOOL cQuadTree::SetCorners( int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR )
{
	m_nCorner[CORNER_TL] = nCornerTL;
	m_nCorner[CORNER_TR] = nCornerTR;
	m_nCorner[CORNER_BL] = nCornerBL;
	m_nCorner[CORNER_BR] = nCornerBR;
	m_nCenter = ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
							 m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;
	return TRUE;
}


//-----------------------------------------------------------------------------//
/// 자식 노드를 추가한다.
//-----------------------------------------------------------------------------//
cQuadTree* cQuadTree::AddChild( int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR )
{
	cQuadTree*	pChild;

	pChild = new cQuadTree( this );
	pChild->SetCorners( nCornerTL, nCornerTR, nCornerBL, nCornerBR );

	return pChild;
}


//-----------------------------------------------------------------------------//
/// Quadtree를 4개의 자식 트리로 부분분할(subdivide)한다.
//-----------------------------------------------------------------------------//
BOOL cQuadTree::SubDivide()
{
	int		nTopEdgeCenter;
	int		nBottomEdgeCenter;
	int		nLeftEdgeCenter;
	int		nRightEdgeCenter;
	int		nCentralPoint;

	// 상단변 가운데
	nTopEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] ) / 2;
	// 하단변 가운데 
	nBottomEdgeCenter	= ( m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 2;
	// 좌측변 가운데
	nLeftEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL] ) / 2;
	// 우측변 가운데
	nRightEdgeCenter	= ( m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR] ) / 2;
	// 한가운데
	nCentralPoint		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
							m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;

	// 더이상 분할이 불가능한가? 그렇다면 SubDivide() 종료
	if( abs(m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1 )
	{
		return FALSE;
	}

	// 4개의 자식노드 추가
	m_pChild[CORNER_TL] = AddChild( m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint );
	m_pChild[CORNER_TR] = AddChild( nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter );
	m_pChild[CORNER_BL] = AddChild( nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter );
	m_pChild[CORNER_BR] = AddChild( nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR] );

	return TRUE;
}


//-----------------------------------------------------------------------------//
/// 출력할 폴리곤의 인덱스를 생성한다.
//-----------------------------------------------------------------------------//
int	cQuadTree::GenTriIndex( int nTris, LPVOID pIndex)
{

#ifdef _USE_INDEX16
	LPWORD p = ((LPWORD)pIndex) + nTris * 3;
#else
	LPDWORD p = ((LPDWORD)pIndex) + nTris * 3;
#endif

	if( IsVisible() )
	{
		// 만약 최하위 노드라면 부분분할(subdivide)이 불가능하므로 그냥 출력하고 리턴한다.
		if( abs(m_nCorner[CORNER_TR]-m_nCorner[CORNER_TL]) <= 1 )
		{
			// 좌측상단 삼각형
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;
			// 우측하단 삼각형
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			nTris++;
			return nTris;
		}
	}

	if( m_pChild[CORNER_TL] ) nTris = m_pChild[CORNER_TL]->GenTriIndex( nTris, pIndex );
	if( m_pChild[CORNER_TR] ) nTris = m_pChild[CORNER_TR]->GenTriIndex( nTris, pIndex );
	if( m_pChild[CORNER_BL] ) nTris = m_pChild[CORNER_BL]->GenTriIndex( nTris, pIndex );
	if( m_pChild[CORNER_BR] ) nTris = m_pChild[CORNER_BR]->GenTriIndex( nTris, pIndex );

	return nTris;
}


//-----------------------------------------------------------------------------//
// QuadTree를 구축한다.
//-----------------------------------------------------------------------------//
BOOL cQuadTree::Build()
{
	BuildQuadTree();

	return TRUE;
}


//-----------------------------------------------------------------------------//
//	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다.
//-----------------------------------------------------------------------------//
int	cQuadTree::GenerateIndex( LPVOID pIndex )
{
	return GenTriIndex( 0, pIndex );
}



BOOL cQuadTree::IsVisible() 
{ 
	return true;
}


//-----------------------------------------------------------------------------//
// 쿼드트리를 만든다.(Build()함수에서 불린다)
//-----------------------------------------------------------------------------//
BOOL cQuadTree::BuildQuadTree()
{
	if( SubDivide() )
	{
		m_pChild[CORNER_TL]->BuildQuadTree();
		m_pChild[CORNER_TR]->BuildQuadTree();
		m_pChild[CORNER_BL]->BuildQuadTree();
		m_pChild[CORNER_BR]->BuildQuadTree();
	}
	return TRUE;
}

