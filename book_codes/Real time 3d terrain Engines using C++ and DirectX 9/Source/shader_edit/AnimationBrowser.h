#if !defined(AFX_ANIMATIONBROWSER_H__F4B8A72C_1EDD_4906_B9E2_94CA7774A646__INCLUDED_)
#define AFX_ANIMATIONBROWSER_H__F4B8A72C_1EDD_4906_B9E2_94CA7774A646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimationBrowser.h : header file
//
#include "gaia\gaia.h"

using namespace gaia;

/////////////////////////////////////////////////////////////////////////////
// CAnimationBrowser dialog
class CMainFrame;



class CAnimationBrowser : public CDialog
{
// Construction
public:
	CAnimationBrowser(CWnd* pParent = NULL);   // standard constructor
	~CAnimationBrowser(){};

	BOOL Create();

	void release();
	void rebuild(cModelResource* pModel);

	void update();

	void notifyAnimationFinished(uint32 channel);

	static void AnimationCallback(
		int eventCode,
		uint32 animChannel,
		uint32 animIndex,
		uint32 lParam,
		uint32 userData);

// Dialog Data
	//{{AFX_DATA(CAnimationBrowser)
	enum { IDD = IDD_ANIMATION };
	CSliderCtrl	m_Slider;
	CListBox	m_PlayQueue;
	CListBox	m_OverlayQueue;
	CButton	m_LoopPrimary;
	CButton	m_LoopOverlay;
	CButton	m_PlayButton;
	CListBox	m_AnimationList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimationBrowser)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CMainFrame* m_pParent;
	cModelResource* m_pResource;

	bool m_playNextPrimary;
	bool m_playNextOverlay;

	int m_nID;

	void EnableControls(bool onOff);

	// Generated message map functions
	//{{AFX_MSG(CAnimationBrowser)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnPlayStop();
	afx_msg void OnSelchangeAnimationList();
	afx_msg void OnDeleteAnim();
	afx_msg void OnAddAnim();
	afx_msg void OnAddOverlay();
	afx_msg void OnAddPrimary();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATIONBROWSER_H__F4B8A72C_1EDD_4906_B9E2_94CA7774A646__INCLUDED_)
