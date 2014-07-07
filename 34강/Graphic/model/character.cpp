
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


void cCharacter::LoadWeapon(const string &fileName)
{
	if (!m_weapon)
		m_weapon = new cModel();

	if (!m_weapon->Create(fileName))
		return;

	RET(!m_bone);

	m_weaponNode = m_bone->FindBone("dummy_weapon");
	RET(!m_weaponNode);
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
