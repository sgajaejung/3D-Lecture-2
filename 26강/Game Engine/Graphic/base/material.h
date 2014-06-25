
#pragma once

namespace graphic
{

	class cMaterial
	{
	public:
		cMaterial();
		virtual ~cMaterial();
		
		void Init(const Vector4 &ambient, 
			const Vector4 &diffuse,
			const Vector4 &specular,
			const Vector4 &emmisive=Vector4(0,0,0,1),
			const float pow=0);

		void Init( D3DMATERIAL9 &mtrl );

		void InitWhite();
		void InitRed();
		void InitBlue();
		void Bind();

		D3DMATERIAL9 m_mtrl;
	};

}
