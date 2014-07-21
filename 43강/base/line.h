#pragma once

// 정육면체 메쉬로 라인을 그리는 클래스다.

namespace graphic
{

	class cLine
	{
	public:
		cLine(const DWORD color=0);
		cLine(const Vector3 &p0, const Vector3 &p1, const float width, const DWORD color=0);
		virtual ~cLine();

		void Render();
		void SetLine(const Vector3 &p0, const Vector3 &p1, const float width);


	protected:
		void InitCube();


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		Vector3 m_p0;
		Vector3 m_p1;
		float m_width;
		DWORD m_color;
		Matrix44 m_tm;
	};

}
