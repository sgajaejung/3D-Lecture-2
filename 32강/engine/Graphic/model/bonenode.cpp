
#include "stdafx.h"
#include "bonenode.h"


using namespace graphic;


cBoneNode::cBoneNode(const int id, const sRawMesh &rawMesh) :
	cNode(id)
,	m_mesh(NULL)
{
	m_offset = rawMesh.worldTm.Inverse();
	m_localTM = rawMesh.localTm;

	m_mesh = new cMesh(id, rawMesh);
}

cBoneNode::~cBoneNode()
{
	SAFE_DELETE( m_mesh );
}


// 에니메이션.
bool cBoneNode::Move(const float elapseTime)
{
	m_aniTM.SetIdentity();

	m_accTM = m_localTM * m_aniTM * m_TM;

	if (m_parent)
		m_accTM = m_accTM * ((cBoneNode*)m_parent)->m_accTM;

	BOOST_FOREACH (auto p, m_children)
		p->Move( elapseTime );

	return true;
}


void cBoneNode::Render(const Matrix44 &parentTm)
{
	RET(!m_mesh);
	m_mesh->Render(m_offset * m_accTM  * parentTm);
	BOOST_FOREACH (auto p, m_children)
		p->Render( parentTm );
}
