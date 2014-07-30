#pragma once


namespace graphic
{

	class cShader
	{
	public:
		cShader();
		virtual ~cShader();

		bool Create(const string &fileName, const string &technique);
		void Begin();
		void BeginPass(int pass);
		void EndPass();
		void End();

		void SetMatrix(const string &key, const Matrix44 &mat);
		void SetTexture(const string &key, cTexture &texture);
		void SetFloat(const string &key, float val);
		void SetVector(const string &key, const Vector3 &vec );
		void CommitChanges();
		LPD3DXEFFECT GetEffect() { return m_effect; }


	private:
		LPD3DXEFFECT m_effect;
		D3DXHANDLE m_hTechnique;
	};

}
