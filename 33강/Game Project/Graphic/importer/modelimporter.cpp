
#include "stdafx.h"
#include "modelimporter.h"

/*
	EXPORTER_V1
		- vertex, index, normal

	EXPORTER_V2
		- vertex, index, normal, texture

	EXPORTER_V3
	- vertex, index, normal, texture, animation

	EXPORTER_V4
	- vertex, index, normal, texture, animation, bone

	EXPORTER_V5
	- vertex, index, normal, texture, animation, bone, vertex weight

*/

namespace graphic { namespace importer {

	bool ReadRawMeshFileV1( const string &fileName, OUT sRawMeshGroup &rawMeshes );
	bool ReadRawMeshFileV2( const string &fileName, OUT sRawMeshGroup &rawMeshes );
	bool ReadRawMeshFileV3( const string &fileName, OUT sRawMeshGroup &rawMeshes, OUT sRawAniGroup &rawAnies );
	bool ReadRawMeshFileV4( const string &fileName, OUT sRawMeshGroup &rawMeshes, OUT sRawAniGroup &rawAnies );
	bool ReadRawMeshFileV5( const string &fileName, OUT sRawMeshGroup &rawMeshes, OUT sRawAniGroup &rawAnies );


	bool ReadVertexIndexNormal( std::ifstream &fin, OUT sRawMesh &rawMesh );
	bool ReadTextureCoordinate( std::ifstream &fin, const string &fileName, OUT sRawMesh &rawMesh );
	bool ReadAnimation(std::ifstream &fin, OUT sRawMesh &rawMesh, OUT sRawAni &rawAni );
	bool ReadBone(std::ifstream &fin, OUT sRawMeshGroup &rawMeshes, OUT sRawAniGroup &rawAnies );
	bool ReadBoneInfo(std::ifstream &fin, OUT sRawMesh &rawMesh );
	bool ReadTM(std::ifstream &fin, OUT sRawMesh &rawMesh );
	bool ReadVertexWeight(std::ifstream &fin, OUT sRawMesh &rawMesh );
}}

using namespace graphic;
using namespace importer;


// load all exporter version
bool importer::ReadRawMeshFile( const string &fileName, 
	OUT sRawMeshGroup &rawMeshes, OUT sRawAniGroup &rawAnies )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string version;
	fin >> version;

	if (version == "EXPORTER_V1")
	{
		return ReadRawMeshFileV1(fileName, rawMeshes);
	}
	else if (version == "EXPORTER_V2")
	{
		return ReadRawMeshFileV2(fileName, rawMeshes);
	}
	else if (version == "EXPORTER_V3")
	{
		return ReadRawMeshFileV3(fileName, rawMeshes, rawAnies);
	}
	else if (version == "EXPORTER_V4")
	{
		return ReadRawMeshFileV4(fileName, rawMeshes, rawAnies);
	}
	else if (version == "EXPORTER_V5")
	{
		return ReadRawMeshFileV5(fileName, rawMeshes, rawAnies);
	}
	else
	{
		::MessageBoxA(GetRenderer()->GetHwnd(), "�������� �ʴ� ���� �Դϴ�.", "Error", MB_OK);
	}

	return true;
}


// load exporter version 1
bool importer::ReadRawMeshFileV1( const string &fileName, OUT sRawMeshGroup &rawMeshes )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >> exporterVersion;

	rawMeshes.meshes.push_back( sRawMesh() );
	ReadVertexIndexNormal(fin, rawMeshes.meshes.back());
	return true;
}


// load exporter version 2
bool importer::ReadRawMeshFileV2( const string &fileName, OUT sRawMeshGroup &rawMeshes )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >> exporterVersion;

	rawMeshes.meshes.push_back( sRawMesh() );
	ReadVertexIndexNormal(fin, rawMeshes.meshes.back());
	ReadTextureCoordinate(fin, fileName, rawMeshes.meshes.back());

	return true;
}


// load exporter version 3
bool importer::ReadRawMeshFileV3( const string &fileName, 
	OUT sRawMeshGroup &rawMeshes, OUT sRawAniGroup &rawAnies )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >> exporterVersion;

	rawMeshes.meshes.push_back( sRawMesh() );
	ReadVertexIndexNormal(fin, rawMeshes.meshes.back());
	ReadTextureCoordinate(fin, fileName, rawMeshes.meshes.back());

	rawAnies.anies.push_back( sRawAni() );
	rawAnies.type = sRawAniGroup::MESH_ANI;
	ReadAnimation(fin, rawMeshes.meshes.back(), rawAnies.anies.back());

	return true;
}


