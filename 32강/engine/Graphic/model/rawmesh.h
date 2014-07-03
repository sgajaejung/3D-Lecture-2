#pragma once


namespace graphic
{

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
	

	struct sRawMesh
	{
		string name;
		vector<Vector3> vertices;
		vector<Vector3> normals;
		vector<Vector3> tex;
		vector<int> indices;
		vector<sVertexWeight> weights;
		string texturePath;
		
		int id; // only bone
		int parentId; // only bone
		Matrix44 localTm; // only bone
		Matrix44 worldTm; // only bone
	};


	// ���ϳ��� ������ �����ϴ� �ڷᱸ��.
	struct sRawMeshGroup
	{
		string name;
		vector<sRawMesh> meshes;
		vector<sRawMesh> bones;
	};

}
