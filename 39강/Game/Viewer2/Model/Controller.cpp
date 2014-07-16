
#include "stdafx.h"
#include "Controller.h"



cController::cController() : 
	m_character(NULL)
,	m_isPlay(true)
{
	m_character = new graphic::cCharacter();
}

cController::~cController()
{
	SAFE_DELETE(m_character);
}


bool cController::LoadFile( const string &fileName )
{
	bool result = false;
	const graphic::RESOURCE_TYPE::TYPE type = graphic::cResourceManager::Get()->GetFileKind(fileName);
	switch (type)
	{
	case graphic::RESOURCE_TYPE::MESH:
		m_currentMeshFileName = fileName;
		result = m_character->Create(fileName);
		break;

	case graphic::RESOURCE_TYPE::ANIMATION:
		m_currentAnimationFileName = fileName;
		m_character->SetAnimation(fileName);
		result = true;
		break;

	default:
		break;
	}

	NotifyObserver();

	return result;
}


void cController::Render()
{
	RET(!m_character);
	m_character->Render();
}


void cController::Update(const float elapseT)
{
	RET(!m_character);

	if (m_isPlay)
		m_character->Move(elapseT);
	else
		m_character->Move(0);
}


void cController::SetCurrentAnimationFrame(const int curFrame)
{
	RET(!m_character);
	graphic::cBoneMgr *boneMgr = m_character->GetBoneMgr();
	RET(!boneMgr);
	boneMgr->SetCurrentAnimationFrame(curFrame);
}