bool importer::ReadRawMeshFileV4( const string &fileName, OUT sRawMeshGroup &rawMeshes, 
	OUT sRawAniGroup &rawAnies )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >> exporterVersion;

	string geomObject, eq;
	int geomObjectCount;
	fin >> geomObject >> eq >> geomObjectCount;

	rawMeshes.meshes.reserve(geomObjectCount);

	rawAnies.type = sRawAniGroup::MESH_ANI;

	for (int i=0; i < geomObjectCount; ++i)
	{
		rawMeshes.meshes.push_back( sRawMesh() );
		ReadVertexIndexNormal(fin, rawMeshes.meshes.back());
		ReadTextureCoordinate(fin, fileName, rawMeshes.meshes.back());

		rawAnies.anies.push_back( sRawAni() );
		ReadAnimation(fin, rawMeshes.meshes.back(), rawAnies.anies.back());
	}

	ReadBone(fin, rawMeshes, rawAnies);

	return true;
}


bool importer::ReadRawMeshFileV5( const string &fileName, OUT sRawMeshGroup &rawMeshes, 
	OUT sRawAniGroup &rawAnies )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >> exporterVersion;

	string geomObject, eq;
	int geomObjectCount;
	fin >> geomObject >> eq >> geomObjectCount;

	rawMeshes.meshes.reserve(geomObjectCount);

	rawAnies.type = sRawAniGroup::MESH_ANI;

	for (int i=0; i < geomObjectCount; ++i)
	{
		rawMeshes.meshes.push_back( sRawMesh() );
		ReadVertexIndexNormal(fin, rawMeshes.meshes.back());
		ReadTextureCoordinate(fin, fileName, rawMeshes.meshes.back());

		rawAnies.anies.push_back( sRawAni() );
		ReadAnimation(fin, rawMeshes.meshes.back(), rawAnies.anies.back());
		ReadVertexWeight(fin, rawMeshes.meshes.back());
	}

	ReadBone(fin, rawMeshes, rawAnies);

	return true;
}



// Read Vertex, Index, Normal Buffer
// Normal �� face ������ŭ �����ؾ� �Ѵ�.
bool importer::ReadVertexIndexNormal( std::ifstream &fin, OUT sRawMesh &rawMesh )
{
	string vtx, eq;
	int vtxSize;
	fin >> vtx >> eq >> vtxSize;

	if (vtxSize <= 0)
		return  false;

	rawMesh.vertices.reserve(vtxSize + vtxSize/2);

	float num1, num2, num3;
	for (int i = 0; i < vtxSize; i++)
	{
		fin >> num1 >> num2 >> num3;
		rawMesh.vertices.push_back( Vector3(num1, num2, num3) );
	}


	// �ε��� ���� �ʱ�ȭ.
	string idx;
	int faceSize;
	fin >> idx >> eq >> faceSize;

	rawMesh.indices.reserve(faceSize);

	if (faceSize > 0)
	{
		int num1, num2, num3;
		for (int i = 0; i < faceSize*3; i+=3)
		{
			fin >> num1 >> num2 >> num3;
			rawMesh.indices.push_back(num1);
			rawMesh.indices.push_back(num2);
			rawMesh.indices.push_back(num3);
		}
	}

	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;
	rawMesh.normals.resize(vtxSize);

	if (numNormal > 0)
	{
		float num1, num2, num3;
		vector<int> vertCount(vtxSize, 0);
		for (int i = 0; i < numNormal; i++)
		{
			fin >> num1 >> num2 >> num3;
			Vector3 n(num1, num2, num3);

			// ���������� ����� ���ؼ� �Ҵ��Ѵ�.
			for (int k=0; k < 3; ++k)
			{
				const int vtxIdx = rawMesh.indices[ i*3 + k];
				rawMesh.normals[ vtxIdx] += n;
				++vertCount[ vtxIdx];
			}
		}

		for (int i=0; i < vtxSize; ++i)
		{
			rawMesh.normals[ i] /= (float)vertCount[ i];
			rawMesh.normals[ i].Normalize();
		}
	}

	return true;
}


