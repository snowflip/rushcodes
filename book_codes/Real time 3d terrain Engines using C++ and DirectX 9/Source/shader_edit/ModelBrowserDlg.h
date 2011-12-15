#if !defined(AFX_MODELBROWSERDLG_H__AB3A704C_ADA8_4FCE_BF1F_17FE582AD48E__INCLUDED_)
#define AFX_MODELBROWSERDLG_H__AB3A704C_ADA8_4FCE_BF1F_17FE582AD48E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelBrowserDlg.h : header file
//
#include "gaia\gaia.h"

using namespace gaia;

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserDlg dialog
class CMainFrame;

class ModelBrowserDlg : public CDialog
{
// Construction
public:
	ModelBrowserDlg(CWnd* pParent = NULL);   // standard constructor
	~ModelBrowserDlg(){};

	BOOL Create();

	CImageList m_imageList;

	void setup();
	void rebuild(cModelResource* pModel);
	void recursiveAddFrame(HTREEITEM hParent, D3DXFRAME_DERIVED* pFrame);
	void updateControls();

	CTreeCtrl* treeCtrl();

	void recursiveKillTree(HTREEITEM hItem);
	void killTree();

	cRenderMethod* m_pMethod;
	int m_activeStage;
	cSurfaceMaterial* m_pMaterial;
	cEffectFile* m_pEffect;
	cModelResource* m_pModel;

	D3DXFRAME_DERIVED* m_pFrame;
	D3DXMESHCONTAINER_DERIVED* m_pMeshContainer;
	int m_iMaterial;

// Dialog Data
	//{{AFX_DATA(ModelBrowserDlg)
	enum { IDD = IDD_MODEL_BROWSER };
	CSliderCtrl	m_detailLevelSlider;
	CButton	m_saveEffect;
	CComboBox	m_renderSlot;
	CEdit	m_effectName;
	CEdit	m_methodName;
	CButton	m_saveMethodSet;
	CButton	m_loadMethodSet;
	CButton	m_loadMaterial;
	CButton	m_saveMaterial;
	CEdit	m_materialName;
	CListBox	m_TextureList;
	CEdit	m_SpecularRed;
	CEdit	m_SpecularGreen;
	CEdit	m_SpecularBlue;
	CEdit	m_SpecularAlpha;
	CButton	m_Specular;
	CSliderCtrl	m_SpecularPower;
	CButton	m_LoadTexture;
	CButton	m_LoadEffect;
	CEdit	m_EmissiveRed;
	CEdit	m_EmissiveAlpha;
	CEdit	m_EmissiveGreen;
	CEdit	m_EmissiveBlue;
	CButton	m_Emissive;
	CEdit	m_EffectFile;
	CEdit	m_DiffuseRed;
	CEdit	m_DiffuseGreen;
	CEdit	m_DiffuseBlue;
	CEdit	m_DiffuseAlpha;
	CButton	m_Diffuse;
	CEdit	m_AmbientRed;
	CEdit	m_AmbientGreen;
	CEdit	m_AmbientBlue;
	CEdit	m_AmbientAlpha;
	CButton	m_Ambient;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelBrowserDlg)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CMainFrame* m_pParent;
	int m_nID;

	void EnableControls(bool onOff);
	void EnableEffectSection(bool onOff);
	void EnableMaterialSection(bool onOff);

	volatile bool m_resettingData;

	// Generated message map functions
	//{{AFX_MSG(ModelBrowserDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangedModelTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAmbient();
	afx_msg void OnChangeAmbientAlpha();
	afx_msg void OnChangeAmbientBlue();
	afx_msg void OnChangeAmbientGreen();
	afx_msg void OnChangeAmbientRed();
	afx_msg void OnDiffuse();
	afx_msg void OnChangeDiffuseAlpha();
	afx_msg void OnChangeDiffuseBlue();
	afx_msg void OnChangeDiffuseGreen();
	afx_msg void OnChangeDiffuseRed();
	afx_msg void OnEmissive();
	afx_msg void OnChangeEmissiveAlpha();
	afx_msg void OnChangeEmissiveBlue();
	afx_msg void OnChangeEmissiveGreen();
	afx_msg void OnChangeEmissiveRed();
	afx_msg void OnLoadEffect();
	afx_msg void OnLoadTexture();
	afx_msg void OnSpecular();
	afx_msg void OnChangeSpecularAlpha();
	afx_msg void OnChangeSpecularBlue();
	afx_msg void OnChangeSpecularGreen();
	afx_msg void OnChangeSpecularRed();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLoadMaterial();
	afx_msg void OnSaveMaterial();
	afx_msg void OnLoadMethodSet();
	afx_msg void OnSaveMethodSet();
	afx_msg void OnSaveEffect();
	afx_msg void OnSelchangeRenderSlot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELBROWSERDLG_H__AB3A704C_ADA8_4FCE_BF1F_17FE582AD48E__INCLUDED_)
