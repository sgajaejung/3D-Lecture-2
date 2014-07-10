
#include "stdafx.h"
#include "quadtree.h"
#include "../common/utility.h"

#define _USE_INDEX16


//-----------------------------------------------------------------------------//
// ����,�ʺ� ���� ���� ����
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
/// �޸𸮿��� ����Ʈ���� �����Ѵ�.
//-----------------------------------------------------------------------------//
void cQuadTree::Destroy()
{
	for( int i = 0 ; i < 4 ; i++ ) 
		SAFE_DELETE( m_pChild[i] );
}


//-----------------------------------------------------------------------------//
/// 4���� �ڳʰ��� �����Ѵ�.
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
/// �ڽ� ��带 �߰��Ѵ�.
//-----------------------------------------------------------------------------//
cQuadTree* cQuadTree::AddChild( int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR )
{
	cQuadTree*	pChild;

	pChild = new cQuadTree( this );
	pChild->SetCorners( nCornerTL, nCornerTR, nCornerBL, nCornerBR );

	return pChild;
}


//-----------------------------------------------------------------------------//
/// Quadtree�� 4���� �ڽ� Ʈ���� �κк���(subdivide)�Ѵ�.
//-----------------------------------------------------------------------------//
BOOL cQuadTree::SubDivide()
{
	int		nTopEdgeCenter;
	int		nBottomEdgeCenter;
	int		nLeftEdgeCenter;
	int		nRightEdgeCenter;
	int		nCentralPoint;

	// ��ܺ� ���
	nTopEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] ) / 2;
	// �ϴܺ� ��� 
	nBottomEdgeCenter	= ( m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 2;
	// ������ ���
	nLeftEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL] ) / 2;
	// ������ ���
	nRightEdgeCenter	= ( m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR] ) / 2;
	// �Ѱ��
	nCentralPoint		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
							m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;

	// ���̻� ������ �Ұ����Ѱ�? �׷��ٸ� SubDivide() ����
	if( abs(m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1 )
	{
		return FALSE;
	}

	// 4���� �ڽĳ�� �߰�
	m_pChild[CORNER_TL] = AddChild( m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint );
	m_pChild[CORNER_TR] = AddChild( nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter );
	m_pChild[CORNER_BL] = AddChild( nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter );
	m_pChild[CORNER_BR] = AddChild( nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR] );

	return TRUE;
}


//-----------------------------------------------------------------------------//
/// ����� �������� �ε����� �����Ѵ�.
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
		// ���� ������ ����� �κк���(subdivide)�� �Ұ����ϹǷ� �׳� ����ϰ� �����Ѵ�.
		if( abs(m_nCorner[CORNER_TR]-m_nCorner[CORNER_TL]) <= 1 )
		{
			// ������� �ﰢ��
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;
			// �����ϴ� �ﰢ��
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
// QuadTree�� �����Ѵ�.
//-----------------------------------------------------------------------------//
BOOL cQuadTree::Build()
{
	BuildQuadTree();

	return TRUE;
}


//-----------------------------------------------------------------------------//
//	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�.
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
// ����Ʈ���� �����.(Build()�Լ����� �Ҹ���)
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

