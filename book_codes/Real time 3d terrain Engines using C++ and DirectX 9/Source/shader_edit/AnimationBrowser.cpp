// AnimationBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "shader_edit.h"
#include "AnimationBrowser.h"
#include "MainFrm.h"
#include "gaia\gaia.h"
#include <iostream>
#include "Shlwapi.h"
#include "gaia\d3dx_frame_manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimationBrowser dialog

void CAnimationBrowser::AnimationCallback(int eventCode,
										  uint32 animChannel,
										  uint32 animIndex,
										  uint32 lParam,
										  uint32 userData)
{
	CAnimationBrowser* pAnimBrowser = (CAnimationBrowser*)userData;

	if (eventCode == cAnimationController::k_playEventEnd)
	{
		pAnimBrowser->notifyAnimationFinished(animChannel);
	}
}


CAnimationBrowser::CAnimationBrowser(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationBrowser::IDD, pParent)
{
	ASSERT(pParent != NULL);

	m_pResource = 0;
	m_pParent = (CMainFrame*)pParent;
	m_nID = CAnimationBrowser::IDD;
	m_playNextPrimary = true;
	m_playNextOverlay = true;

	//{{AFX_DATA_INIT(CAnimationBrowser)
	//}}AFX_DATA_INIT
}


void CAnimationBrowser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimationBrowser)
	DDX_Control(pDX, IDC_ANIMATION_SLIDER, m_Slider);
	DDX_Control(pDX, IDC_PLAY_QUEUE, m_PlayQueue);
	DDX_Control(pDX, IDC_OVERLAY_QUEUE, m_OverlayQueue);
	DDX_Control(pDX, IDC_LOOP_PRIMARY, m_LoopPrimary);
	DDX_Control(pDX, IDC_LOOP_OVERLAY, m_LoopOverlay);
	DDX_Control(pDX, IDC_PLAY_STOP, m_PlayButton);
	DDX_Control(pDX, IDC_ANIMATION_LIST, m_AnimationList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimationBrowser, CDialog)
	//{{AFX_MSG_MAP(CAnimationBrowser)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PLAY_STOP, OnPlayStop)
	ON_LBN_SELCHANGE(IDC_ANIMATION_LIST, OnSelchangeAnimationList)
	ON_BN_CLICKED(IDC_DELETE_ANIM, OnDeleteAnim)
	ON_BN_CLICKED(IDC_ADD_ANIM, OnAddAnim)
	ON_BN_CLICKED(IDC_ADD_OVERLAY, OnAddOverlay)
	ON_BN_CLICKED(IDC_ADD_PRIMARY, OnAddPrimary)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationBrowser message handlers

BOOL CAnimationBrowser::Create()
{
	BOOL result=CDialog::Create(m_nID, m_pParent);
	m_pResource = 0;

	m_Slider.SetTicFreq(10);
	m_Slider.SetRange(0, 100, TRUE);

	return result;
}

void CAnimationBrowser::release()
{
	m_AnimationList.ResetContent( );
	m_pResource = 0;
}

void CAnimationBrowser::rebuild(cModelResource* pModel)
{
	// destroy the current data set
	release();

	// add the model resource data
	if (pModel)
	{
		// add each texture slot to the list window
		uint32 totalAnimations =pModel->totalAnimations();
		tchar nameToInsert[MAX_PATH];

		for (uint32 i=0; i<totalAnimations; ++i)
		{
			const tchar* name = pModel->animationName(i);
			if (name)
			{
				sprintf(nameToInsert, "%01i: %s", i, name);
			}
			else
			{
				sprintf(nameToInsert, "%01i: <unnamed>", i);
			}
			m_AnimationList.AddString(nameToInsert);
		}

		m_AnimationList.SetCurSel(0);
		OnSelchangeAnimationList();

	}
}

void CAnimationBrowser::OnClose() 
{
	release();
	CDialog::OnClose();
}

void CAnimationBrowser::OnDestroy() 
{
	release();
	CDialog::OnDestroy();
}

void CAnimationBrowser::OnPlayStop() 
{
	bool play = m_PlayButton.GetCheck() !=0;
	
	cSubNodeGroup* pGroup = m_pParent->childView()->m_modelNode.subNodeGroupData();

	if (pGroup)
	{
		pGroup->animController().play(play);
	}

}

