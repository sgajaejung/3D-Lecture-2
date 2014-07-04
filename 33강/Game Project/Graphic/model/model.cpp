
#include "stdafx.h"
#include "model.h"
#include "node.h"
#include "../manager/resourcemanager.h"
#include "mesh.h"
#include "bone.h"


using namespace graphic;


cModel::cModel() :
	m_bone(NULL)
{
	
}

cModel::~cModel()
{
	Clear();
}


bool cModel::Create(const string &modelName)
{
	sRawMeshGroup *rawMeshes = cResourceManager::Get()->LoadModel(modelName);
	RETV(!rawMeshes, false);

	Clear();

	const bool isSkinnedMesh = !rawMeshes->bones.empty();

	// 스키닝 애니메이션이면 Bone을 생성한다.
	if (isSkinnedMesh)
	{
		m_bone = new cBone(0, *rawMeshes);

		if (sRawAniGroup *rawAnies = cResourceManager::Get()->FindAni(modelName))
		{
			if (!rawAnies->anies.empty())
				m_bone->SetAnimation(*rawAnies, 0);
		}
	}

	// 메쉬 생성.
	BOOST_FOREACH (auto &mesh, rawMeshes->meshes)
	{
		cMesh *p = NULL;
		if (isSkinnedMesh)
		{
			p = new cSkinnedMesh(0, m_bone->GetPalette(), mesh);
		}
		else
		{
			p = new cRigidMesh(0, mesh);
			if (sRawAniGroup *rawAnies = cResourceManager::Get()->FindAni(modelName))
			{
				if (!rawAnies->anies.empty())
					((cRigidMesh*)p)->LoadAnimation(rawAnies->anies[0]);
			}
		}

		if (p)
			m_meshes.push_back(p);
	}

	return true;
}


bool cModel::Move(const float elapseTime)
{
	BOOST_FOREACH (auto node, m_meshes)
		node->Move(elapseTime);

	if (m_bone)
		m_bone->Move(elapseTime);

	return true;
}


void cModel::Render()
{
	Matrix44 identity;
	GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&identity);

	BOOST_FOREACH (auto node, m_meshes)
		node->Render(m_matTM);

	if (m_bone)
		m_bone->Render(m_matTM);
}


// remove all data
void cModel::Clear()
{
	BOOST_FOREACH (auto mesh, m_meshes)
	{
		SAFE_DELETE(mesh);
	}
	m_meshes.clear();

	SAFE_DELETE(m_bone);
}
