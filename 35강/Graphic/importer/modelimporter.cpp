
#include "stdafx.h"
#include "modelimporter.h"


namespace graphic { namespace importer {

	bool ReadRawMeshFileV9( const string &fileName, OUT sRawMeshGroup &rawMeshes );
	bool ReadRawAnimationFileV9( const string &fileName, OUT sRawAniGroup &rawAnies );


	bool ReadMeshInfo( std::ifstream &fin, OUT sRawMesh &rawMesh );
	bool ReadVertexIndexNormal( std::ifstream &fin, OUT sRawMesh &rawMesh );
	bool ReadVertexIndexNormalBone( std::ifstream &fin, OUT sRawBone &rawBone );
	bool ReadTextureCoordinate( std::ifstream &fin, const string &fileName, OUT sRawMesh &rawMesh, bool flag=false );
	bool ReadAnimation(std::ifstream &fin, OUT sRawAni &rawAni );
	bool ReadBone(std::ifstream &fin, OUT sRawMeshGroup &rawMeshes);
	bool ReadBoneInfo(std::ifstream &fin, OUT sRawBone &rawBone );
	bool ReadTM(std::ifstream &fin, OUT sRawBone &rawBone );
	bool ReadVertexWeight(std::ifstream &fin, OUT sRawMesh &rawMesh );
	bool ReadMaterial(std::ifstream &fin, const string &fileName, OUT sMaterial &mtrl);
}}

using namespace graphic;
using namespace importer;


// load all exporter version
bool importer::ReadRawMeshFile( const string &fileName, OUT sRawMeshGroup &rawMeshes )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string version;
	fin >> version;

	if (version == "EXPORTER_V9")
	{
		ReadRawMeshFileV9(fileName, rawMeshes);
	}
	else
	{
		::MessageBoxA(GetRenderer()->GetHwnd(), "�������� �ʴ� ���� �Դϴ�.", "Error", MB_OK);
	}

	return true;
}


// load animation file
bool importer::ReadRawAnimationFile( const string &fileName, OUT sRawAniGroup &rawAni )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string version;
	fin >> version;

	if (version == "EXPORTER_V9")
	{
		ReadRawAnimationFileV9(fileName, rawAni);
	}
	else
	{
		::MessageBoxA(GetRenderer()->GetHwnd(), "�������� �ʴ� ���� �Դϴ�.", "Error", MB_OK);
	}

	return true;
}


bool importer::ReadRawMeshFileV9( const string &fileName, OUT sRawMeshGroup &rawMeshes )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >> exporterVersion;

	string meshExporter;
	fin >> meshExporter;

	if (meshExporter != "MESH_EXPORT")
		return false;

	string material, eq;
	int mtrlCount;
	fin >> material >> eq >> mtrlCount;

	rawMeshes.mtrls.resize(mtrlCount);

	for (int i=0; i < mtrlCount; ++i)
	{
		ReadMaterial(fin, fileName, rawMeshes.mtrls[ i]);
	}

	string geomObject;
	int geomObjectCount;
	fin >> geomObject >> eq >> geomObjectCount;

	rawMeshes.meshes.reserve(geomObjectCount);

	for (int i=0; i < geomObjectCount; ++i)
	{
		rawMeshes.meshes.push_back( sRawMesh() );
		ReadMeshInfo(fin, rawMeshes.meshes.back());
		ReadVertexIndexNormal(fin, rawMeshes.meshes.back());
		ReadTextureCoordinate(fin, fileName, rawMeshes.meshes.back(), true);
		ReadVertexWeight(fin, rawMeshes.meshes.back());
	}

	ReadBone(fin, rawMeshes);

	return true;
}


bool importer::ReadRawAnimationFileV9( const string &fileName, OUT sRawAniGroup &rawAnies )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >> exporterVersion;
	
	string animationExporter;
	fin >> animationExporter;

	if (animationExporter != "ANIMATION_EXPORT")
		return false;

	string exp, eq;
	int aniCount;
	fin >> exp >> eq >> aniCount;

	for (int i=0; i < aniCount; ++i)
	{
		rawAnies.anies.push_back( sRawAni() );
		ReadAnimation(fin, rawAnies.anies.back());
	}

	return true;
}


