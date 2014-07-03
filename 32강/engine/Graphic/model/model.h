#pragma once


namespace graphic
{
	class cBoneMgr;

	DECLARE_TYPE_NAME_SCOPE(graphic, cModel)
	class cModel : public memmonitor::Monitor<cModel, TYPE_NAME(cModel)>
	{
	public:
		cModel();
		virtual ~cModel();

		bool Create(const string &modelName);
		virtual bool Move(const float elapseTime);
		virtual void Render();
		void Clear();

		void SetTM(const Matrix44 &tm);
		void MultiplyTM(const Matrix44 &tm);
		const Matrix44& GetTM();
		cBoneMgr* GetBone();


	private:
		vector<cNode*> m_meshes;
		cBoneMgr *m_bone;
		Matrix44 m_matTM;
	};


	inline void cModel::SetTM(const Matrix44 &tm) { m_matTM = tm; }
	inline void cModel::MultiplyTM(const Matrix44 &tm) { m_matTM *= tm; }
	inline const Matrix44& cModel::GetTM() { return m_matTM; }
	inline cBoneMgr* cModel::GetBone() { return m_bone; }
}