// �ؽ��� ��ǥ�� �о� �´�.
bool importer::ReadTextureCoordinate( std::ifstream &fin, const string &fileName, OUT sRawMesh &rawMesh )
{
	const int vtxSize = rawMesh.vertices.size();

	string tex, eq;
	int numTex;
	fin >> tex >> eq >> numTex;

	rawMesh.tex.resize(vtxSize);

	if (numTex > 0)
	{
		float fnum1, fnum2;
		vector<Vector3> texVertices(numTex);
		for (int i = 0; i < numTex; i++)
		{
			fin >> fnum1 >> fnum2;
			texVertices[ i] = Vector3(fnum1, fnum2, 0);
		}

		string strTexFace;
		int numTexFace;
		fin >> strTexFace >> eq >> numTexFace;

		vector<int> texFaces;
		texFaces.reserve(numTexFace*3);
		if (numTexFace > 0)
		{
			int num1, num2, num3;
			for (int i=0; i < numTexFace; ++i)
			{
				fin >> num1 >> num2 >> num3;
				texFaces.push_back( num1 );
				texFaces.push_back( num2 );
				texFaces.push_back( num3 );
			}
		}

		map<int, vector<int> > vtxIdxMap; // vertex index, vertex index array
		for (int i=0; i < vtxSize; ++i)
		{
			vector<int> varray;
			varray.reserve(4);
			varray.push_back(i);
			vtxIdxMap[ i] = varray;
		}

/*
		// �ؽ��� ��ǥ�� ���ؽ� ���ۿ� �����Ѵ�. 
		// ���ؽ� ������ uv ���� �ʱ�ȭ ���� �ʾҴٸ�, �ʱ�ȭ �Ѵ�.
		// ���ؽ��� �ϳ� �̻��� uv���� �����Ѵٸ�, ���ؽ��� �߰��ϰ�, �ε������۸� �����Ѵ�.
		for (int i=0; i < (int)texFaces.size(); ++i)
		{
			const Vector3 tex = texVertices[ texFaces[ i]];
			const int vtxIdx = rawMesh.indices[ i];

			bool isFind = false;
			for (int k=0; k < (int)vtxIdxMap[ vtxIdx].size(); ++k)
			{
				const int subVtxIdx = vtxIdxMap[ vtxIdx][ k];

				// �ؽ��� ��ǥ�� ���ؽ� ���ۿ� ����Ǿ� �ִٸ�, index buffer ���� �ش� vertex index ��
				// ���� �Ѵ�.
				if ((-100 == rawMesh.tex[ subVtxIdx].x) &&
					(-100 == rawMesh.tex[ subVtxIdx].y))
				{
					rawMesh.tex[ subVtxIdx].x = tex.x;
					rawMesh.tex[ subVtxIdx].y = tex.y;
					isFind = true;
					break;
				}
				else if ((tex.x == rawMesh.tex[ subVtxIdx].x) && 
					(tex.y == rawMesh.tex[ subVtxIdx].y))
				{
					rawMesh.indices[ i] = subVtxIdx;
					isFind = true;
					break;
				}
			}

			// ���ؽ� ���ۿ� ���� uv ��ǥ���, �� ���ؽ��� ���ؽ����ۿ� �߰��Ѵ�.
			// �ε��� ���ۿ��� ���� �߰��� ���ؽ� �ε������� �ִ´�.
			if (!isFind)
			{
				Vector3 p = rawMesh.vertices[ vtxIdx];
				Vector3 n = rawMesh.normals[ vtxIdx];
				Vector3 t = rawMesh.tex[ vtxIdx];

				t.x = tex.x;
				t.y = tex.y;

				rawMesh.vertices.push_back(p);
				rawMesh.normals.push_back(n);
				rawMesh.tex.push_back(t);

				const int newVtxIdx = rawMesh.vertices.size()-1;
				vtxIdxMap[ vtxIdx].push_back( newVtxIdx );
				rawMesh.indices[ i] = newVtxIdx;
			}
		}
/**/
	}

	// �ؽ��� �����̸� �ε�.
	string textureTok, texFilePath;
	fin >> textureTok >> eq;
	std::getline(fin, texFilePath);
	string  textureFileName = common::GetFilePathExceptFileName(fileName) + "\\" + 
		common::trim(texFilePath);
	rawMesh.texturePath = textureFileName;

	return true;
}


// �ִϸ��̼� ������ �д´�.
bool importer::ReadAnimation(std::ifstream &fin, OUT sRawMesh &rawMesh, OUT sRawAni &rawAni )
{
	// �ִϸ��̼� �ε�.
	float startT = 0;
	float endT = 0;
	string aniRange, strST, strET, eq;
	fin >> aniRange >> eq >> strST >> strET;
	startT = (float)atof(strST.c_str());
	endT = (float)atof(strET.c_str());

	rawAni.start = startT;
	rawAni.end = endT;


	{ // �̵� �ִϸ��̼� �ε�
		string keypos;
		int posSize;
		fin >> keypos >> eq >> posSize;

		rawAni.pos.resize(posSize);

		for (int i=0; i < posSize; ++i)
		{
			string framePos, strT; // FRAME_POS
			float x, y, z;
			fin >> framePos >> strT >> x >> y >> z;
			const float t = (float)atof(strT.c_str());

			rawAni.pos[ i].t = t;
			rawAni.pos[ i].p = Vector3(x, y, z);
		}
	}


	{ // ȸ�� �ִϸ��̼� �ε�.
		string keyrot;
		int rotSize;
		fin >> keyrot >> eq >> rotSize;

		rawAni.rot.resize(rotSize);

		for (int i=0; i < rotSize; ++i)
		{
			string frameRot, strT; // FRAME_ROT
			float x, y, z, w;
			fin >> frameRot >> strT >> x >> y >> z >> w;
			const float t = (float)atof(strT.c_str());

			rawAni.rot[ i].t = t;
			rawAni.rot[ i].q = Quaternion(x, y, z, w);
		}
	}


	{ // ������ �ִϸ��̼� �ε�.
		string keyscale;
		int scaleSize;
		fin >> keyscale >> eq >> scaleSize;

		rawAni.scale.resize(scaleSize);

		for (int i=0; i < scaleSize; ++i)
		{
			string frameScale, strT; // FRAME_SCALE
			float x, y, z;
			fin >> frameScale >> strT >> x >> y >> z;
			const float t = (float)atof(strT.c_str());

			rawAni.scale[ i].t = t;
			rawAni.scale[ i].s = Vector3(x, y, z);
		}
	}

	return true;
}


