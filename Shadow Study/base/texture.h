#pragma once

namespace graphic
{
	class cTexture
	{
	public:
		cTexture();
		virtual ~cTexture();

		bool Create(const string &fileName, bool isSizePow2=true);
		bool Create(const int width, const int height, const D3DFORMAT format);
		void Bind(int stage);
		void Bind(cShader &shader, const string &key);
		void Lock(D3DLOCKED_RECT &out);
		void Unlock();
		IDirect3DTexture9* GetTexture();
		const D3DXIMAGE_INFO& GetImageInfo();
		void Clear();
		

	protected:
		bool CreateEx(const string &fileName);


	private:
		IDirect3DTexture9 *m_texture;
		D3DXIMAGE_INFO m_imageInfo;
	};


	inline IDirect3DTexture9* cTexture::GetTexture() { return m_texture; }
	inline const D3DXIMAGE_INFO& cTexture::GetImageInfo() { return m_imageInfo; }
}
