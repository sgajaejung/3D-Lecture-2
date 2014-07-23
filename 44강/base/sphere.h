#pragma once


namespace graphic
{

	class cSphere
	{
	public:
		cSphere();
		cSphere(const float radius, const int numSegments, const int numSlices);
		virtual ~cSphere();

		void Create(const float radius, const int numSegments, const int numSlice);
		void SetTransform( const Matrix44 &tm );
		const Matrix44& GetTransform() const;

		void Render(const Matrix44 &tm);


	protected:


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		Matrix44 m_tm;
		float m_radian;
		int m_numSlice;
	};	


	inline void cSphere::SetTransform( const Matrix44 &tm ) { m_tm = tm; }
	inline const Matrix44& cSphere::GetTransform() const { return m_tm; }
}
