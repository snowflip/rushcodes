// ModelBrowserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "shader_edit.h"
#include "ModelBrowserDlg.h"
#include "MainFrm.h"
#include <iostream.h>
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserDlg dialog
BOOL BuildRelativePathTo(
    LPTSTR  pszPath,
    LPCTSTR pszFrom,
    DWORD   dwAttrFrom,
    LPCTSTR pszTo,
    DWORD   dwAttrTo
    )
{
	BOOL result = PathRelativePathTo(
		pszPath,
		pszFrom,
		dwAttrFrom,
		pszTo,
		dwAttrTo);

	if (result)
	{
		// swap all backslashes to forward slashes
		int length = lstrlen(pszPath);

		for (int i=0;i<length;++i)
		{
			if (pszPath[i]=='\\')
			{
				pszPath[i]='/';
			}
		}

		// also just remove opening "./" directory settings
		if (memcmp(pszPath, "./", 2)==0)
		{
			for (int i=2;i<length;++i)
			{
				pszPath[i-2]=pszPath[i];
			}
			pszPath[length-2] = 0;
		}
	}

	return result;
}


struct TreeListEntry
{
	D3DXFRAME_DERIVED* frame;
	D3DXMESHCONTAINER_DERIVED* mesh_container;
	int iMaterial;
	cRenderMethod* method;
};

ModelBrowserDlg::ModelBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ModelBrowserDlg::IDD, pParent)
{
	ASSERT(pParent != NULL);

	m_pParent = (CMainFrame*)pParent;
	m_nID = ModelBrowserDlg::IDD;
	m_pMaterial = 0;
	m_pMethod = 0;
	m_pEffect = 0;
	m_activeStage=0;
	m_pFrame=0;
	m_pMeshContainer=0;
	m_iMaterial=0;
	m_resettingData = false;
	m_pModel = 0;
	//{{AFX_DATA_INIT(ModelBrowserDlg)
	//}}AFX_DATA_INIT
}


void ModelBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModelBrowserDlg)
	DDX_Control(pDX, IDC_DETAIL_LEVEL, m_detailLevelSlider);
	DDX_Control(pDX, IDC_SAVE_EFFECT, m_saveEffect);
	DDX_Control(pDX, IDC_RENDER_SLOT, m_renderSlot);
	DDX_Control(pDX, IDC_EFFECT_NAME, m_effectName);
	DDX_Control(pDX, IDC_METHOD_NAME, m_methodName);
	DDX_Control(pDX, IDC_SAVE_METHOD_SET, m_saveMethodSet);
	DDX_Control(pDX, IDC_LOAD_METHOD_SET, m_loadMethodSet);
	DDX_Control(pDX, IDC_LOAD_MATERIAL, m_loadMaterial);
	DDX_Control(pDX, IDC_SAVE_MATERIAL, m_saveMaterial);
	DDX_Control(pDX, IDC_MATERIAL_NAME, m_materialName);
	DDX_Control(pDX, IDC_TEXTURE_LIST, m_TextureList);
	DDX_Control(pDX, IDC_SPECULAR_RED, m_SpecularRed);
	DDX_Control(pDX, IDC_SPECULAR_GREEN, m_SpecularGreen);
	DDX_Control(pDX, IDC_SPECULAR_BLUE, m_SpecularBlue);
	DDX_Control(pDX, IDC_SPECULAR_ALPHA, m_SpecularAlpha);
	DDX_Control(pDX, IDC_SPECULAR, m_Specular);
	DDX_Control(pDX, IDC_SPEC_POWER, m_SpecularPower);
	DDX_Control(pDX, IDC_LOAD_TEXTURE, m_LoadTexture);
	DDX_Control(pDX, IDC_LOAD_EFFECT, m_LoadEffect);
	DDX_Control(pDX, IDC_EMISSIVE_RED, m_EmissiveRed);
	DDX_Control(pDX, IDC_EMISSIVE_ALPHA, m_EmissiveAlpha);
	DDX_Control(pDX, IDC_EMISSIVE_GREEN, m_EmissiveGreen);
	DDX_Control(pDX, IDC_EMISSIVE_BLUE, m_EmissiveBlue);
	DDX_Control(pDX, IDC_EMISSIVE, m_Emissive);
	DDX_Control(pDX, IDC_DIFFUSE_RED, m_DiffuseRed);
	DDX_Control(pDX, IDC_DIFFUSE_GREEN, m_DiffuseGreen);
	DDX_Control(pDX, IDC_DIFFUSE_BLUE, m_DiffuseBlue);
	DDX_Control(pDX, IDC_DIFFUSE_ALPHA, m_DiffuseAlpha);
	DDX_Control(pDX, IDC_DIFFUSE, m_Diffuse);
	DDX_Control(pDX, IDC_AMBIENT_RED, m_AmbientRed);
	DDX_Control(pDX, IDC_AMBIENT_GREEN, m_AmbientGreen);
	DDX_Control(pDX, IDC_AMBIENT_BLUE, m_AmbientBlue);
	DDX_Control(pDX, IDC_AMBIENT_ALPHA, m_AmbientAlpha);
	DDX_Control(pDX, IDC_AMBIENT, m_Ambient);
	DDX_Control(pDX, IDC_MODEL_TREE, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ModelBrowserDlg, CDialog)
	//{{AFX_MSG_MAP(ModelBrowserDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MODEL_TREE, OnSelchangedModelTree)
	ON_BN_CLICKED(IDC_AMBIENT, OnAmbient)
	ON_EN_CHANGE(IDC_AMBIENT_ALPHA, OnChangeAmbientAlpha)
	ON_EN_CHANGE(IDC_AMBIENT_BLUE, OnChangeAmbientBlue)
	ON_EN_CHANGE(IDC_AMBIENT_GREEN, OnChangeAmbientGreen)
	ON_EN_CHANGE(IDC_AMBIENT_RED, OnChangeAmbientRed)
	ON_BN_CLICKED(IDC_DIFFUSE, OnDiffuse)
	ON_EN_CHANGE(IDC_DIFFUSE_ALPHA, OnChangeDiffuseAlpha)
	ON_EN_CHANGE(IDC_DIFFUSE_BLUE, OnChangeDiffuseBlue)
	ON_EN_CHANGE(IDC_DIFFUSE_GREEN, OnChangeDiffuseGreen)
	ON_EN_CHANGE(IDC_DIFFUSE_RED, OnChangeDiffuseRed)
	ON_BN_CLICKED(IDC_EMISSIVE, OnEmissive)
	ON_EN_CHANGE(IDC_EMISSIVE_ALPHA, OnChangeEmissiveAlpha)
	ON_EN_CHANGE(IDC_EMISSIVE_BLUE, OnChangeEmissiveBlue)
	ON_EN_CHANGE(IDC_EMISSIVE_GREEN, OnChangeEmissiveGreen)
	ON_EN_CHANGE(IDC_EMISSIVE_RED, OnChangeEmissiveRed)
	ON_BN_CLICKED(IDC_LOAD_EFFECT, OnLoadEffect)
	ON_BN_CLICKED(IDC_LOAD_TEXTURE, OnLoadTexture)
	ON_BN_CLICKED(IDC_SPECULAR, OnSpecular)
	ON_EN_CHANGE(IDC_SPECULAR_ALPHA, OnChangeSpecularAlpha)
	ON_EN_CHANGE(IDC_SPECULAR_BLUE, OnChangeSpecularBlue)
	ON_EN_CHANGE(IDC_SPECULAR_GREEN, OnChangeSpecularGreen)
	ON_EN_CHANGE(IDC_SPECULAR_RED, OnChangeSpecularRed)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_LOAD_MATERIAL, OnLoadMaterial)
	ON_BN_CLICKED(IDC_SAVE_MATERIAL, OnSaveMaterial)
	ON_BN_CLICKED(IDC_LOAD_METHOD_SET, OnLoadMethodSet)
	ON_BN_CLICKED(IDC_SAVE_METHOD_SET, OnSaveMethodSet)
	ON_BN_CLICKED(IDC_SAVE_EFFECT, OnSaveEffect)
	ON_CBN_SELCHANGE(IDC_RENDER_SLOT, OnSelchangeRenderSlot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserDlg message handlers

BOOL ModelBrowserDlg::Create()
{
	BOOL result=CDialog::Create(m_nID, m_pParent);
	if (result)
	{
		setup();
	}
	return result;
}

CTreeCtrl* ModelBrowserDlg::treeCtrl()
{
	return (CTreeCtrl*)GetDlgItem(IDC_MODEL_TREE);
}

void ModelBrowserDlg::setup()
{
	m_imageList.Create(IDB_IMAGE_LIST, 16, 16, 0x00ffffff);
	m_Tree.SetImageList(&m_imageList, TVSIL_NORMAL);

	m_Tree.DeleteAllItems();
	EnableControls(false);

	m_SpecularPower.SetRange(0, 100, TRUE);
	m_SpecularPower.SetTicFreq(10);
	m_detailLevelSlider.SetRange(0, cModelResource::k_maxLOD, TRUE);
	m_detailLevelSlider.SetTicFreq(1);

}


void ModelBrowserDlg::recursiveAddFrame(HTREEITEM hParent, D3DXFRAME_DERIVED* pFrame)
{
	static char* unnamed = "<unnamed node>";

	char* pname = pFrame->Name;
	if (!pname)
	{
		pname = unnamed;
	}

	TreeListEntry* pFrameData = new TreeListEntry;
	memset(pFrameData,0,sizeof(TreeListEntry));

	pFrameData->frame = pFrame;

	HTREEITEM localItem= m_Tree.InsertItem(pname, 
									0, 
									0, 
									hParent, 
									TVI_LAST);

	m_Tree.EnsureVisible(localItem);

	m_Tree.SetItemData(localItem, (DWORD)pFrameData);

	if (pFrame->pMeshContainer)
	{
		TreeListEntry* pMeshData = new TreeListEntry;
		memcpy(pMeshData,pFrameData,sizeof(TreeListEntry));

		pMeshData->mesh_container = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		char* pmeshname = pFrame->pMeshContainer->Name;
		if (!pmeshname)
		{
			pmeshname = pname;
		}
		HTREEITEM hMesh =m_Tree.InsertItem(pmeshname, 
							1, 
							1, 
							localItem, 
							TVI_LAST);

		m_Tree.EnsureVisible(hMesh);
		m_Tree.SetItemData(hMesh, (DWORD)pMeshData);

		for (unsigned int i=0; i<pFrame->pMeshContainer->NumMaterials; ++i)
		{
			TreeListEntry* pMethodData = new TreeListEntry;
			memcpy(pMethodData,pMeshData,sizeof(TreeListEntry));

			pMethodData->method = pMeshData->mesh_container->ppRenderMethodList[i];
			pMethodData->iMaterial = i;

			char method_name[255];
			sprintf(method_name, "method :%i", i);

			HTREEITEM hMat =m_Tree.InsertItem(method_name, 
								2, 
								2, 
								hMesh, 
								TVI_LAST);

			m_Tree.EnsureVisible(hMat);
			m_Tree.SetItemData(hMat, (DWORD)pMethodData);
		}
	}

	if (pFrame->pFrameSibling)
	{
		recursiveAddFrame(hParent, (D3DXFRAME_DERIVED*) pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild)
	{
		recursiveAddFrame(localItem, (D3DXFRAME_DERIVED*) pFrame->pFrameFirstChild);
	}
}

void ModelBrowserDlg::recursiveKillTree(HTREEITEM hItem)
{
	if (hItem)
	{
		void* pData = (void*)m_Tree.GetItemData(hItem);
		safe_delete(pData);

		HTREEITEM hSibling = m_Tree.GetNextSiblingItem(hItem);
		if (hSibling)
		{
			recursiveKillTree(hSibling);
		}

		HTREEITEM hChild = m_Tree.GetChildItem(hItem);
		if (hChild)
		{
			recursiveKillTree(hChild);
		}
	}
}

void ModelBrowserDlg::killTree()
{
	recursiveKillTree(m_Tree.GetRootItem( ));
	m_Tree.DeleteAllItems();
	EnableControls(false);
}

void ModelBrowserDlg::rebuild(cModelResource* pModel)
{
	// destroy the tree
	killTree();

	m_pModel = pModel;
	m_detailLevelSlider.EnableWindow(false);

	// add the model resource data
	if (pModel)
	{
		D3DXFRAME_DERIVED* pFrame = pModel->getRootFrame();

		// recursivly add all frames to the tree
		recursiveAddFrame(TVI_ROOT, pFrame);
		m_Tree.EnsureVisible(m_Tree.GetRootItem( ));
		m_detailLevelSlider.EnableWindow(pModel->containsProgressiveMesh());
		m_detailLevelSlider.SetPos(m_pModel->getLOD());
	}
}

void ModelBrowserDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//	killTree();

//	CDialog::OnClose();
}

void ModelBrowserDlg::OnDestroy() 
{
	killTree();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void ModelBrowserDlg::EnableMaterialSection(bool onOff)
{
	if (!m_pMaterial) onOff = false;

	m_TextureList.EnableWindow(onOff);
	m_LoadTexture.EnableWindow(onOff);
	m_SpecularRed.EnableWindow(onOff);
	m_SpecularGreen.EnableWindow(onOff);
	m_SpecularBlue.EnableWindow(onOff);
	m_SpecularAlpha.EnableWindow(onOff);
	m_Specular.EnableWindow(onOff);
	m_SpecularPower.EnableWindow(onOff);
	m_EmissiveRed.EnableWindow(onOff);
	m_EmissiveAlpha.EnableWindow(onOff);
	m_EmissiveGreen.EnableWindow(onOff);
	m_EmissiveBlue.EnableWindow(onOff);
	m_Emissive.EnableWindow(onOff);
	m_DiffuseRed.EnableWindow(onOff);
	m_DiffuseGreen.EnableWindow(onOff);
	m_DiffuseBlue.EnableWindow(onOff);
	m_DiffuseAlpha.EnableWindow(onOff);
	m_Diffuse.EnableWindow(onOff);
	m_AmbientRed.EnableWindow(onOff);
	m_AmbientGreen.EnableWindow(onOff);
	m_AmbientBlue.EnableWindow(onOff);
	m_AmbientAlpha.EnableWindow(onOff);
	m_Ambient.EnableWindow(onOff);
	m_saveMaterial.EnableWindow(onOff);
	m_materialName.EnableWindow(onOff);
}

void ModelBrowserDlg::EnableEffectSection(bool onOff)
{
	if (!m_pEffect) onOff = false;
	m_saveEffect.EnableWindow(onOff);
	m_effectName.EnableWindow(onOff);
}

void ModelBrowserDlg::EnableControls(bool onOff)
{
	m_loadMaterial.EnableWindow(onOff);
	m_loadMethodSet.EnableWindow(onOff);
	m_saveMethodSet.EnableWindow(onOff);
	m_LoadEffect.EnableWindow(onOff);
	m_renderSlot.EnableWindow(onOff);

	EnableEffectSection(onOff);
	EnableMaterialSection(onOff);

	if (!onOff)
	{
		m_TextureList.ResetContent( );
		m_materialName.SetWindowText("<none>");
		m_methodName.SetWindowText("<none>");
		m_effectName.SetWindowText("<none>");
	}
}

void ModelBrowserDlg::updateControls()
{
	int i;

	if (m_pModel)
	{
		m_detailLevelSlider.SetPos(m_pModel->getLOD());
	}
	else
	{
		m_detailLevelSlider.SetPos(cModelResource::k_maxLOD);
	}

	// setup all data
	if (m_pMethod)
	{
		const cString* name = m_pMethod->findResourceName();
		if (name)
		{
			m_methodName.SetWindowText(name->c_str());
		}
	}
	else
	{
		m_methodName.SetWindowText("<unused>");
	}

	if (m_pEffect) 
	{
		EnableEffectSection(true);
		const cString* name = m_pEffect->findResourceName();
		if (name)
		{
			m_effectName.SetWindowText(name->c_str());
		}
	}
	else
	{
		EnableEffectSection(false);
		m_effectName.SetWindowText("<unused>");
	}
	
	m_TextureList.ResetContent();
	if (m_pMaterial) 
	{
		EnableMaterialSection(true);
		const cString* name = m_pMaterial->findResourceName();
		if (name)
		{
			m_materialName.SetWindowText(name->c_str());
		}

		for (i=0; i<cSurfaceMaterial::k_maxTexturesPerSurface;++i)
		{
			cTexture* pTexture = m_pMaterial->texture(i);
			char text[MAX_PATH];

			sprintf(text, "%02i %s", i, "<none>");
			if (pTexture)
			{
				const cString* name = pTexture->findResourceName();

				if (name)
				{
					sprintf(text, "%02i %s", i, name->c_str());
				}
			}

			m_TextureList.AddString(text);
		}

		SetDlgItemInt(IDC_DIFFUSE_RED, (int)(m_pMaterial->d3dMaterial().Diffuse.r * 255.0f), TRUE);
		SetDlgItemInt(IDC_DIFFUSE_GREEN, (int)(m_pMaterial->d3dMaterial().Diffuse.g * 255.0f), TRUE);
		SetDlgItemInt(IDC_DIFFUSE_BLUE, (int)(m_pMaterial->d3dMaterial().Diffuse.b * 255.0f), TRUE);
		SetDlgItemInt(IDC_DIFFUSE_ALPHA, (int)(m_pMaterial->d3dMaterial().Diffuse.a * 255.0f), TRUE);

		SetDlgItemInt(IDC_AMBIENT_RED, (int)(m_pMaterial->d3dMaterial().Ambient.r * 255.0f), TRUE);
		SetDlgItemInt(IDC_AMBIENT_GREEN, (int)(m_pMaterial->d3dMaterial().Ambient.g * 255.0f), TRUE);
		SetDlgItemInt(IDC_AMBIENT_BLUE, (int)(m_pMaterial->d3dMaterial().Ambient.b * 255.0f), TRUE);
		SetDlgItemInt(IDC_AMBIENT_ALPHA, (int)(m_pMaterial->d3dMaterial().Ambient.a * 255.0f), TRUE);

		SetDlgItemInt(IDC_EMISSIVE_RED, (int)(m_pMaterial->d3dMaterial().Emissive.r * 255.0f), TRUE);
		SetDlgItemInt(IDC_EMISSIVE_GREEN, (int)(m_pMaterial->d3dMaterial().Emissive.g * 255.0f), TRUE);
		SetDlgItemInt(IDC_EMISSIVE_BLUE, (int)(m_pMaterial->d3dMaterial().Emissive.b * 255.0f), TRUE);
		SetDlgItemInt(IDC_EMISSIVE_ALPHA, (int)(m_pMaterial->d3dMaterial().Emissive.a * 255.0f), TRUE);

		SetDlgItemInt(IDC_SPECULAR_RED, (int)(m_pMaterial->d3dMaterial().Specular.r * 255.0f), TRUE);
		SetDlgItemInt(IDC_SPECULAR_GREEN, (int)(m_pMaterial->d3dMaterial().Specular.g * 255.0f), TRUE);
		SetDlgItemInt(IDC_SPECULAR_BLUE, (int)(m_pMaterial->d3dMaterial().Specular.b * 255.0f), TRUE);
		SetDlgItemInt(IDC_SPECULAR_ALPHA, (int)(m_pMaterial->d3dMaterial().Specular.a * 255.0f), TRUE);
		
		m_SpecularPower.SetPos((int)(m_pMaterial->d3dMaterial().Power * 100.0f));


	}
	else
	{
		EnableMaterialSection(false);
		m_materialName.SetWindowText("<unused>");

		SetDlgItemInt(IDC_DIFFUSE_RED, 0, TRUE);
		SetDlgItemInt(IDC_DIFFUSE_GREEN, 0, TRUE);
		SetDlgItemInt(IDC_DIFFUSE_BLUE, 0, TRUE);
		SetDlgItemInt(IDC_DIFFUSE_ALPHA, 0, TRUE);

		SetDlgItemInt(IDC_AMBIENT_RED, 0, TRUE);
		SetDlgItemInt(IDC_AMBIENT_GREEN, 0, TRUE);
		SetDlgItemInt(IDC_AMBIENT_BLUE, 0, TRUE);
		SetDlgItemInt(IDC_AMBIENT_ALPHA, 0, TRUE);

		SetDlgItemInt(IDC_EMISSIVE_RED, 0, TRUE);
		SetDlgItemInt(IDC_EMISSIVE_GREEN, 0, TRUE);
		SetDlgItemInt(IDC_EMISSIVE_BLUE, 0, TRUE);
		SetDlgItemInt(IDC_EMISSIVE_ALPHA, 0, TRUE);

		SetDlgItemInt(IDC_SPECULAR_RED, 0, TRUE);
		SetDlgItemInt(IDC_SPECULAR_GREEN, 0, TRUE);
		SetDlgItemInt(IDC_SPECULAR_BLUE, 0, TRUE);
		SetDlgItemInt(IDC_SPECULAR_ALPHA, 0, TRUE);
		
		m_SpecularPower.SetPos(0);
	
	}


	m_renderSlot.SetCurSel(m_activeStage);
}

void ModelBrowserDlg::OnSelchangedModelTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool success = false;

	m_resettingData = true;

	HTREEITEM hSelection = m_Tree.GetSelectedItem();

	if (hSelection)
	{
		TreeListEntry* pData = (TreeListEntry*)m_Tree.GetItemData(hSelection);

		if (pData && pData->method)
		{
			success = true;
			EnableControls(false);

			// setup all data
			m_pMethod = pData->method;
			m_activeStage = 0;
			m_pMaterial = m_pMethod->getMaterial(m_activeStage);
			m_pEffect = m_pMethod->getEffect(m_activeStage);
			m_pFrame=  pData->frame;
			m_pMeshContainer=  pData->mesh_container;
			m_iMaterial=  pData->iMaterial;

			EnableControls(true);
			updateControls();
		}
	}

	if (!success)
	{
		EnableControls(false);
		m_pMethod=0;
		m_pMaterial = 0;
		m_pEffect = 0;
		m_pFrame=0;
		m_pMeshContainer=0;
		m_iMaterial=0;
	}

	m_resettingData = false;

	*pResult = 0;
}



void ModelBrowserDlg::OnAmbient() 
{
	// TODO: Add your control notification handler code here
	int r = GetDlgItemInt(IDC_AMBIENT_RED, NULL, TRUE) & 0xff;
	int g = GetDlgItemInt(IDC_AMBIENT_GREEN, NULL, TRUE) & 0xff;
	int b = GetDlgItemInt(IDC_AMBIENT_BLUE, NULL, TRUE) & 0xff;
	COLORREF currentColor = (b<<16) | (g << 8) | (r);
	
	CColorDialog colorDialog(currentColor, 0, this);
	if (colorDialog.DoModal() == IDOK)
	{
		currentColor=colorDialog.GetColor( );
		int r = currentColor & 0xff;
		int g = (currentColor>>8) & 0xff;
		int b = (currentColor>>16) & 0xff;

		SetDlgItemInt(IDC_AMBIENT_RED, r, TRUE);
		SetDlgItemInt(IDC_AMBIENT_GREEN, g, TRUE);
		SetDlgItemInt(IDC_AMBIENT_BLUE, b, TRUE);
	}
}

void ModelBrowserDlg::OnChangeAmbientAlpha() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_AMBIENT_ALPHA, NULL, TRUE);
		m_pMaterial->d3dMaterial().Ambient.a = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeAmbientBlue() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_AMBIENT_BLUE, NULL, TRUE);
		m_pMaterial->d3dMaterial().Ambient.b = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeAmbientGreen() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_AMBIENT_GREEN, NULL, TRUE);
		m_pMaterial->d3dMaterial().Ambient.g = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeAmbientRed() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_AMBIENT_RED, NULL, TRUE);
		m_pMaterial->d3dMaterial().Ambient.r = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnDiffuse() 
{
	int r = GetDlgItemInt(IDC_DIFFUSE_RED, NULL, TRUE) & 0xff;
	int g = GetDlgItemInt(IDC_DIFFUSE_GREEN, NULL, TRUE) & 0xff;
	int b = GetDlgItemInt(IDC_DIFFUSE_BLUE, NULL, TRUE) & 0xff;
	COLORREF currentColor = (b<<16) | (g << 8) | (r);
	
	CColorDialog colorDialog(currentColor, 0, this);
	if (colorDialog.DoModal() == IDOK)
	{
		currentColor=colorDialog.GetColor( );
		int r = currentColor & 0xff;
		int g = (currentColor>>8) & 0xff;
		int b = (currentColor>>16) & 0xff;

		SetDlgItemInt(IDC_DIFFUSE_RED, r, TRUE);
		SetDlgItemInt(IDC_DIFFUSE_GREEN, g, TRUE);
		SetDlgItemInt(IDC_DIFFUSE_BLUE, b, TRUE);
	}
}

