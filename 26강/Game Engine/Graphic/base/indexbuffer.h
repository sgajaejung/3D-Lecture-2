#pragma once

namespace graphic
{

	class cIndexBuffer
	{
	public:
		cIndexBuffer();
		virtual ~cIndexBuffer();

		bool Create(int faceCount);
		void* Lock();
		void Unlock();
		void Bind() const;
		int GetFaceCount() const;
		void Clear();


	private:
		LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; // �ε��� ����
		int m_faceCount;
	};


	inline int cIndexBuffer::GetFaceCount() const { return m_faceCount; }
}
