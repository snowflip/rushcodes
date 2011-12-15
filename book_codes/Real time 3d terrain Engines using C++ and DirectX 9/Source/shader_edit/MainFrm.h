// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__5BC41E52_CA7E_4819_A0EF_DE42CE4E460F__INCLUDED_)
#define AFX_MAINFRM_H__5BC41E52_CA7E_4819_A0EF_DE42CE4E460F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "gaia\gaia.h"
#include "gaia\game_host.h"
#include "ModelBrowserDlg.h"
#include "AnimationBrowser.h"

using namespace gaia;

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void SetupGameHost();


	ModelBrowserDlg* treeDialog() {return m_dlgModelBrowser;}
	CAnimationBrowser* animDialog() {return m_dlgAnimBrowser;}
	CChildView* childView() {return &m_wndView;}

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView    m_wndView;
	ModelBrowserDlg*  m_dlgModelBrowser;
	CAnimationBrowser* m_dlgAnimBrowser;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnRender();
	afx_msg void OnViewChangeDevice();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__5BC41E52_CA7E_4819_A0EF_DE42CE4E460F__INCLUDED_)
