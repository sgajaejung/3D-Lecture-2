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
		void CreateMaterials(const sRawMesh &rawMesh);
		void CreateAttributes(const sRawMesh &rawMesh);


	protected:
		bool m_isSkinned;
		vector<cMaterial> m_mtrls;
		vector<cTexture*>m_textures;  // reference
		vector<sAttribute> m_attributes;
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
	};

}