// �޽������� �о�´�.
// Mesh Name, Material ID
bool importer::ReadMeshInfo( std::ifstream &fin, OUT sRawMesh &rawMesh )
{
	string exp, eq;
	fin >> exp >> eq;

	string name;
	std::getline(fin, name);
	rawMesh.name = common::trim(name);

	int materialId;
	fin >> exp >> eq >> materialId;

	rawMesh.mtrlId = materialId;

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

	// ���� ���͸� �о�´�.
	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;
	rawMesh.normals.resize(vtxSize);

	if (numNormal > 0)
	{
		float num1, num2, num3;
		for (int i = 0; i < numNormal; i++)
		{
			fin >> num1 >> num2 >> num3;
			Vector3 n(num1, num2, num3);

			// ���������� ����� ���ؼ� �Ҵ��Ѵ�.
			for (int k=0; k < 3; ++k)
			{
				const int vtxIdx = rawMesh.indices[ i*3 + k];
				rawMesh.normals[ vtxIdx] += n;
			}
		}

		for (int i=0; i < vtxSize; ++i)
			rawMesh.normals[ i].Normalize();
	}

	return true;
}


bool importer::ReadVertexIndexNormalBone( std::ifstream &fin, OUT sRawBone &rawBone )
{
	string vtx, eq;
	int vtxSize;
	fin >> vtx >> eq >> vtxSize;

	//int materialId;
	//fin >> materialId;

	if (vtxSize > 0)
		rawBone.vertices.reserve(vtxSize + vtxSize/2);

	float num1, num2, num3;
	for (int i = 0; i < vtxSize; i++)
	{
		fin >> num1 >> num2 >> num3;
		rawBone.vertices.push_back( Vector3(num1, num2, num3) );
	}


	// �ε��� ���� �ʱ�ȭ.
	string idx;
	int faceSize;
	fin >> idx >> eq >> faceSize;

	if (faceSize > 0)
	{
		rawBone.indices.reserve(faceSize);

		int num1, num2, num3;
		for (int i = 0; i < faceSize*3; i+=3)
		{
			fin >> num1 >> num2 >> num3;
			rawBone.indices.push_back(num1);
			rawBone.indices.push_back(num2);
			rawBone.indices.push_back(num3);
		}
	}

	// ���� ���͸� �о�´�.
	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;

	if ((numNormal > 0) && (vtxSize > 0))
	{
		rawBone.normals.resize(vtxSize);

		float num1, num2, num3;
		for (int i = 0; i < numNormal; i++)
		{
			fin >> num1 >> num2 >> num3;
			Vector3 n(num1, num2, num3);

			// ���������� ����� ���ؼ� �Ҵ��Ѵ�.
			for (int k=0; k < 3; ++k)
			{
				const int vtxIdx = rawBone.indices[ i*3 + k];
				rawBone.normals[ vtxIdx] += n;
			}
		}

		for (int i=0; i < vtxSize; ++i)
			rawBone.normals[ i].Normalize();
	}

	return true;
}


