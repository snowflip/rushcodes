// shader_edit.h : main header file for the SHADER_EDIT application
//

#if !defined(AFX_SHADER_EDIT_H__BE36873B_EA40_4BED_9F70_4EC39E7BD7EA__INCLUDED_)
#define AFX_SHADER_EDIT_H__BE36873B_EA40_4BED_9F70_4EC39E7BD7EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CShader_editApp:
// See shader_edit.cpp for the implementation of this class
//

class CShader_editApp : public CWinApp
{
public:
	CShader_editApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShader_editApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CShader_editApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADER_EDIT_H__BE36873B_EA40_4BED_9F70_4EC39E7BD7EA__INCLUDED_)
