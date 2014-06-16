
#pragma once


namespace framework
{

	class cGameMain
	{
	public:
		cGameMain();
		virtual ~cGameMain();
		
		enum STATE
		{
			INIT,
			RUN,
			SHUTDOWN,			
		};

		virtual bool Init(HWND hWnd);
		virtual void ShutDown();
		virtual void Run();
		virtual void Update(const float elapseT);
		virtual void Render(const float elapseT);
		virtual void Exit();

		const wstring& GetWindowName();
		const RECT& GetWindowRect();		
		virtual void MessageProc( UINT message, WPARAM wParam, LPARAM lParam);


	protected:
		virtual bool OnInit() { return true; }
		virtual void OnUpdate(const float elapseT) {}
		virtual void OnRender(const float elapseT) {}
		virtual void OnShutdown() {}


	protected:
		STATE m_state;
		HWND m_hWnd;
		wstring m_windowName;
		RECT m_windowRect;
		LPDIRECT3DDEVICE9 m_DxDevice;


	// singleton
	protected:
		static cGameMain* m_pInstance;
	public:
		static cGameMain* Get();
	};


	// �����ӿ�ũ ���� �Լ�.
	int FrameWorkWinMain(HINSTANCE hInstance, 
		HINSTANCE hPrevInstance, 
		LPSTR lpCmdLine, 
		int nCmdShow);

	// �����ӿ�ũ �ν��Ͻ��� �����Ѵ�. �ݵ�� �� �Լ��� �����ؾ� �Ѵ�.
	cGameMain* CreateFrameWork();
}