void CAnimationBrowser::OnSelchangeAnimationList() 
{
	int curSel = m_AnimationList.GetCurSel();
	
	if (curSel != LB_ERR)
	{
/*		cSubNodeGroup* pGroup = m_pParent->childView()->m_modelNode.subNodeGroupData();

		if (pGroup)
		{
			pGroup->animController().playAnimation((uint32)curSel);
		}
*/
	}
}

void CAnimationBrowser::OnDeleteAnim() 
{
	int curSel = m_AnimationList.GetCurSel();
	
	if (curSel != LB_ERR)
	{
		cModelResource* pModelResource = m_pParent->childView()->m_testModel;

		if (pModelResource)
		{
			pModelResource->removeAnimation(curSel);
			rebuild(pModelResource);
		}
	}
	else
	{
		MessageBox("An animation slot must be selected.", "Whuh?");
	}
}


class cDummyAlloc: public ID3DXAllocateHierarchy
{
public:
    STDMETHOD(CreateFrame)(THIS_ LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
	{
		LPD3DXFRAME pNewFrame = new D3DXFRAME;
		memset(pNewFrame, 0, sizeof(D3DXFRAME));
		AllocateName(Name, &pNewFrame->Name);
		*ppNewFrame = pNewFrame;
		return S_OK;
	};

    STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR Name, LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer)
	{
		LPD3DXMESHCONTAINER pNewMeshContainer = new D3DXMESHCONTAINER;
		memset(pNewMeshContainer, 0, sizeof(D3DXMESHCONTAINER));
		*ppNewMeshContainer = pNewMeshContainer;
		return S_OK;
	};

    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree)
	{
		safe_delete_array(pFrameToFree->Name);
		safe_delete(pFrameToFree);
		return S_OK;
	};
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		safe_delete_array(pMeshContainerBase->Name);
		safe_delete(pMeshContainerBase);
		return S_OK;
	};
 
    cDummyAlloc(){};
	~cDummyAlloc(){};
};


