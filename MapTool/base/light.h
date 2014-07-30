
#pragma once

namespace graphic
{

	class cLight
	{
	public:
		cLight();
		virtual ~cLight();

		enum TYPE 
		{
			LIGHT_POINT = 1,
			LIGHT_SPOT = 2,
			LIGHT_DIRECTIONAL = 3,
		};

		void Init(TYPE type, 
			const Vector4 &ambient,
			const Vector4 &diffuse,
			const Vector4 &specular,
			const Vector3 &direction);

		void SetDirection( const Vector3 &direction );
		void Bind(int lightIndex);

		D3DLIGHT9 m_light;
	};

}
