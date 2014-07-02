
#include "stdafx.h"
#include "track.h"

using namespace graphic;


cTrack::cTrack(const sRawAni &rawAni) :
	m_rawAni(rawAni)
,	m_curFrame(0)
,	m_keyPosIdx(-1)
,	m_keyRotIdx(-1)
,	m_keyScaleIdx(-1)
{
	InitAnimation();

}

cTrack::~cTrack()
{

}


// Animation
void cTrack::Move( const int curFrame, OUT Matrix44 &out )
{
	m_curFrame = curFrame;

	Quaternion q;
	if (GetRotKey(curFrame, q))
		out = q.GetMatrix();

	Vector3 p(0,0,0);
	if (GetPosKey(curFrame, p))
	{
		out._41 = p.x;
		out._42 = p.y;
		out._43 = p.z;
	}

	Vector3 s(1,1,1);
	if (GetScaleKey(curFrame, s))
	{
		out._11 *= s.x;
		out._22 *= s.y;
		out._33 *= s.z;
	}
}


// curFrame 일 대의 위치 값을 얻어온다.
bool cTrack::GetPosKey( const int curFrame, OUT Vector3 &out )
{
	RETV(!m_curKeyPos[ 0], false);

	if (!m_curKeyPos[ 1])
	{
		out = m_curKeyPos[ 0]->p;
		return true;
	}

	// 프레임이 다음 노드를 가리키면 다음 에니메이션으로 넘어간다.
	if (curFrame > m_curKeyPos[ 1]->t)
	{
		// 에니메이션의 마지막이면 마지막 키값을 리턴한다.
		if ((int)m_rawAni.pos.size() <= m_keyPosIdx+1 )
		{
			out = m_curKeyPos[ 1]->p;
			return true;
		}

		// 다음 에니메이션으로 넘어간다.
		++m_keyPosIdx;
		m_curKeyPos[ 0] = m_curKeyPos[ 1];
		m_curKeyPos[ 1] = (sKeyPos*)&m_rawAni.pos[ m_keyPosIdx];
	}

	// curKeyPos[0]->t 에서 curKeyPos[1]->t 사이에서 curFrame이 어디에 위치하는지 0~1 사이 값으로 
	// 리턴한다.
	const float alpha = GetAlpha( m_curKeyPos[ 0]->t, m_curKeyPos[ 1]->t, (float)curFrame );
	out = m_curKeyPos[ 0]->p.Interpolate( m_curKeyPos[ 1]->p, alpha );
	return true;
}


//  curFrame 일 때의 회전 값을 얻어온다.
bool cTrack::GetRotKey( const int curFrame, OUT Quaternion &out )
{
	RETV(!m_curKeyRot[ 0], false);

	if (!m_curKeyRot[ 1])
	{
		out = m_curKeyRot[ 0]->q;
		return true;
	}

	// 프레임이 다음 노드를 가리키면 다음 에니메이션으로 넘어간다.
	if (curFrame > m_curKeyRot[ 1]->t)
	{
		// 에니메이션의 마지막이면 마지막 키값을 리턴한다.
		if ((int)m_rawAni.rot.size() <= m_keyRotIdx+1 )
		{
			out = m_curKeyRot[ 1]->q;
			return true;
		}

		// 다음 에니메이션으로 넘어간다.
		++m_keyRotIdx;
		m_curKeyRot[ 0] = m_curKeyRot[ 1];
		m_curKeyRot[ 1] = (sKeyRot*)&m_rawAni.rot[ m_keyRotIdx];
	}

	const float alpha = GetAlpha( m_curKeyRot[ 0]->t, m_curKeyRot[ 1]->t, (float)curFrame );
	out = m_curKeyRot[ 0]->q.Interpolate( m_curKeyRot[ 1]->q, alpha );
	return true;
}


// curFrame 일 때의 스케일 값을 얻어온다.
bool cTrack::GetScaleKey( const int curFrame, OUT Vector3 &out )
{
	RETV(!m_curKeyScale[ 0], false);

	if (!m_curKeyScale[ 1])
	{
		out = m_curKeyScale[ 0]->s;
		return true;
	}

	// 프레임이 다음 노드를 가리키면 다음 에니메이션으로 넘어간다.
	if (curFrame > m_curKeyScale[ 1]->t)
	{
		// 에니메이션의 마지막이면 마지막 키값을 리턴한다.
		if ((int)m_rawAni.scale.size() <= m_keyScaleIdx+1)
		{
			out = m_curKeyScale[ 1]->s;
			return true;
		}

		// 다음 에니메이션으로 넘어간다.
		++m_keyScaleIdx;
		m_curKeyScale[ 0] = m_curKeyScale[ 1];
		m_curKeyScale[ 1] = (sKeyScale*)&m_rawAni.scale[ m_keyScaleIdx];
	}

	const float alpha = GetAlpha( m_curKeyScale[ 0]->t, m_curKeyScale[ 1]->t, (float)curFrame );
	out = m_curKeyScale[ 0]->s.Interpolate( m_curKeyScale[ 1]->s, alpha );
	return true;
}


// 애니메이션 초기화, 처음으로 돌아간다.
void cTrack::InitAnimation()
{
	ZeroMemory( m_curKeyPos, sizeof(m_curKeyPos) );
	ZeroMemory( m_curKeyRot, sizeof(m_curKeyRot) );
	ZeroMemory( m_curKeyScale, sizeof(m_curKeyScale) );

	if (1 <= m_rawAni.pos.size())
	{
		m_keyPosIdx = 1;
		m_curKeyPos[ 0] = (sKeyPos*)&m_rawAni.pos[ 0];
		if (2 <= m_rawAni.pos.size())
			m_curKeyPos[ 1] = (sKeyPos*)&m_rawAni.pos[ 1];
	}

	if (1 <= m_rawAni.rot.size())
	{
		m_keyRotIdx = 1;
		m_curKeyRot[ 0] = (sKeyRot*)&m_rawAni.rot[ 0];
		if (2 <= m_rawAni.rot.size())
			m_curKeyRot[ 1] = (sKeyRot*)&m_rawAni.rot[ 1];
	}
	
	if (1 <= m_rawAni.scale.size())
	{
		m_keyScaleIdx = 1;
		m_curKeyScale[ 0] = (sKeyScale*)&m_rawAni.scale[ 0];
		if (2 <= m_rawAni.scale.size())
			m_curKeyScale[ 1] = (sKeyScale*)&m_rawAni.scale[ 1];
	}
}


// Get Alpha
float cTrack::GetAlpha(float f1, float f2, float frame ) const 
{
	if (FLOAT_EQ(f1, f2))
		return 0;
	return (frame-f1) / (f2-f1); 
}
