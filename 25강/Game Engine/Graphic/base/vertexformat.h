#pragma once


namespace graphic
{

	struct sVertexDiffuse
	{
		Vector3 p;
		DWORD c;
		enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};


	struct sVertexNormTex
	{
		Vector3 p;
		Vector3 n;
		float u,v;

		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
	};


}
