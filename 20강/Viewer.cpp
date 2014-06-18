
#include "stdafx.h"
#include <fstream>



class cViewer : public framework::cGameMain
{
public:
	cViewer();
	virtual ~cViewer();

	virtual bool OnInit() override;
	virtual void OnUpdate(const float elapseT) override;
	virtual void OnRender(const float elapseT) override;
	virtual void OnShutdown() override;
	virtual void MessageProc( UINT message, WPARAM wParam, LPARAM lParam) override;


protected:
	bool ReadModelFile( const string &fileName, graphic::cVertexBuffer &vtxBuff,  
		graphic::cIndexBuffer &idxBuff, graphic::cTexture &texture );


private:
	graphic::cVertexBuffer m_vtxBuff;
	graphic::cIndexBuffer m_idxBuff;
	graphic::cMaterial m_mtrl;
	graphic::cLight m_light;
	graphic::cTexture m_texture;
	string m_filePath;

};

INIT_FRAMEWORK(cViewer);



// ���ؽ� ����ü
struct Vertex
{
	Vertex() {}
	Vertex(float x0, float y0, float z0) : p(Vector3(x0, y0, z0)), n(Vector3(0,0,0))
		,u(-100), v(-100)
	{}
	Vector3 p;
	Vector3 n;
	float u,v;
	static const DWORD FVF;
};
//���ؽ� ����ü ����.
const DWORD Vertex::FVF  = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


cViewer::cViewer()
{
	m_windowName = L"Viewer";
	const RECT r = {0, 0, 800, 600};
	m_windowRect = r;
}

cViewer::~cViewer()
{
}


bool cViewer::OnInit()
{
	DragAcceptFiles(m_hWnd, TRUE);

	ReadModelFile("../media/cube.dat", m_vtxBuff, m_idxBuff, m_texture);

	m_mtrl.InitWhite();

	Vector4 color(1,1,1,1);
	m_light.Init( graphic::cLight::LIGHT_DIRECTIONAL, color * 0.4f, color, color * 0.6f, Vector3(1,0,0));
	m_light.Bind(0);


	Matrix44 V;
	Vector3 dir = Vector3(0,0,0)-Vector3(0,0,-5);
	dir.Normalize();
	V.SetView(Vector3(0,0,-500), dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);


	const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;


	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	return true;
}


void cViewer::OnUpdate(const float elapseT)
{

}


void cViewer::OnRender(const float elapseT)
{
	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(255, 255, 255),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();

		static float y = 0;
		y += elapseT;
		// ������ 2*PI �� �̸��� 0���� �ʱ�ȭ�Ѵ�.
		if (y >= 6.28f)
			y = 0;

		Matrix44 rx, ry, r;
		rx.SetRotationX(MATH_PI/4.f); 	// x������ 45�� ȸ����Ų��.
		ry.SetRotationY(y); // y������ ȸ��
		r = rx*ry;
		graphic::GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&r);

		m_mtrl.Bind();
		m_texture.Bind(0);
		m_idxBuff.Bind();
		m_vtxBuff.Bind();
		graphic::GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
			m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());


		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}


void cViewer::OnShutdown()
{

}


void cViewer::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DROPFILES:
		{
			HDROP hdrop = (HDROP)wParam;
			char filePath[ MAX_PATH];
			const UINT size = DragQueryFileA(hdrop, 0, filePath, MAX_PATH);
			if (size == 0) 
				return;// handle error...

			m_filePath = filePath;
			m_texture.Clear();
			m_vtxBuff.Clear();
			m_idxBuff.Clear();
			ReadModelFile(filePath, m_vtxBuff, m_idxBuff, m_texture);
		}
		break;

	case WM_KEYDOWN:
		if (wParam == VK_F5) // Refresh
		{
			 if (m_filePath.empty())
				 return;
			 m_texture.Clear();
			m_vtxBuff.Clear();
			m_idxBuff.Clear();
			ReadModelFile(m_filePath, m_vtxBuff, m_idxBuff, m_texture);
		}
		break;
	}
}


