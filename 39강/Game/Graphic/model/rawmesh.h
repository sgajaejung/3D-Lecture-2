#pragma once


namespace graphic
{

	struct sMaterial
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
		float power;
		string texture;

		sMaterial() {}
		sMaterial(const sMaterial &rhs);
		sMaterial& operator=(const sMaterial &rhs);
	};


	struct sAttribute
	{
		int attribId;
		int faceStart;
		int faceCount;
		int vertexStart;
		int vertexCount;
	};


	struct sWeight
	{
		int bone;
		float weight;
	};


	struct sVertexWeight
	{
		int vtxIdx;
		int size;
		sWeight w[6];
	};

	
	struct sRawBone
	{
		int id;
		int parentId;
		string name;
		Matrix44 localTm;
		Matrix44 worldTm;

		// debug 용.
		vector<Vector3> vertices;
		vector<Vector3> normals; // vertex 갯수만큼 저장된다.
		vector<Vector3> tex;
		vector<int> indices;
	};


	struct sRawMesh
	{
		string name;
		vector<Vector3> vertices;
		vector<Vector3> normals; // vertex 갯수만큼 저장된다.
		vector<Vector3> tex;
		vector<int> indices;
		vector<sAttribute> attributes;
		vector<sVertexWeight> weights;
		sMaterial mtrl;
		int mtrlId;
		vector<sMaterial> mtrls;
		vector<int> mtrlIds;
	};


	// 모델하나의 정보를 저장하는 자료구조.
	struct sRawMeshGroup
	{
		string name;
		vector<sMaterial> mtrls;
		vector<sRawMesh> meshes;
		vector<sRawBone> bones;
	};

}
