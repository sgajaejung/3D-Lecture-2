#pragma once


namespace graphic
{

	class cMesh
	{
	public:
		cMesh();
		virtual ~cMesh();

		bool Create(const string &fileName);
		void Render(const Matrix44 &tm);
		void Move(const float elapseT);
		Matrix44& GetTM();


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		cMaterial m_mtrl;
		cTexture m_tex;

		Matrix44 m_tm;
	};


	inline Matrix44& cMesh::GetTM() { return m_tm; }
}
