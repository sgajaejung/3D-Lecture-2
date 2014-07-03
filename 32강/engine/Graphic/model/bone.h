#pragma once


namespace graphic
{
	class cBoneNode;

	class cBone
	{
	public:
		cBone(const int id, const sRawMeshGroup &rawMeshes);
		virtual ~cBone();

		bool Move(const float elapseTime);
		void Render(const Matrix44 &parentTm);
		void Clear();	
		cBoneNode* FindBone(const int id);


	private:
		cBoneNode *m_root;
		int m_id;
	};

}
