#pragma once


namespace graphic
{

	class cRigidMesh : public cMesh
	{
	public:
		cRigidMesh(const int id, const sRawMesh &raw);
		virtual ~cRigidMesh();

		void LoadAnimation( const sRawAni &rawAni );
		virtual bool Move(const float elapseTime) override;


	private:
		// animation
		cTrack *m_track;
		int m_aniStart;
		int m_aniEnd;
		int m_aniFrame;
		float m_aniTime;
	};

}
