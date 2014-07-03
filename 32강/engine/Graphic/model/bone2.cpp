
#include "stdafx.h"
#include "bone2.h"
#include "bonenode2.h"

using namespace  graphic;


cBone::cBone(const int id, const sRawMeshGroup &rawMeshes) :
	m_root(NULL)
,	m_id(id)
{
	m_palette.resize(rawMeshes.bones.size());

	vector<cBoneNode*> vec(rawMeshes.bones.size(), NULL);
	for (u_int i=0; i < rawMeshes.bones.size(); ++i)
	{
		const int id = rawMeshes.bones[ i].id;
		const int parentId = rawMeshes.bones[ i].parentId;
		cBoneNode *bone = new cBoneNode(id, m_palette, rawMeshes.bones[ i]);
		SAFE_DELETE(vec[ id]);
		vec[ id] = bone;

		if (-1 >=  parentId) // root
			m_root = bone;
		else
			vec[ parentId]->InsertChild( bone );
	}

}

cBone::~cBone()
{
	Clear();
}


// 에니메이션 설정.
void cBone::SetAnimation( const sRawAniGroup &rawAnies, int nAniFrame )
{
	SetAnimationRec( m_root, rawAnies, nAniFrame );
}


// 애니메이션 설정.
void cBone::SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAnies, int nAniFrame )
{
	RET(!node);
	RET(node->GetId() >= (int)rawAnies.anies.size());

	node->SetAnimation( rawAnies.anies[ node->GetId()], nAniFrame, true );
	BOOST_FOREACH (auto p, node->GetChildren())
	{
		SetAnimationRec((cBoneNode*)p, rawAnies, nAniFrame );
	}
}


// 애니메이션
bool cBone::Move(const float elapseTime)
{
	RETV(!m_root, false);
	return m_root->Move(elapseTime);
}


// 스켈레톤 출력.
void cBone::Render(const Matrix44 &parentTm)
{
	RET(!m_root);
	m_root->Render(parentTm);
}


// 동적으로 할당된 객체 제거.
void cBone::Clear()
{
	SAFE_DELETE(m_root);
}
