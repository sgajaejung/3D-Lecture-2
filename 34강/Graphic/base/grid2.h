#pragma once

// grid format
// vertex + normal + texture

namespace graphic
{

	class cGrid2
	{
	public:
		cGrid2();
		virtual ~cGrid2();

		void Create( const int rowCellCount, const int colCellCount, const float cellSize, 
			const float textureUVFactor=8.f);
		void Render(const int stage=0);

		cVertexBuffer& GetVertexBuffer();
		cIndexBuffer& GetIndexBuffer();
		cTexture& GetTexture();
		cMaterial& GetMaterial();


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		cTexture m_tex;
		cMaterial m_mtrl;

		int m_rowCellCount;
		int m_colCellCount;
		float m_cellSize;
	};


	inline cVertexBuffer& cGrid2::GetVertexBuffer() { return m_vtxBuff; }
	inline cIndexBuffer& cGrid2::GetIndexBuffer() { return m_idxBuff; }
	inline cTexture& cGrid2::GetTexture() { return m_tex; }
	inline cMaterial& cGrid2::GetMaterial() { return m_mtrl; }
}
