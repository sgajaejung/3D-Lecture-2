
#pragma once


class CModelView;
class CAnimationController2;
class CViewer2Dlg : public CDialogEx
{
// 생성입니다.
public:
	CViewer2Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CViewer2Dlg();
	enum { IDD = IDD_VIEWER2_DIALOG };

	void MainLoop();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CModelView *m_modelView;
	CAnimationController2 *m_aniController;
	bool m_loop;
	bool m_dxInit;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckWireframe();
	BOOL m_WireFrame;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedCheckBone();
	BOOL m_RenderBone;
	BOOL m_RenderMesh;
	afx_msg void OnBnClickedCheckMesh();
};