void ModelBrowserDlg::OnChangeDiffuseAlpha() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_DIFFUSE_ALPHA, NULL, TRUE);
		m_pMaterial->d3dMaterial().Diffuse.a = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeDiffuseBlue() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_DIFFUSE_BLUE, NULL, TRUE);
		m_pMaterial->d3dMaterial().Diffuse.b =(float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeDiffuseGreen() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_DIFFUSE_GREEN, NULL, TRUE);
		m_pMaterial->d3dMaterial().Diffuse.g = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeDiffuseRed() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_DIFFUSE_RED, NULL, TRUE);
		m_pMaterial->d3dMaterial().Diffuse.r = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnEmissive() 
{
	int r = GetDlgItemInt(IDC_EMISSIVE_RED, NULL, TRUE) & 0xff;
	int g = GetDlgItemInt(IDC_EMISSIVE_GREEN, NULL, TRUE) & 0xff;
	int b = GetDlgItemInt(IDC_EMISSIVE_BLUE, NULL, TRUE) & 0xff;
	COLORREF currentColor = (b<<16) | (g << 8) | (r);
	
	CColorDialog colorDialog(currentColor, 0, this);
	if (colorDialog.DoModal() == IDOK)
	{
		currentColor=colorDialog.GetColor( );
		int r = currentColor & 0xff;
		int g = (currentColor>>8) & 0xff;
		int b = (currentColor>>16) & 0xff;

		SetDlgItemInt(IDC_EMISSIVE_RED, r, TRUE);
		SetDlgItemInt(IDC_EMISSIVE_GREEN, g, TRUE);
		SetDlgItemInt(IDC_EMISSIVE_BLUE, b, TRUE);
	}
}

