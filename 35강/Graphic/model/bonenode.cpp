
#include "stdafx.h"
#include "bonenode.h"


using namespace graphic;


cBoneNode::cBoneNode(const int id, vector<Matrix44> &palette, const sRawBone &rawBone) :
	cNode(id, rawBone.name)
,	m_track(NULL)
,	m_mesh(NULL)
,	m_palette(palette)
,	m_aniStart(0)
,	m_aniEnd(0)
,	m_curPlayFrame(0)
,	m_incPlayFrame(0)
,	m_totalPlayTime(0)
,	m_curPlayTime(0)
,	m_incPlayTime(0)
,	m_isAni(false)
,	m_isLoop(false)
{
	m_offset = rawBone.worldTm.Inverse();
	m_localTM = rawBone.localTm;

	m_mesh = new cMesh(id, rawBone);

}

cBoneNode::~cBoneNode()
{
	SAFE_DELETE( m_track );
	SAFE_DELETE( m_mesh );
}


// 애니메이션 설정.
void cBoneNode::SetAnimation( const sRawAni &rawAni, int nAniFrame, bool bLoop)
{
	int aniend = 0;
	if (0 == nAniFrame)
	{
		m_totalPlayTime = (int)rawAni.end;
		aniend = (int)rawAni.end;
	}
	else
	{
		m_totalPlayTime = nAniFrame;
		aniend = (int)rawAni.end;
	}

	m_aniStart = 0;
	m_aniEnd	 = aniend;
	m_incPlayFrame = 0;

	m_isLoop = bLoop;
	m_isAni = true;

	m_curPlayFrame = 0;

	SAFE_DELETE(m_track)
	m_track = new cTrack(rawAni);
}


// 에니메이션.
bool cBoneNode::Move(const float elapseTime)
{
	RETV(!m_isAni, true);
	RETV(!m_track, true);

	m_curPlayTime += elapseTime;
	m_incPlayTime += elapseTime;
	m_curPlayFrame = (int)(m_curPlayTime * 30.f);
	m_incPlayFrame = (int)(m_incPlayTime * 30.f);

	BOOL ani_loop_end = (m_curPlayFrame > m_aniEnd);	// 에니메이션 끝까지 갔다면 TRUE
	BOOL ani_end = (!m_isLoop) && (m_incPlayFrame > m_totalPlayTime);	// 총에니메이션 시간이 지났다면 TRUE

	if (ani_loop_end || ani_end)
	{
		// 보간에니메이션이 종료된후 반복 에니메이션이라면
		// 보간없이 에니메이션을 처음으로 돌린다.
		if (m_isLoop)
		{
			m_curPlayFrame = (int)(m_aniStart * 30.f);
			m_curPlayTime = 0;
			m_track->InitAnimation();
		}
		else
		{
			// 반복 에니메이션이 아니라면 
			// 총 에니메이션 시간이 지나면 에니메이션을 종료하고 FALSE를 리턴한다.
			// 그렇지 않다면 에니메이션을 처음으로 돌린다.				
			if (ani_loop_end)
			{
				m_curPlayFrame = (int)(m_aniStart * 30.f);
				m_curPlayTime = 0;

				// 총 에니메이션이 끝나지 않았다면 에니메이션 정보를 처음으로 되돌린다.
				// 총 에니메이션이 끝났다면 정보를 되돌리지 않고 마지막 프레임을 향하게 내버려둔다.
				// 다음 에니메이션에서 보간되기 위해서 마지막 프레임으로 두어야 한다.
				if (!ani_end)
					m_track->InitAnimation();
			}
			if (ani_end)
			{
				m_isAni = false;
				return false;
			}
		}
	}

	m_aniTM.SetIdentity();

	m_track->Move( m_curPlayFrame, m_aniTM );

	m_accTM = m_localTM * m_aniTM * m_TM;

	// 만약 pos키값이 없으면 local TM의 좌표를 사용한다
	if( m_aniTM._41 == 0.0f && m_aniTM._42 == 0.0f && m_aniTM._43 == 0.0f )
	{
		m_accTM._41 = m_localTM._41;
		m_accTM._42 = m_localTM._42;
		m_accTM._43 = m_localTM._43;
	}
	else	// pos키값을 좌표값으로 적용한다(이렇게 하지 않으면 TM의 pos성분이 두번적용된다)
	{
		m_accTM._41 = m_aniTM._41;
		m_accTM._42 = m_aniTM._42;
		m_accTM._43 = m_aniTM._43;
	}

	if (m_parent)
		m_accTM = m_accTM * ((cBoneNode*)m_parent)->m_accTM;

	m_palette[ m_id] = m_offset * m_accTM;

	//if (m_pBox)
	//	m_pBox->SetWorldTM(&m_pPalette[ m_nId]);

	BOOST_FOREACH (auto p, m_children)
		p->Move( elapseTime );

	return true;
}


void cBoneNode::Render(const Matrix44 &parentTm)
{
	RET(!m_mesh);

	if (m_track)
		m_mesh->Render(m_offset * m_accTM * parentTm);
	else
		m_mesh->Render(parentTm);

	BOOST_FOREACH (auto p, m_children)
		p->Render( parentTm );
}


void cBoneNode::SetCurrentFrame(const int curFrame) 
{ 
	m_curPlayTime = curFrame / 30.f;
	m_curPlayFrame = curFrame; 
	if (m_track)
		m_track->SetCurrentFramePos(curFrame);
}
