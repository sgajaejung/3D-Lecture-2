#pragma once


namespace graphic
{

	struct sVertexDiffuse
	{
		Vector3 p;
		DWORD c;
		enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};

	struct sVertexTex
	{
		Vector3 p;
		float u, v;
		enum {FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

		sVertexTex() {}
		sVertexTex(float x, float y, float z, float tu0, float tv0) :
			p(Vector3(x,y,z)), u(tu0), v(tv0) { }
	};

	struct sVertexNormDiffuse
	{
		Vector3 p;
		Vector3 n;
		DWORD c;
		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE };
	};

	struct sVertexNormTex
	{
		Vector3 p;
		Vector3 n;
		float u,v;

		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
	};


}
