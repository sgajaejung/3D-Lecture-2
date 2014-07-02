#pragma once


namespace graphic
{

	class cBoneNode : public cNode
	{
	public:
		cBoneNode(const int id, const sRawMesh &rawMesh);
		virtual ~cBoneNode();

		const Matrix44& GetAccTM() const;
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;


	private:
		cMesh *m_mesh;
		Matrix44 m_accTM;	// ´©ÀûµÈ TM
		Matrix44 m_offset;	// inverse( m_matWorld )
	};

	
	inline const Matrix44& cBoneNode::GetAccTM() const { return m_accTM; }
}
