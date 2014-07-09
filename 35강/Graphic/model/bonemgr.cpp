
#include "stdafx.h"
#include "bonemgr.h"
#include "bonenode.h"

using namespace  graphic;


cBoneMgr::cBoneMgr(const int id, const sRawMeshGroup &rawMeshes) :
	m_root(NULL)
,	m_id(id)
{
	m_palette.resize(rawMeshes.bones.size());

	m_bones.resize(rawMeshes.bones.size(), NULL);

	for (u_int i=0; i < rawMeshes.bones.size(); ++i)
	{
		const int id = rawMeshes.bones[ i].id;
		const int parentId = rawMeshes.bones[ i].parentId;
		if (m_root && (parentId < 0))
			continue;

		cBoneNode *bone = new cBoneNode(id, m_palette, rawMeshes.bones[ i]);
		SAFE_DELETE(m_bones[ id]);
		m_bones[ id] = bone;

		if (-1 >=  parentId) // root
			m_root = bone;
		else
			m_bones[ parentId]->InsertChild( bone );
	}

}

cBoneMgr::~cBoneMgr()
{
	Clear();
}


// ���ϸ��̼� ����.
void cBoneMgr::SetAnimation( const sRawAniGroup &rawAnies, int nAniFrame )
{
	SetAnimationRec( m_root, rawAnies, nAniFrame );
}


// �ִϸ��̼� ����.
void cBoneMgr::SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAnies, int nAniFrame )
{
	RET(!node);
	RET(node->GetId() >= (int)rawAnies.anies.size());

	node->SetAnimation( rawAnies.anies[ node->GetId()], nAniFrame, true );
	BOOST_FOREACH (auto p, node->GetChildren())
	{
		SetAnimationRec((cBoneNode*)p, rawAnies, nAniFrame );
	}
}


void cBoneMgr::SetCurrentAnimationFrame(const int curFrame)
{
	BOOST_FOREACH (auto p, m_bones)
	{
		if (p)
			p->SetCurrentFrame(curFrame);
	}
}


// �ִϸ��̼�
bool cBoneMgr::Move(const float elapseTime)
{
	RETV(!m_root, false);
	return m_root->Move(elapseTime);
}


// ���̷��� ���.
void cBoneMgr::Render(const Matrix44 &parentTm)
{
	RET(!m_root);
	m_root->Render(parentTm);
}


// BoneNode ã�Ƽ� ����.
cBoneNode* cBoneMgr::FindBone(const int id)
{
	RETV(!m_root, NULL);
	return (cBoneNode*)m_root->FindNode(id);
}


cBoneNode* cBoneMgr::FindBone(const string &name)
{
	RETV(!m_root, NULL);
	return (cBoneNode*)m_root->FindNode(name);
}


// �������� �Ҵ�� ��ü ����.
void cBoneMgr::Clear()
{
	SAFE_DELETE(m_root);
	m_bones.clear();
}
