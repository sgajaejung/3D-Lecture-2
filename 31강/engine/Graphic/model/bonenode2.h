#pragma once


namespace graphic
{

	class cBoneNode : public cNode
	{
	public:
		cBoneNode(const int id, vector<Matrix44> &palette, const sRawMesh &rawMesh);
		virtual ~cBoneNode();

		void SetAnimation( const sRawAni &rawAni, int nAniFrame, bool bLoop=false );
		const Matrix44& GetAccTM() const;
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;


	private:
		cTrack *m_track;
		cMesh *m_mesh;
		vector<Matrix44> &m_palette;
		Matrix44 m_accTM;	// 누적된 TM
		Matrix44 m_offset;	// inverse( m_matWorld )
		int m_aniStart; // 프래임 시작시간 (프레임)
		int m_aniEnd; // 프래임 종료시간 (프레임)
		int m_totalPlayTime; // 총 에니메이션 될 프레임

		int m_curPlayFrame; // 현재 에니메이션 프래임 (AniEnd를 지나면 0으로 초기화된다.)
		int m_incPlayFrame; // 에니메이션 중인 총 프레임
		float m_curPlayTime; // 현재 애니메이션 시간 (m_aniEnd 에 도달하면 0 이 된다.)
		float m_incPlayTime; // 현재 애니메이션 시간 (총 시간)

		bool m_isAni; // TRUE일경우만 에니메이션이 된다.
		bool m_isLoop; // 에니메이션 반복 여부
	};

	
	inline const Matrix44& cBoneNode::GetAccTM() const { return m_accTM; }
}
