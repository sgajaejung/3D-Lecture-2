#pragma once


namespace graphic
{

	class cShader
	{
	public:
		cShader();
		virtual ~cShader();

		bool Create(const string &fileName, const string &technique, const bool showMsgBox=true);
		void Begin();
		void BeginPass(int pass=-1);
		void EndPass();
		void End();

		void SetInt(const string &key, const int val );
		void SetMatrix(const string &key, const Matrix44 &mat);
		void SetTexture(const string &key, cTexture &texture);
		void SetTexture(const string &key, IDirect3DTexture9 *texture);
		void SetFloat(const string &key, float val);
		void SetVector(const string &key, const Vector3 &vec );
		void SetMatrixArray(const string &key, const Matrix44 *mat, const int count );
		void CommitChanges();
		LPD3DXEFFECT GetEffect() { return m_effect; }
		void SetRenderPass(int pass);
		int GetRenderPass() const;


	private:
		LPD3DXEFFECT m_effect;
		D3DXHANDLE m_hTechnique;
		int m_renderPass; // default = 0;
	};


	inline void cShader::SetRenderPass(int pass) { m_renderPass = pass; }
	inline int cShader::GetRenderPass() const { return m_renderPass; }
}