void CAnimationBrowser::OnAddAnim() 
{
	int curSel = m_AnimationList.GetCurSel();
	
	if (curSel != LB_ERR)
	{
		cModelResource* pModelResource = m_pParent->childView()->m_testModel;

		if (pModelResource)
		{
			static char szFilter[] = "X Files (*.x)|*.x||";

			CFileDialog fileDialog(TRUE, ".x", NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

			if (fileDialog.DoModal() == IDOK)
			{
				SetCurrentDirectory(TheGameHost.rootPath());
				HRESULT hr;

				// load the file to get the animation data
				LPD3DXFRAME pFrame=0;
				cDummyAlloc Alloc;
				LPD3DXANIMATIONCONTROLLER pAnimMixerToDelete = NULL;

				hr = D3DXLoadMeshHierarchyFromX(
					fileDialog.GetPathName( ), 
					D3DXMESH_SYSTEMMEM, 
					TheGameHost.d3dDevice(), 
					&Alloc, 
					NULL, 
					(LPD3DXFRAME*)&pFrame, 
					&pAnimMixerToDelete);

				if (FAILED(hr))
				{
					MessageBox("Unable To Load File!", "Warning", MB_OK);
					D3DXFrameDestroy(pFrame,&Alloc);
					return;
				}

				if (pAnimMixerToDelete == NULL)
				{
					MessageBox("No animation data in file!", "Warning", MB_OK);
					D3DXFrameDestroy(pFrame,&Alloc);
					return;
				}

				uint32 cAnimSets = pAnimMixerToDelete->GetNumAnimationSets();

				if (!cAnimSets)
				{
					MessageBox("No animation data in file!", "Warning", MB_OK);
					D3DXFrameDestroy(pFrame,&Alloc);
					return;
				}

				//pModelResource->addAnimations(pAnimMixerToDelete);

				
				LPD3DXANIMATIONSET pAnimSet;
				for (uint32 iAnimSet = 0; iAnimSet < cAnimSets; ++iAnimSet)
				{
					pAnimMixerToDelete->GetAnimationSet(iAnimSet, &pAnimSet);
					pModelResource->insertAnimation(curSel, pAnimSet);
					safe_release(pAnimSet);
					++curSel;
				}
				

				safe_release(pAnimMixerToDelete);
				D3DXFrameDestroy(pFrame,&Alloc);
				rebuild(pModelResource);

				// rebuild the animation controller
				cSubNodeGroup* pGroup = m_pParent->childView()->m_modelNode.subNodeGroupData();

				if (pGroup)
				{
					pGroup->animController().recreate();
					m_PlayButton.SetCheck(0);
				}

			}
		}
	}
	else
	{
		MessageBox("An animation slot must be selected.", "Whuh?");
	}
	
}


void CAnimationBrowser::OnAddOverlay() 
{
	int curSel = m_AnimationList.GetCurSel();
	
	if (curSel != LB_ERR)
	{
		CString nameString;
		
		m_AnimationList.GetText(curSel, nameString);

		int id = m_OverlayQueue.AddString(nameString);
		m_OverlayQueue.SetItemData(id, (DWORD)curSel);
	}
}

void CAnimationBrowser::OnAddPrimary() 
{
	int curSel = m_AnimationList.GetCurSel();
	
	if (curSel != LB_ERR)
	{
		CString nameString;
		
		m_AnimationList.GetText(curSel, nameString);

		int id = m_PlayQueue.AddString(nameString);
		m_PlayQueue.SetItemData(id, (DWORD)curSel);
	}
}

void CAnimationBrowser::notifyAnimationFinished(uint32 channel)
{
	if (channel == cAnimationController::k_primaryChannel
		&& m_PlayQueue.GetCount())
	{
		bool looping = m_LoopPrimary.GetCheck() != 0;
		CString name=" ";
		DWORD data=0;

		if (looping)
		{
			m_PlayQueue.GetText(0, name);
			data = m_PlayQueue.GetItemData(0);
		}

		// delete the list entry
		m_PlayQueue.DeleteString(0);
		m_playNextPrimary = true;

		if (looping)
		{
			int newID = m_PlayQueue.AddString(name);
			m_PlayQueue.SetItemData(newID, data);
		}
	}
	else if (channel == cAnimationController::k_overlayChannel
				&& m_OverlayQueue.GetCount())
	{
		bool looping = m_LoopOverlay.GetCheck() != 0;
		CString name=" ";
		DWORD data=0;

		if (looping)
		{
			m_OverlayQueue.GetText(0, name);
			data = m_OverlayQueue.GetItemData(0);
		}

		// delete the list entry
		m_OverlayQueue.DeleteString(0);
		m_playNextOverlay = true;

		if (looping)
		{
			int newID = m_OverlayQueue.AddString(name);
			m_OverlayQueue.SetItemData(newID, data);
		}
	}
}

void CAnimationBrowser::update()
{
	cSubNodeGroup* pGroup = m_pParent->childView()->m_modelNode.subNodeGroupData();

	if (pGroup)
	{
		cAnimationController::sPlaybackSettings settings;


		pGroup->animController().copyDefaultSettings(&settings);
		settings.func = AnimationCallback;
		settings.user_data = (DWORD)this;
	
		float time = pGroup->animController().primaryTime();
		char text[MAX_PATH];
		sprintf(text, "%04f", time);
		SetDlgItemText(IDC_ANIM_TIME, text);

		

		if (m_playNextPrimary)
		{
			int id = m_PlayQueue.GetItemData(0);

			if (id != LB_ERR)
			{
				// make sure this id exists in the larger list box
				int count = m_AnimationList.GetCount();

				if (id < count)
				{
					// play the animation
					pGroup->animController().playAnimation(
						id, 
						cAnimationController::k_primaryChannel, 
						&settings);

					// wait for the callback
					m_playNextPrimary = false;
				}
				else
				{
					m_PlayQueue.DeleteString(0);
				}

			}
		}


		if (m_playNextOverlay)
		{
			int id = m_OverlayQueue.GetItemData(0);

			if (id != LB_ERR)
			{
				// make sure this id exists in the larger list box
				int count = m_AnimationList.GetCount();

				if (id < count)
				{
					// play the animation
					pGroup->animController().playAnimation(
						id, 
						cAnimationController::k_overlayChannel, 
						&settings);

					// wait for the callback
					m_playNextOverlay = false;
				}
				else
				{
					m_OverlayQueue.DeleteString(0);
				}
			}
		}
	}
}

void CAnimationBrowser::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if ((CSliderCtrl *)pScrollBar == &m_Slider)
	{
		cSubNodeGroup* pGroup = m_pParent->childView()->m_modelNode.subNodeGroupData();

		if (pGroup)
		{
			int iPos = m_Slider.GetPos();
			float rPos = iPos/100.0f;

			clamp(rPos, 0.0f, 1.0f);
			
			pGroup->animController().forcePriorityBlend(rPos);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CAnimationBrowser::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// prevent this window from closing in reaction to the enter key being pressed
	if (nID == 1 && (nCode == -1 || nCode == 0)) return false;
//	cString text("nID= %i, nCode= %i\n", nID, nCode);
//	OutputDebugString(text);
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