void ModelBrowserDlg::OnChangeEmissiveAlpha() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_EMISSIVE_ALPHA, NULL, TRUE);
		m_pMaterial->d3dMaterial().Emissive.a = (float)value/255.0f;

	}
}

void ModelBrowserDlg::OnChangeEmissiveBlue() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_EMISSIVE_BLUE, NULL, TRUE);
		m_pMaterial->d3dMaterial().Emissive.b = (float)value/255.0f;

	}
}

void ModelBrowserDlg::OnChangeEmissiveGreen() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_EMISSIVE_GREEN, NULL, TRUE);
		m_pMaterial->d3dMaterial().Emissive.g = (float)value/255.0f;

	}
}

void ModelBrowserDlg::OnChangeEmissiveRed() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_EMISSIVE_RED, NULL, TRUE);
		m_pMaterial->d3dMaterial().Emissive.r = (float)value/255.0f;

	}
}



void ModelBrowserDlg::OnLoadTexture() 
{
	if (m_pMaterial)
	{
		int selection = m_TextureList.GetCurSel( );

		if (selection == LB_ERR)
		{
			MessageBox("Please select a texture slot first", "No Slot Selected", MB_OK);
			return;
		}

		static char szFilter[] = "Texture Files|*.dds;*.bmp;*.tga;*.jpg||";

		CFileDialog fileDialog(TRUE, ".dds", NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

		if (fileDialog.DoModal() == IDOK)
		{
			SetCurrentDirectory(TheGameHost.rootPath());
			char relativePath[MAX_PATH];

			// convert to a relative path
			BuildRelativePathTo(  relativePath,
								 TheGameHost.rootPath(),
								 FILE_ATTRIBUTE_DIRECTORY,
								 fileDialog.GetPathName( ),
								 FILE_ATTRIBUTE_NORMAL);

			if (!m_pMaterial->setTexture(selection, relativePath))
			{
				MessageBox("Unable to Load Texture", "Error", MB_OK);
			}
			else
			{
				m_TextureList.DeleteString(selection);
				m_TextureList.InsertString(selection,relativePath);
				updateControls();
			}
		}
	}
}

void ModelBrowserDlg::OnSpecular() 
{
	int r = GetDlgItemInt(IDC_SPECULAR_RED, NULL, TRUE) & 0xff;
	int g = GetDlgItemInt(IDC_SPECULAR_GREEN, NULL, TRUE) & 0xff;
	int b = GetDlgItemInt(IDC_SPECULAR_BLUE, NULL, TRUE) & 0xff;
	COLORREF currentColor = (b<<16) | (g << 8) | (r);
	
	CColorDialog colorDialog(currentColor, 0, this);
	if (colorDialog.DoModal() == IDOK)
	{
		currentColor=colorDialog.GetColor( );
		int r = currentColor & 0xff;
		int g = (currentColor>>8) & 0xff;
		int b = (currentColor>>16) & 0xff;

		SetDlgItemInt(IDC_SPECULAR_RED, r, TRUE);
		SetDlgItemInt(IDC_SPECULAR_GREEN, g, TRUE);
		SetDlgItemInt(IDC_SPECULAR_BLUE, b, TRUE);
	}
}

void ModelBrowserDlg::OnChangeSpecularAlpha() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_SPECULAR_ALPHA, NULL, TRUE);
		m_pMaterial->d3dMaterial().Specular.a = (float)value/255.0f;

	}
}