// �ؽ��� ��ǥ�� �о� �´�.
bool importer::ReadTextureCoordinate( std::ifstream &fin, const string &fileName, OUT sRawMesh &rawMesh,
	bool flag) // flag=false
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
			if (vtxSize > i)
				rawMesh.tex[ i] = Vector3(fnum1, fnum2, 0);
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
/*
		map<int, vector<int> > vtxIdxMap; // vertex index, vertex index array
		for (int i=0; i < vtxSize; ++i)
		{
			vector<int> varray;
			varray.reserve(4);
			varray.push_back(i);
			vtxIdxMap[ i] = varray;
		}


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

	return true;
}


// �ִϸ��̼� ������ �д´�.
bool importer::ReadAnimation(std::ifstream &fin, OUT sRawAni &rawAni )
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
bool importer::ReadBone(std::ifstream &fin, OUT sRawMeshGroup &rawMeshes )
{
	string boneObject, eq;
	int boneObjectCount;
	fin >> boneObject >> eq >> boneObjectCount;

	rawMeshes.bones.reserve(boneObjectCount);

	for (int i=0; i < boneObjectCount; ++i)
	{
		rawMeshes.bones.push_back( sRawBone() );
		ReadVertexIndexNormalBone(fin, rawMeshes.bones.back());
		ReadBoneInfo(fin, rawMeshes.bones.back());
		ReadTM(fin, rawMeshes.bones.back());
	}

	return true;
}


// Bone ������ �д´�.
bool importer::ReadBoneInfo(std::ifstream &fin, OUT sRawBone &rawBone )
{
	string cmd, eq, boneName;
	int id, parentId;
	fin >> cmd >> eq >> id;

	fin >> cmd >> eq;
	std::getline(fin, boneName);
	boneName = common::trim(boneName);

	fin >> cmd >> eq >> parentId;	

	rawBone.id = id;
	rawBone.parentId = parentId;
	rawBone.name = boneName;

	return true;
}


// ����, ���� ����� �о �����Ѵ�.
bool importer::ReadTM(std::ifstream &fin, OUT sRawBone &rawBone )
{
	string local, world, mat;

	{ // LocalTM
		fin >> local;

		Matrix44 tm;
		fin >> mat >> tm._11 >> tm._12 >> tm._13 >> tm._14;
		fin >> mat >> tm._21 >> tm._22 >> tm._23 >> tm._24;
		fin >> mat >> tm._31 >> tm._32 >> tm._33 >> tm._34;
		fin >> mat >> tm._41 >> tm._42 >> tm._43 >> tm._44;
		rawBone.localTm = tm;
	}

	{ // WorldTM
		fin >> world;

		Matrix44 tm;
		fin >> mat >> tm._11 >> tm._12 >> tm._13 >> tm._14;
		fin >> mat >> tm._21 >> tm._22 >> tm._23 >> tm._24;
		fin >> mat >> tm._31 >> tm._32 >> tm._33 >> tm._34;
		fin >> mat >> tm._41 >> tm._42 >> tm._43 >> tm._44;
		rawBone.worldTm = tm;
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


// ���͸��� ���� �ε�.
bool importer::ReadMaterial(std::ifstream &fin, const string &fileName, OUT sMaterial &mtrl)
{
	string id, eq;

	int idx;
	fin >> id >> idx;

	Vector4 v;
	fin >> id >> v.x >> v.y >> v.z >> v.w;
	mtrl.diffuse = v;
	fin >> id >> v.x >> v.y >> v.z >> v.w;
	mtrl.ambient = v;
	fin >> id >> v.x >> v.y >> v.z >> v.w;
	mtrl.specular = v;
	fin >> id >> v.x >> v.y >> v.z >> v.w;
	mtrl.emissive = v;

	float f;
	fin >> id >> f;
	mtrl.power = f;

	string textureTok, texFilePath;
	fin >> textureTok; // TEXTURE

	std::getline(fin, texFilePath);
	string  textureFileName = common::GetFilePathExceptFileName(fileName) + "\\" + 
		common::trim(texFilePath);
	mtrl.texture = textureFileName;

	return true;
}


// ���� ������ �����Ѵ�.
RESOURCE_TYPE::TYPE importer::GetFileKind( const string &fileName )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return RESOURCE_TYPE::NONE;

	string version;
	fin >> version;

	string exportName;
	fin >> exportName;

	if (exportName == "ANIMATION_EXPORT")
	{
		return RESOURCE_TYPE::ANIMATION;
	}
	else if (exportName == "MESH_EXPORT")
	{
		return RESOURCE_TYPE::MESH;
	}
	
	// exception
	return RESOURCE_TYPE::NONE;
}
