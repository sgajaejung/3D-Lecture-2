
#pragma once


class cSkyBox
{
public:
	cSkyBox();
	virtual ~cSkyBox();

	bool Create(const string &textureFilePath);
	void Render();


protected:
	bool CreateVertexBuffer();	
	void SetRenderState();


private:
	enum {FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, MAX_FACE};
	graphic::cVertexBuffer m_vtxBuff;
	graphic::cTexture m_textures[ MAX_FACE];
};