void ModelBrowserDlg::OnChangeSpecularBlue() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_SPECULAR_BLUE, NULL, TRUE);
		m_pMaterial->d3dMaterial().Specular.b = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeSpecularGreen() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_SPECULAR_GREEN, NULL, TRUE);
		m_pMaterial->d3dMaterial().Specular.g = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnChangeSpecularRed() 
{
	if (m_pMaterial && !m_resettingData)
	{
		int value = GetDlgItemInt(IDC_SPECULAR_RED, NULL, TRUE);
		m_pMaterial->d3dMaterial().Specular.r = (float)value/255.0f;
	}
}

void ModelBrowserDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (m_pMaterial && !m_resettingData)
	{
		if(&m_SpecularPower == (CSliderCtrl*)pScrollBar)
		{
			int pos = m_SpecularPower.GetPos();
			m_pMaterial->d3dMaterial().Power = (float)pos/100.0f;
		}
	}

	if (m_pModel && &m_detailLevelSlider == (CSliderCtrl*)pScrollBar)
	{
		int pos = m_detailLevelSlider.GetPos();
		m_pModel->setLOD(pos);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void ModelBrowserDlg::OnLoadMaterial() 
{
	if (m_pMethod)
	{
		static char szFilter[] = "Material Files (*.mat)|*.mat|All Files (*.*)|*.*||";

		CFileDialog fileDialog(TRUE, "mat", NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

		if (fileDialog.DoModal() == IDOK)
		{
			SetCurrentDirectory(TheGameHost.rootPath());
			char relativePath[MAX_PATH];

			// convert to a relative path
			BuildRelativePathTo(  relativePath,
								 TheGameHost.rootPath(),
								 FILE_ATTRIBUTE_DIRECTORY,
								 fileDialog.GetPathName( ),
								 FILE_ATTRIBUTE_NORMAL);

			cSurfaceMaterial* pNewMaterial = 
				DisplayManager.surfaceMaterialPool().loadResource(relativePath);
			if (!pNewMaterial)
			{
				MessageBox("Unable to Load Material", "Error", MB_OK);
			}
			else
			{
				m_pMethod->setMaterial(m_activeStage, pNewMaterial);
				m_pMaterial = pNewMaterial;
				safe_release(pNewMaterial);
				updateControls();
			}
		}
	}
	
}

void ModelBrowserDlg::OnSaveMaterial() 
{
	if (m_pMaterial)
	{
		static char szFilter[] = "Material Files (*.mat)|*.mat|All Files (*.*)|*.*||";
		CFileDialog fileDialog(FALSE, "mat", 0, OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

		if (fileDialog.DoModal() == IDOK)
		{
			char relativePath[MAX_PATH];

			// convert to a relative path
			SetCurrentDirectory(TheGameHost.rootPath());
			BuildRelativePathTo(  relativePath,
								 TheGameHost.rootPath(),
								 FILE_ATTRIBUTE_DIRECTORY,
								 fileDialog.GetPathName( ),
								 FILE_ATTRIBUTE_NORMAL);

			if (!m_pMaterial->saveResource(relativePath))
			{
				MessageBox("Unable to Save Material", "Error", MB_OK);
			}
			else
			{
				m_pMaterial->setResourceName(relativePath);
				updateControls();
			}
		}
	}
}

BOOL ModelBrowserDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// prevent this window from closing in reaction to the enter key being pressed
	if (nID == 1 && (nCode == -1 || nCode == 0)) return false;
//	cString text("nID= %i, nCode= %i\n", nID, nCode);
//	OutputDebugString(text);

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void ModelBrowserDlg::OnLoadMethodSet() 
{
	if (m_pMeshContainer && m_pMeshContainer->ppRenderMethodList)
	{
		static char szFilter[] = "Render Method Files(*.grm)|*.grm||";

		CFileDialog fileDialog(TRUE, ".grm", NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

		if (fileDialog.DoModal() == IDOK)
		{
			SetCurrentDirectory(TheGameHost.rootPath());
			char relativePath[MAX_PATH];

			// convert to a relative path
			BuildRelativePathTo(  relativePath,
								 TheGameHost.rootPath(),
								 FILE_ATTRIBUTE_DIRECTORY,
								 fileDialog.GetPathName( ),
								 FILE_ATTRIBUTE_NORMAL);

			cRenderMethod* pNewMethod = DisplayManager.renderMethodPool().loadResource(relativePath);
			if (!pNewMethod)
			{
				MessageBox("Unable to Load Method File", "Error", MB_OK);
			}
			else
			{
				safe_release(m_pMeshContainer->ppRenderMethodList[m_iMaterial]);
				m_pMeshContainer->ppRenderMethodList[m_iMaterial] = pNewMethod;
				m_methodName.SetWindowText(relativePath);
				m_pMethod = pNewMethod;
				m_activeStage = 0;
				m_renderSlot.SetCurSel(m_activeStage);
				m_pMaterial = m_pMethod->getMaterial(m_activeStage);
				m_pEffect = m_pMethod->getEffect(m_activeStage);
				updateControls();
			}
		}
	}
}

void ModelBrowserDlg::OnSaveMethodSet() 
{
	if (m_pMethod)
	{
		static char szFilter[] = "Render Method Files(*.grm)|*.grm||";
		CFileDialog fileDialog(FALSE, "grm", 0, OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

		if (fileDialog.DoModal() == IDOK)
		{
			SetCurrentDirectory(TheGameHost.rootPath());
			char relativePath[MAX_PATH];

			// convert to a relative path
			BuildRelativePathTo(  relativePath,
								 TheGameHost.rootPath(),
								 FILE_ATTRIBUTE_DIRECTORY,
								 fileDialog.GetPathName( ),
								 FILE_ATTRIBUTE_NORMAL);

			if (!m_pMethod->saveResource(relativePath))
			{
				MessageBox("Unable to Save Method", "Error", MB_OK);
			}
			else
			{
				// reload the resource
				m_pMethod->setResourceName(relativePath);
				updateControls();
			}
		}
	}
}

void ModelBrowserDlg::OnLoadEffect() 
{
	if (m_pMethod)
	{
		static char szFilter[] = "Effect Files(*.fx)|*.fx||";

		CFileDialog fileDialog(TRUE, ".fx", NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

		if (fileDialog.DoModal() == IDOK)
		{
			SetCurrentDirectory(TheGameHost.rootPath());
			char relativePath[MAX_PATH];

			// convert to a relative path
			BuildRelativePathTo(  relativePath,
								 TheGameHost.rootPath(),
								 FILE_ATTRIBUTE_DIRECTORY,
								 fileDialog.GetPathName( ),
								 FILE_ATTRIBUTE_NORMAL);

			cEffectFile* newEffect = DisplayManager.effectFilePool().loadResource(relativePath);
			if (!newEffect)
			{
				MessageBox("Unable to Load Effect File", "Error", MB_OK);
			}
			else
			{
				m_pMethod->setEffect(m_activeStage, newEffect);
				m_pEffect = newEffect;
				safe_release(newEffect);
				updateControls();
			}
		}
	}
}


void ModelBrowserDlg::OnSaveEffect() 
{
	if (m_pEffect)
	{
		static char szFilter[] = "Effect Files(*.fx)|*.fx||";
		CFileDialog fileDialog(FALSE, "fx", 0, OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

		if (fileDialog.DoModal() == IDOK)
		{
			SetCurrentDirectory(TheGameHost.rootPath());
			char relativePath[MAX_PATH];

			// convert to a relative path
			BuildRelativePathTo(  relativePath,
								 TheGameHost.rootPath(),
								 FILE_ATTRIBUTE_DIRECTORY,
								 fileDialog.GetPathName( ),
								 FILE_ATTRIBUTE_NORMAL);

			if (!m_pEffect->saveResource(relativePath))
			{
				MessageBox("Unable to Save Effect", "Error", MB_OK);
			}
			else
			{
				m_pEffect->setResourceName(relativePath);
				updateControls();
			}
		}
	}
}

void ModelBrowserDlg::OnSelchangeRenderSlot() 
{
	if (m_pMethod && !m_resettingData)
	{
		m_activeStage = m_renderSlot.GetCurSel();
		m_pMaterial = m_pMethod->getMaterial(m_activeStage);
		m_pEffect = m_pMethod->getEffect(m_activeStage);
		updateControls();
	}
}

