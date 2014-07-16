
#include "stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter() :
	m_weapon(NULL)
,	m_weaponNode(NULL)
{

}

cCharacter::~cCharacter()
{
	SAFE_DELETE(m_weapon);
}


bool cCharacter::Create(const string &modelName)
{
	m_weaponNode = NULL;
	return cModel::Create(modelName);
}


void cCharacter::LoadWeapon(const string &fileName)
{
	SAFE_DELETE(m_weapon);

	RET(!m_bone);
	m_weaponNode = m_bone->FindBone("dummy_weapon");
	RET(!m_weaponNode);

	if (!m_weapon)
		m_weapon = new cModel();

	if (!m_weapon->Create(fileName))
		return;
}


bool cCharacter::Move(const float elapseTime)
{
	cModel::Move(elapseTime);
	
	if (m_weapon && m_weaponNode)
	{
		Matrix44 mat = m_weaponNode->GetAccTM();
		m_weapon->SetTM(mat * m_matTM);
		m_weapon->Move(elapseTime);
	}
	return true;
}


void cCharacter::Render()
{
	cModel::Render();

	if (m_weapon)
		m_weapon->Render();
}
