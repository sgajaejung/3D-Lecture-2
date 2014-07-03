#pragma once


namespace graphic
{
	class cBoneNode;

	class cBone
	{
	public:
		cBone(const int id, const sRawMeshGroup &rawMeshes);
		virtual ~cBone();

		void SetAnimation( const sRawAniGroup &rawAnies, int nAniFrame );
		bool Move(const float elapseTime);
		void Render(const Matrix44 &parentTm);
		void Clear();	
		vector<Matrix44>& GetPalette();


	protected:
		void SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAni, int nAniFrame );


	private:
		cBoneNode *m_root;
		int m_id;
		vector<Matrix44> m_palette;
	};


	inline vector<Matrix44>& cBone::GetPalette() { return m_palette; }
}