// Bone ������ �о�´�.
bool importer::ReadBone(std::ifstream &fin, OUT sRawMeshGroup &rawMeshes, OUT sRawAniGroup &rawAnies )
{
	string boneObject, eq;
	int boneObjectCount;
	fin >> boneObject >> eq >> boneObjectCount;

	rawMeshes.bones.reserve(boneObjectCount);

	if (boneObjectCount > 0)
	{
		rawAnies.type = sRawAniGroup::BONE_ANI;
		rawAnies.anies.clear();
	}

	for (int i=0; i < boneObjectCount; ++i)
	{
		rawMeshes.bones.push_back( sRawMesh() );
		ReadVertexIndexNormal(fin, rawMeshes.bones.back());
		ReadBoneInfo(fin, rawMeshes.bones.back());
		ReadTM(fin, rawMeshes.bones.back());

		rawAnies.anies.push_back( sRawAni() );
		ReadAnimation(fin, rawMeshes.bones.back(), rawAnies.anies.back());
	}

	return true;
}


// Bone ������ �д´�.
bool importer::ReadBoneInfo(std::ifstream &fin, OUT sRawMesh &rawMesh )
{
	string cmd, eq, boneName;
	int id, parentId;
	fin >> cmd >> eq >> id;

	fin >> cmd >> eq;
	std::getline(fin, boneName);
	boneName = common::trim(boneName);

	fin >> cmd >> eq >> parentId;	

	rawMesh.id = id;
	rawMesh.parentId = parentId;
	rawMesh.name = boneName;

	return true;
}


// ����, ���� ����� �о �����Ѵ�.
bool importer::ReadTM(std::ifstream &fin, OUT sRawMesh &rawMesh )
{
	string local, world, mat;

	{ // LocalTM
		fin >> local;

		Matrix44 tm;
		fin >> mat >> tm._11 >> tm._12 >> tm._13 >> tm._14;
		fin >> mat >> tm._21 >> tm._22 >> tm._23 >> tm._24;
		fin >> mat >> tm._31 >> tm._32 >> tm._33 >> tm._34;
		fin >> mat >> tm._41 >> tm._42 >> tm._43 >> tm._44;
		rawMesh.localTm = tm;
	}

	{ // WorldTM
		fin >> world;

		Matrix44 tm;
		fin >> mat >> tm._11 >> tm._12 >> tm._13 >> tm._14;
		fin >> mat >> tm._21 >> tm._22 >> tm._23 >> tm._24;
		fin >> mat >> tm._31 >> tm._32 >> tm._33 >> tm._34;
		fin >> mat >> tm._41 >> tm._42 >> tm._43 >> tm._44;
		rawMesh.worldTm = tm;
	}

	return true;
}


// ��Ų�ִϸ��̼� ���ؽ� ����ġ ������ �о �����Ѵ�.
bool importer::ReadVertexWeight(std::ifstream &fin, OUT sRawMesh &rawMesh )
{
	string id, eq;
	int vtxWeightCount;
	fin >> id >> eq >> vtxWeightCount; //  VERTEXWEIGHT_COUNT = 0

	rawMesh.weights.reserve(vtxWeightCount);

	for (int i=0; i < vtxWeightCount; ++i)
	{
		int vtxIdx, size;
		fin >> id >> eq >> vtxIdx >> size; // VTXWEIGHT_VERTEX_COUNT = 0 2
		
		sVertexWeight vtxWeight;
		for (int k=0; k < size; ++k)
		{
			int bone;
			float w;
			fin >> bone >> w;

			sWeight weight;
			weight.bone = bone;
			weight.weight = w;
			if (k < 6)
				vtxWeight.w[ k] = weight;
		}

		vtxWeight.vtxIdx = vtxIdx;
		vtxWeight.size = min(size, 6);
		rawMesh.weights.push_back(vtxWeight);
	}

	return true;
}

