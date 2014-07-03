#pragma once


namespace graphic
{
	class cBoneNode;

	class cBoneMgr
	{
	public:
		cBoneMgr(const int id, const sRawMeshGroup &rawMeshes);
		virtual ~cBoneMgr();

		bool Move(const float elapseTime);
		void Render(const Matrix44 &parentTm);
		void Clear();
		cBoneNode* GetRoot();
		cBoneNode* FindBone(const int id);


	private:
		cBoneNode *m_root;
		int m_id;
	};


	inline cBoneNode* cBoneMgr::GetRoot() { return m_root; }
}
