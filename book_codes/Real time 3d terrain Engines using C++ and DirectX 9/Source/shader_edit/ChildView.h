// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__DC47B214_62F8_4B86_AD7E_BF90D863CF67__INCLUDED_)
#define AFX_CHILDVIEW_H__DC47B214_62F8_4B86_AD7E_BF90D863CF67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gaia\gaia.h"

/////////////////////////////////////////////////////////////////////////////
// CChildView window
using namespace gaia;

class CMainFrame;

class CChildView : public CWnd, public gaia::cGameHost
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:
    void OnToggleFullscreen(void) { ToggleFullscreen(); }
	HRESULT AdjustWindowForChange();
    void ChangeDevice();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT updateScene();
	HRESULT renderScene();
	void SetupGameHost();

    CD3DArcBall m_ArcBall;          // mouse rotation utility
    cSIMDMatrix m_matWorld;

	cSceneNode m_rootNode;
	cSceneNode m_modelNode;
	cModelResource* m_testModel;
	CMainFrame* m_pMainFrame;

	cVector3 m_CameraPos;
	cCamera m_camera;

	// Generated message map functions
protected:
    HWND m_hwndRenderFullScreen;
    HWND m_hwndRenderWindow;

	char m_filePath[MAX_PATH];
	char m_fileTitle[MAX_PATH];

	void loadMesh(const char* filename);

	void setNameStrings(const char* title, const char* path);

	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
    afx_msg void OnRender();
	afx_msg void OnChangedevice();
	afx_msg void OnExit();
	afx_msg void OnOpenfile();
	afx_msg void OnSinglestep();
	afx_msg void OnTogglefullscreen();
	afx_msg void OnTogglestart();
	afx_msg void OnSaveAs();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__DC47B214_62F8_4B86_AD7E_BF90D863CF67__INCLUDED_)
