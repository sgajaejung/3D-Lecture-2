
#include "stdafx.h"
#include "../wxMemMonitorLib/wxMemMonitor.h"
#include "TestScene.h"


DECLARE_TYPE_NAME(cViewer)
class cViewer : public framework::cGameMain
						, public memmonitor::Monitor<cViewer, TYPE_NAME(cViewer)>
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
	void UpdateCamera();


private:
	LPD3DXSPRITE m_sprite;

	graphic::cLight m_light;
	graphic::cMaterial m_mtrl;
	graphic::cTexture m_texture;
	graphic::cModel *m_model;
	graphic::cSprite *m_image;

	cTestScene *m_scene;


	string m_filePath;

	POINT m_curPos;
	bool m_LButtonDown;
	bool m_RButtonDown;
	Matrix44 m_rotateTm;

	Vector3 m_camPos;
	Vector3 m_lookAtPos;
};

INIT_FRAMEWORK(cViewer);


cViewer::cViewer() :
	m_model(NULL)
,	m_sprite(NULL)
,	m_image(NULL)
,	m_scene(NULL)
{
	m_windowName = L"Viewer";
	const RECT r = {0, 0, 1024, 768};
	m_windowRect = r;
	m_LButtonDown = false;
	m_RButtonDown = false;
}

cViewer::~cViewer()
{
	SAFE_DELETE(m_model);
	SAFE_DELETE(m_image);
	SAFE_DELETE(m_scene);
	SAFE_RELEASE(m_sprite);
	graphic::ReleaseRenderer();
}


bool cViewer::OnInit()
{
	DragAcceptFiles(m_hWnd, TRUE);

	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);


	m_scene = new cTestScene(m_sprite);
	m_scene->SetPos(Vector3(100,100,0));

	framework::cButton *btn1 = new framework::cButton(m_sprite, 1);
	btn1->Create("button1.png");
	m_scene->InsertChild(btn1);

	framework::cButton *btn2 = new framework::cButton(m_sprite, 2);
	btn2->Create("button2.png");
	btn2->SetScale(Vector3(2,2,0));
	btn2->SetPos(Vector3(200,0,0));
	m_scene->InsertChild(btn2);

	framework::cButton *btn3 = new framework::cButton(m_sprite, 3);
	btn3->Create("button1.png");
	btn3->SetPos(Vector3(200,0,0));
	m_scene->InsertChild(btn3);



	m_filePath = "../media/mesh.dat";
	m_model = new graphic::cModel();
	//m_model->Create( m_filePath );
	//m_texture.Create("../media/001 copy.jpg");
	//m_model->SetAnimation("../media/ani.ani");
	//m_model->SetAnimation("../media/ani4.ani");
	//m_texture.Create("../media/���Ҷ�.jpg");

	//m_image = new graphic::cSprite(m_sprite, 0, "image1");
	//m_image->Create( "../media/���Ҷ�.jpg");
	//m_image->SetScale(Vector3(0.5f,0.5f,0.5f));
	//m_image->SetPos( Vector3(200, 200, 0) );


	m_mtrl.InitWhite();

	Vector4 color(1,1,1,1);
	m_light.Init( graphic::cLight::LIGHT_DIRECTIONAL, color * 0.4f, color, color * 0.6f, Vector3(0,-1,0));
	m_light.Bind(0);


	m_camPos = Vector3(100,100,-500);
	m_lookAtPos = Vector3(0,0,0);
	UpdateCamera();

	const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	Matrix44 proj;
	proj.SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;


	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	return true;
}


void cViewer::OnUpdate(const float elapseT)
{
	m_model->Move(elapseT);
}


void cViewer::OnRender(const float elapseT)
{
	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(150, 150, 150),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();

		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

		Matrix44 tm;
		m_scene->Render(tm);
	//m_image->Render(tm);

		m_model->SetTM(m_rotateTm);
		m_model->Render();

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
	if (m_scene)
		m_scene->MessageProc(message, wParam, lParam);

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
			m_model->Create(filePath);
			m_model->SetAnimation("../media/ani4.ani");
		}
		break;

	case WM_MOUSEWHEEL:
		{
			int fwKeys = GET_KEYSTATE_WPARAM(wParam);
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			dbg::Print( "%d %d", fwKeys, zDelta);

			Vector3 dir = m_lookAtPos - m_camPos;
			const float len = dir.Length();
			dir.Normalize();

			float zoomLen = (len > 100)? 50 : (len/3.f);
			if (fwKeys & 0x4)
				zoomLen = 1;

			m_camPos += (zDelta<0)? dir*-zoomLen : dir*zoomLen;
			UpdateCamera();
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F5: // Refresh
			{
				if (m_filePath.empty())
					return;
				m_model->Create(m_filePath);
			}
			break;
		case VK_BACK:
			// ȸ�� ��� �ʱ�ȭ.
			m_rotateTm.SetIdentity();
			m_model->SetTM(m_rotateTm);
			break;
		case VK_TAB:
			{
				static bool flag = false;
				graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, flag);
				graphic::GetDevice()->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
				flag = !flag;
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			m_LButtonDown = true;
			m_curPos.x = LOWORD(lParam);
			m_curPos.y = HIWORD(lParam);
		}
		break;

	case WM_LBUTTONUP:
		m_LButtonDown = false;
		break;

	case WM_RBUTTONDOWN:
		{
			m_RButtonDown = true;
			m_curPos.x = LOWORD(lParam);
			m_curPos.y = HIWORD(lParam);
		}
		break;

	case WM_RBUTTONUP:
		m_RButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		if (wParam & 0x10) // middle button down
		{
			POINT pos = {LOWORD(lParam), HIWORD(lParam)};
			m_scene->SetPos(Vector3(pos.x, pos.y,0));
		}

		if (m_LButtonDown)
		{
			POINT pos = {LOWORD(lParam), HIWORD(lParam)};
			const int x = pos.x - m_curPos.x;
			const int y = pos.y - m_curPos.y;
			m_curPos = pos;

			Matrix44 mat1;
			mat1.SetRotationY( -x * 0.01f );
			Matrix44 mat2;
			mat2.SetRotationX( -y * 0.01f );

			m_rotateTm *= (mat1 * mat2);
		}
		else if (m_RButtonDown)
		{
			POINT pos = {LOWORD(lParam), HIWORD(lParam)};
			const int x = pos.x - m_curPos.x;
			const int y = pos.y - m_curPos.y;
			m_curPos = pos;

			{ // rotate Y-Axis
				Quaternion q(Vector3(0,1,0), x * 0.005f); 
				Matrix44 m = q.GetMatrix();
				m_camPos *= m;
			}

			{ // rotate X-Axis
				Quaternion q(Vector3(1,0,0), y * 0.005f); 
				Matrix44 m = q.GetMatrix();
				m_camPos *= m;
			}

			UpdateCamera();
		}
		else
		{
			POINT pos = {LOWORD(lParam), HIWORD(lParam)};
			//m_scene->SetPos(Vector3(pos.x, pos.y,0));
			//if (m_image->IsContain(Vector2(pos.x, pos.y)))
			//{
			//	static int count = 0;
			//	++count;
			//	dbg::Print( "IsContain %d", count);
			//}
		}

		break;
	}
}


void cViewer::UpdateCamera()
{
	Matrix44 V;
	Vector3 dir = m_lookAtPos - m_camPos;
	dir.Normalize();
	V.SetView(m_camPos, dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);
}
