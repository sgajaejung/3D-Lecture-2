
#include "stdafx.h"
#include "bone.h"
#include "bonenode.h"

using namespace  graphic;


cBone::cBone(const int id, const sRawMeshGroup &rawMeshes) :
	m_root(NULL)
,	m_id(id)
{
	vector<cBoneNode*> vec(rawMeshes.bones.size(), NULL);
	for (u_int i=0; i < rawMeshes.bones.size(); ++i)
	{
		const int id = rawMeshes.bones[ i].id;
		const int parentId = rawMeshes.bones[ i].parentId;
		cBoneNode *bone = new cBoneNode(id, rawMeshes.bones[ i]);
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


// �ִϸ��̼�
bool cBone::Move(const float elapseTime)
{
	RETV(!m_root, false);
	return m_root->Move(elapseTime);
}


// ���̷��� ���.
void cBone::Render(const Matrix44 &parentTm)
{
	RET(!m_root);
	m_root->Render(parentTm);
}


// �������� �Ҵ�� ��ü ����.
void cBone::Clear()
{
	SAFE_DELETE(m_root);
}


// BoneNode ã�Ƽ� ����.
cBoneNode* cBone::FindBone(const int id)
{
	RETV(!m_root, NULL);
	return (cBoneNode*)m_root->FindNode(id);
}
