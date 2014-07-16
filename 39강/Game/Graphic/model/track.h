#pragma once


namespace graphic
{

	DECLARE_TYPE_NAME_SCOPE(graphic, cTrack)
	class cTrack : public memmonitor::Monitor<cTrack, TYPE_NAME(cTrack)>
	{
	public:
		cTrack(const sRawAni &rawAni);
		virtual ~cTrack();

		void InitAnimation();
		void Move( const int curFrame, OUT Matrix44 &out );
		int GetCurrentFrame() const;
		void SetCurrentFramePos( const int curFrame );


	protected:
		bool GetPosKey( const int curFrame, OUT Vector3 &out );
		bool GetRotKey( const int curFrame, OUT Quaternion &out );
		bool GetScaleKey( const int curFrame, OUT Vector3 &out );
		inline float GetAlpha( float f1, float f2, float frame ) const;


	private:
		const sRawAni &m_rawAni; // reference
		Matrix44 m_matAni;
		int m_curFrame;

		// 에니메이션의 다음 Key index를 나타낸다.
		int m_keyPosIdx;
		int m_keyRotIdx;
		int m_keyScaleIdx;

		// 에니메이션에서 현재 Key와 다음 Key를 저장한다.
		sKeyPos *m_curKeyPos[ 2];
		sKeyRot *m_curKeyRot[ 2];
		sKeyScale *m_curKeyScale[ 2];

		// 새 에니메이션과 보간을 위해 필요함
		sKeyPos m_TempPos[ 2];
		sKeyRot m_TempRot[ 2];
		sKeyScale m_TempScale[ 2];

	};


	inline int cTrack::GetCurrentFrame() const { return m_curFrame; }
}
