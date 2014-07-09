#pragma once

#include "node.h"

namespace graphic
{

	class cMesh : public cNode
	{
	public:
		cMesh(const int id, const sRawMesh &rawMesh);
		cMesh(const int id, const sRawBone &rawBone);
		virtual ~cMesh();

		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;
		void RenderBBox();


	protected:
		void CreateMesh( const vector<Vector3> &vertices, 
			const vector<Vector3> &normals, 
			const vector<Vector3> &tex,
			const vector<int> &indices );


	protected:
		bool m_isSkinned;
		cMaterial m_mtrl;
		cTexture m_texture;
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
	};

}
