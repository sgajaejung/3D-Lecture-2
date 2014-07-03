#pragma once


namespace graphic
{

	class cBoneNode : public cNode
	{
	public:
		cBoneNode(const int id, vector<Matrix44> &palette, const sRawMesh &rawMesh);
		virtual ~cBoneNode();

		void SetAnimation( const sRawAni &rawAni, int nAniFrame, bool bLoop=false );
		const Matrix44& GetAccTM() const;
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;


	private:
		cTrack *m_track;
		cMesh *m_mesh;
		vector<Matrix44> &m_palette;
		Matrix44 m_accTM;	// ������ TM
		Matrix44 m_offset;	// inverse( m_matWorld )
		int m_aniStart; // ������ ���۽ð� (������)
		int m_aniEnd; // ������ ����ð� (������)
		int m_totalPlayTime; // �� ���ϸ��̼� �� ������

		int m_curPlayFrame; // ���� ���ϸ��̼� ������ (AniEnd�� ������ 0���� �ʱ�ȭ�ȴ�.)
		int m_incPlayFrame; // ���ϸ��̼� ���� �� ������
		float m_curPlayTime; // ���� �ִϸ��̼� �ð� (m_aniEnd �� �����ϸ� 0 �� �ȴ�.)
		float m_incPlayTime; // ���� �ִϸ��̼� �ð� (�� �ð�)

		bool m_isAni; // TRUE�ϰ�츸 ���ϸ��̼��� �ȴ�.
		bool m_isLoop; // ���ϸ��̼� �ݺ� ����
	};

	
	inline const Matrix44& cBoneNode::GetAccTM() const { return m_accTM; }
}