bool cViewer::ReadModelFile( const string &fileName, graphic::cVertexBuffer &vtxBuff,  
	graphic::cIndexBuffer &idxBuff, graphic::cTexture &texture )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >>exporterVersion;

	if (exporterVersion != "EXPORTER_V1")
		return false;

	string vtx, eq;
	int vtxSize;
	fin >> vtx >> eq >> vtxSize;

	if (vtxSize <= 0)
		return  false;

	vector<Vertex> tempVtxBuff;
	tempVtxBuff.reserve(vtxSize + vtxSize/2);

	float num1, num2, num3;
	for (int i = 0; i < vtxSize; i++)
	{
		fin >> num1 >> num2 >> num3;
		tempVtxBuff.push_back( Vertex(num1, num2, num3) );
	}


	// �ε��� ���� �ʱ�ȭ.
	string idx;
	int faceSize;
	fin >> idx >> eq >> faceSize;

	vector<int> tempIdxBuff;
	tempIdxBuff.reserve(faceSize);

	if (faceSize > 0)
	{
		int num1, num2, num3;
		for (int i = 0; i < faceSize*3; i+=3)
		{
			fin >> num1 >> num2 >> num3;
			tempIdxBuff.push_back(num1);
			tempIdxBuff.push_back(num2);
			tempIdxBuff.push_back(num3);
		}
	}

	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;

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
				const int vtxIdx = tempIdxBuff[ i*3 + k];
				tempVtxBuff[ vtxIdx].n += n;
				++vertCount[ vtxIdx];
			}
		}

		for (int i=0; i < vtxSize; ++i)
		{
			tempVtxBuff[ i].n /= (float)vertCount[ i];
			tempVtxBuff[ i].n.Normalize();
		}
	}


	string tex;
	int numTex;
	fin >> tex >> eq >> numTex;

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

		// �ؽ��� ��ǥ�� ���ؽ� ���ۿ� �����Ѵ�. 
		// ���ؽ� ������ uv ���� �ʱ�ȭ ���� �ʾҴٸ�, �ʱ�ȭ �Ѵ�.
		// ���ؽ��� �ϳ� �̻��� uv���� �����Ѵٸ�, ���ؽ��� �߰��ϰ�, �ε������۸� �����Ѵ�.
		for (int i=0; i < (int)texFaces.size(); ++i)
		{
			const Vector3 tex = texVertices[ texFaces[ i]];
			const int vtxIdx = tempIdxBuff[ i];

			bool isFind = false;
			for (int k=0; k < (int)vtxIdxMap[ vtxIdx].size(); ++k)
			{
				const int subVtxIdx = vtxIdxMap[ vtxIdx][ k];

				// �ؽ��� ��ǥ�� ���ؽ� ���ۿ� ����Ǿ� �ִٸ�, index buffer ���� �ش� vertex index ��
				// ���� �Ѵ�.
				if ((-100 == tempVtxBuff[ subVtxIdx].u) &&
					(-100 == tempVtxBuff[ subVtxIdx].v))
				{
					tempVtxBuff[ subVtxIdx].u = tex.x;
					tempVtxBuff[ subVtxIdx].v = tex.y;
					isFind = true;
					break;
				}
				else if ((tex.x == tempVtxBuff[ subVtxIdx].u) && 
					(tex.y == tempVtxBuff[ subVtxIdx].v))
				{
					tempIdxBuff[ i] = subVtxIdx;
					isFind = true;
					break;
				}
			}

			// ���ؽ� ���ۿ� ���� uv ��ǥ���, �� ���ؽ��� ���ؽ����ۿ� �߰��Ѵ�.
			// �ε��� ���ۿ��� ���� �߰��� ���ؽ� �ε������� �ִ´�.
			if (!isFind)
			{
				Vertex v = tempVtxBuff[ vtxIdx];
				v.u = tex.x;
				v.v = tex.y;
				tempVtxBuff.push_back(v);
				const int newVtxIdx = tempVtxBuff.size()-1;
				vtxIdxMap[ vtxIdx].push_back( newVtxIdx );
				tempIdxBuff[ i] = newVtxIdx;
			}
		}
	}


	// �ؽ��� �����̸� �ε�.



	// ���ؽ� ���� ����.
	if (!vtxBuff.Create(tempVtxBuff.size(), sizeof(Vertex), Vertex::FVF))
		return false;

	// ���ؽ� ���� �ʱ�ȭ.
	Vertex* vertices = (Vertex*)vtxBuff.Lock();
	for (int i = 0; i < (int)tempVtxBuff.size(); i++)
		vertices[ i] = tempVtxBuff[ i];
	vtxBuff.Unlock();

	// �ε��� ���� ����.
	if (!idxBuff.Create(tempIdxBuff.size()))
		return false;
	WORD *indices = (WORD*)idxBuff.Lock();
	for (int i = 0; i < (int)tempIdxBuff.size(); ++i)
		indices[ i] = tempIdxBuff[ i];
	idxBuff.Unlock();

	return true;
}
