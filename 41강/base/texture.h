#pragma once

namespace graphic
{
	class cTexture
	{
	public:
		cTexture();
		virtual ~cTexture();

		bool Create(const string &fileName);
		bool Create(const int width, const int height, const D3DFORMAT format);
		void Bind(int stage);
		void Clear();

		void Lock(D3DLOCKED_RECT &out);
		void Unlock();
		

	private:
		IDirect3DTexture9* m_texture;
	};
}
