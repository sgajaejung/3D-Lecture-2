#pragma once


namespace graphic
{

	void RenderAxis();

	void RenderFPS(int timeDelta);

	bool InitDirectX(HWND hWnd, int width, int height, LPDIRECT3DDEVICE9 *out);

}
