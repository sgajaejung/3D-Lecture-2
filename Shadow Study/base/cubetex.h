#pragma once


namespace graphic
{

	class cCubeTex
	{
	public:
		cCubeTex();
		cCubeTex(const Vector3 &vMin, const Vector3 &vMax );

		void SetCube(const Vector3 &vMin, const Vector3 &vMax );
		void SetTransform( const Matrix44 &tm );
		const Matrix44& GetTransform() const;
		const Vector3& GetMin() const;
		const Vector3& GetMax() const;

		void Render(const Matrix44 &tm);


	protected:
		void InitCube();


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		Vector3 m_min;
		Vector3 m_max;
		Matrix44 m_tm;
	};	


	inline void cCubeTex::SetTransform( const Matrix44 &tm ) { m_tm = tm; }
	inline const Matrix44& cCubeTex::GetTransform() const { return m_tm; }
	inline const Vector3& cCubeTex::GetMin() const { return m_min; }
	inline const Vector3& cCubeTex::GetMax() const { return m_max; }
}
