#pragma once

namespace graphic
{

	class cRenderer : public common::cSingleton<cRenderer>
	{
	public:
		cRenderer();
		virtual ~cRenderer();

		bool CreateDirectX(HWND hWnd, const int width, const int height);
		void Update(const float elpaseT);
		LPDIRECT3DDEVICE9 GetDevice();
		HWND GetHwnd() const;


	private:
		LPDIRECT3DDEVICE9 m_pDevice;		
		HWND m_hWnd;
	};


	// 렌더러 초기화.
	bool InitRenderer(HWND hWnd, const int width, const int height);
	void ReleaseRenderer();
	inline cRenderer* GetRenderer() { return cRenderer::Get(); }
	inline LPDIRECT3DDEVICE9 GetDevice() { return cRenderer::Get()->GetDevice(); }
	inline LPDIRECT3DDEVICE9 cRenderer::GetDevice() { return m_pDevice; }
	inline HWND cRenderer::GetHwnd() const { return m_hWnd; }

}
