// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "shader_edit.h"
#include "ChildView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace gaia;

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
:gaia::cGameHost()
,m_testModel(0)
{

	setupWorldQuadTree(
		cRect3d(
		-1000.0f, 1000.0f,
		-1000.0f, 1000.0f,
		-1000.0f, 1000.0f));

}

CChildView::~CChildView()
{
    Cleanup3DEnvironment();
    SAFE_RELEASE( m_pD3D );
    FinalCleanup();
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
    ON_COMMAND(ID_VIEW_RENDER, OnRender)
	ON_COMMAND(IDM_CHANGEDEVICE, OnChangedevice)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(IDM_OPENFILE, OnOpenfile)
	ON_COMMAND(IDM_SINGLESTEP, OnSinglestep)
	ON_COMMAND(IDM_TOGGLEFULLSCREEN, OnTogglefullscreen)
	ON_COMMAND(IDM_TOGGLESTART, OnTogglestart)
	ON_COMMAND(IDM_SAVE_AS, OnSaveAs)
	ON_COMMAND(IDM_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	setNameStrings(0,0);
	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

//-----------------------------------------------------------------------------
// Name: FullScreenWndProc()
// Desc: The WndProc funtion used when the app is in fullscreen mode. This is
//       needed simply to trap the ESC key.
//-----------------------------------------------------------------------------
LRESULT CALLBACK FullScreenWndProc( HWND hWnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam )
{
    static CChildView* s_pRenderView = NULL;

    if( msg == WM_CREATE )
    {
        s_pRenderView = (CChildView*)((CREATESTRUCT*)lParam)->lpCreateParams;
    }
    else if( msg == WM_CLOSE )
    {
        // User wants to exit, so go back to windowed mode and exit for real
        s_pRenderView->OnToggleFullscreen();
        s_pRenderView->GetActiveWindow()->PostMessage( WM_CLOSE, 0, 0 );
        //g_App.GetMainWnd()->PostMessage( WM_CLOSE, 0, 0 );
    }
    else if( msg == WM_SETCURSOR )
    {
        SetCursor( NULL );
    }
    else if( msg == WM_KEYUP && wParam == VK_ESCAPE )
    {
        // User wants to leave fullscreen mode
        s_pRenderView->OnToggleFullscreen();
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Adjusts the window properties for windowed or fullscreen mode
//-----------------------------------------------------------------------------
HRESULT CChildView::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        ::ShowWindow( m_hwndRenderFullScreen, SW_HIDE );
        CD3DApplication::m_hWnd = m_hwndRenderWindow;
    }
    else
    {
        if( ::IsIconic( m_hwndRenderFullScreen ) )
            ::ShowWindow( m_hwndRenderFullScreen, SW_RESTORE );
        ::ShowWindow( m_hwndRenderFullScreen, SW_SHOW );
        CD3DApplication::m_hWnd = m_hwndRenderFullScreen;
    }
    return S_OK;
}

void CChildView::SetupGameHost() 
{
	m_hwndRenderWindow = GetSafeHwnd( );

	// Register a class for a fullscreen window
	WNDCLASS wndClass = { CS_HREDRAW | CS_VREDRAW, FullScreenWndProc, 0, 0, NULL,
						  NULL, NULL, (HBRUSH)GetStockObject(WHITE_BRUSH), NULL,
						  _T("Fullscreen Window") };
	RegisterClass( &wndClass );

	// We create the fullscreen window (not visible) at startup, so it can
	// be the focus window.  The focus window can only be set at CreateDevice
	// time, not in a Reset, so ToggleFullscreen wouldn't work unless we have
	// already set up the fullscreen focus window.
	m_hwndRenderFullScreen = CreateWindow( _T("Fullscreen Window"), NULL,
										   WS_POPUP, CW_USEDEFAULT,
										   CW_USEDEFAULT, 100, 100,
										   GetTopLevelParent()->GetSafeHwnd(), 0L, NULL, this );

	// Note that for the MFC samples, the device window and focus window
	// are not the same.
	CD3DApplication::m_hWnd = m_hwndRenderWindow;
	CD3DApplication::m_hWndFocus = m_hwndRenderFullScreen;
	CD3DApplication::m_d3dEnumeration.AppUsesDepthBuffer = true;
//    m_bShowCursorWhenFullscreen = true;

	// If we haven't initialized the D3D framework, do so now
	if( !m_bActive )
	{
		CRect rc;
		::GetClientRect( m_hwndRenderWindow, &rc );
		if( rc.Width() == 0 || rc.Height() == 0 )
		{
			MoveWindow(0,0,320,240,true);
			MessageBox( TEXT("The render view must be visible.  Please change the window size or splitters and start EffectEdit again."),
				TEXT("EffectEdit") );
		}
		else
		{
			CD3DApplication::Create( AfxGetInstanceHandle() );
		}

		DisplayManager.enableFrameClearing(true);
	}
}

BOOL CChildView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pMainFrame = (CMainFrame*)pParentWnd;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CChildView::loadMesh(const char* filename)
{
	if (m_pMainFrame->treeDialog())
	{
		m_pMainFrame->treeDialog()->killTree();
	}

	m_modelNode.destroy();

	//displayManager().modelPool().releaseResource(m_testModel);
	safe_release(m_testModel);
	m_testModel=0;

	m_testModel = displayManager().modelPool().loadResource(filename);

	m_modelNode.create();
	m_modelNode.attachToParent(&m_rootNode);

	if (m_testModel)
	{
		m_modelNode.createFromResource(m_testModel->resourceHandle());

		m_ArcBall.SetRadius( m_testModel->boundingSphere().radius );

		if (m_pMainFrame->treeDialog())
		{
			m_pMainFrame->treeDialog()->rebuild(m_testModel);
			m_pMainFrame->animDialog()->rebuild(m_testModel);
		}

		m_CameraPos.set(0.0f, 0.0f, -4*m_testModel->boundingSphere().radius);
		activeCamera()->orientation().setRotation(cVector3(0.0f,0.0f, 1.0f), cVector3(0.0f,1.0f,0.0f));

		RECT rcClient;
		GetClientRect(&rcClient);
		float width = (float)(rcClient.right-rcClient.left);
		float height = (float)(rcClient.bottom-rcClient.top);

		FLOAT      fAspect = width / height;
		
		activeCamera()->setProjParams(
			D3DX_PI/4.0f, 
			fAspect, 
			m_testModel->boundingSphere().radius/64.0f, 
			m_testModel->boundingSphere().radius*10.0f);
	}
	else
	{
		MessageBox("Failed to Load X File", "Bad News");
		setNameStrings(0,0);
	}
}

HRESULT CChildView::InitDeviceObjects()
{
	cGameHost::InitDeviceObjects();

    // Setup the arcball parameters
    m_ArcBall.SetWindow( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height, 0.85f );
	m_rootNode.create();
	m_modelNode.create();

//	m_camera.create();
//	m_camera.attachToParent(&m_rootNode);
	m_modelNode.attachToParent(&m_rootNode);

	m_CameraPos.set(0.0f, 0.0f, -10.0f);
	activeCamera()->orientation().setRotation(cVector3(0.0f,0.0f, 1.0f), cVector3(0.0f,1.0f,0.0f));

	return S_OK;
}

HRESULT CChildView::RestoreDeviceObjects()
{
	cGameHost::RestoreDeviceObjects();

	return S_OK;
}

HRESULT CChildView::InvalidateDeviceObjects()
{
	cGameHost::InvalidateDeviceObjects();

	return S_OK;
}

HRESULT CChildView::DeleteDeviceObjects()
{
//	m_camera.destroy();
	m_modelNode.destroy();
	m_rootNode.destroy();

	//displayManager().modelPool().releaseResource(m_testModel);
	safe_release(m_testModel);
	m_testModel=0;
	
	cGameHost::DeleteDeviceObjects();

	return S_OK;
}

HRESULT CChildView::updateScene()
{
	if (m_testModel)
	{

		// are we the focus window?
		if (GetFocus( ) == this)
		{
			const cVector3& cameraUp = activeCamera()->orientation().getUp();

			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) 
			{
				activeCamera()->orientation().addRotation(0.0f, 0.0f, cameraUp.z >0.0f ? 0.10f: -0.10f);
			}
			else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				activeCamera()->orientation().addRotation( 0.0f, 0.0f, cameraUp.z >0.0f ? -0.10f: 0.10f);
			}

			if (GetAsyncKeyState(VK_UP) & 0x8000) 
			{
				activeCamera()->orientation().addRelRotation(0.0f, 0.10f, 0.0f);
			}
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				activeCamera()->orientation().addRelRotation(0.0f, -0.10f, 0.0f);
			}

			if (GetAsyncKeyState(VK_PRIOR) & 0x8000) 
			{
				m_CameraPos += activeCamera()->orientation().getUp() * 0.5f;
				activeCamera()->orientation().setPosition(m_CameraPos);
			}
			else if (GetAsyncKeyState(VK_NEXT) & 0x8000)
			{
				m_CameraPos -= activeCamera()->orientation().getUp() * 0.5f;
				activeCamera()->orientation().setPosition(m_CameraPos);
			}


			if (GetAsyncKeyState('S') & 0x8000) 
			{
				m_CameraPos -= activeCamera()->orientation().getForward() * 0.5f;
				activeCamera()->orientation().setPosition(m_CameraPos);
			}
			else if (GetAsyncKeyState('W') & 0x8000)
			{
				m_CameraPos += activeCamera()->orientation().getForward() * 0.5f;
				activeCamera()->orientation().setPosition(m_CameraPos);
			}

			if (GetAsyncKeyState('A') & 0x8000) 
			{
				m_CameraPos -= activeCamera()->orientation().getRight() * 0.5f;
				activeCamera()->orientation().setPosition(m_CameraPos);
			}
			else if (GetAsyncKeyState('D') & 0x8000)
			{
				m_CameraPos += activeCamera()->orientation().getRight() * 0.5f;
				activeCamera()->orientation().setPosition(m_CameraPos);
			}

			activeCamera()->orientation().setPosition(m_CameraPos);
		}



		// Setup the world, view, and projection matrices
		// For our world matrix, we will just leave it as the identity
		D3DXMatrixTranslation( &m_matWorld, -m_testModel->boundingSphere().center.x,
										  -m_testModel->boundingSphere().center.y,
										  -m_testModel->boundingSphere().center.z );
		D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_ArcBall.GetRotationMatrix() );
		D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_ArcBall.GetTranslationMatrix() );

		D3DXMATRIX* pRoot = (D3DXMATRIX*)m_modelNode.accessLocalMatrix();
		*pRoot = m_matWorld;

		// update the animation browser
		if (m_pMainFrame && m_pMainFrame->animDialog())
		{
			m_pMainFrame->animDialog()->update();
		}

		m_rootNode.recursiveUpdate();
	}

	return cGameHost::updateScene();
}

HRESULT CChildView::renderScene()
{

	if (!m_testModel)
	{
		return D3D_OK;
	}

    // Turn on the zbuffer
    d3dDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
    d3dDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW  );

    // Turn on ambient lighting 
    d3dDevice()->SetRenderState( D3DRS_AMBIENT, 0xffffffff );

	m_rootNode.recursiveRender();

	return S_OK;
}

LRESULT CChildView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    m_ArcBall.HandleMouseMessages( GetSafeHwnd(), message, wParam, lParam );

    return CWnd::WindowProc(message, wParam, lParam);
}

void CChildView::ChangeDevice() 
{
    Pause(true);
    UserSelectNewDevice();
    Pause(false);
}

void CChildView::OnRender() 
{
    if( m_pd3dDevice != NULL )
        Render3DEnvironment();
}

BOOL CChildView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return CWnd ::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CChildView::OnChangedevice() 
{
    Pause(true);
    UserSelectNewDevice();
    Pause(false);
	
}

void CChildView::OnExit() 
{
    // Recieved key/menu command to exit app
    GetTopLevelParent()->SendMessage(WM_CLOSE, 0, 0 );
	
}

void CChildView::OnOpenfile() 
{
	if (m_testModel)
	{
		char warning[MAX_PATH];

		sprintf(warning, "Are you sure you want to load a new file? \n Any unsaved changes to %s will be lost", m_fileTitle);
		if (IDNO == MessageBox(warning, "Really?", MB_ICONQUESTION|MB_YESNO))
		{
			return;
		}
	}
	
	static char szFilter[] = "X Files (*.x)|*.x|All Files (*.*)|*.*||";

	CFileDialog fileDialog(TRUE, "x", NULL, OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON, szFilter, this);

	if (fileDialog.DoModal() == IDOK)
	{
		loadMesh(fileDialog.GetPathName( ));

		setNameStrings(fileDialog.GetFileTitle( ), fileDialog.GetPathName( ));
	}

}

void CChildView::OnSinglestep() 
{
    if( false == m_bFrameMoving )
        DXUtil_Timer( TIMER_ADVANCE );
    else
        DXUtil_Timer( TIMER_STOP );
    m_bFrameMoving = false;
    m_bSingleStep  = true;
}

void CChildView::OnTogglefullscreen() 
{
    Pause( true );
    if( FAILED( ToggleFullscreen() ) )
        DisplayErrorMsg( D3DAPPERR_RESETFAILED, 0 );
    Pause( false );                        
}

void CChildView::OnTogglestart() 
{
    // Toggle frame movement
    m_bFrameMoving = !m_bFrameMoving;
    DXUtil_Timer( m_bFrameMoving ? TIMER_START : TIMER_STOP );
}


void CChildView::OnSaveAs() 
{
	if (!m_testModel)
	{
		return;
	}

	// TODO: Add your command handler code here
	static char szFilter[] = "X Files (*.x)|*.x|All Files (*.*)|*.*||";
	CFileDialog fileDialog(FALSE, "x", m_filePath, OFN_OVERWRITEPROMPT|OFN_NONETWORKBUTTON, szFilter, this);

	if (fileDialog.DoModal() == IDOK)
	{
		if (!m_testModel->saveResource(fileDialog.GetPathName( )))
		{
			MessageBox("Unable To Save File", "Bad News");
		}
		else
		{
			setNameStrings(fileDialog.GetFileTitle( ), fileDialog.GetPathName( ));
		}
	}
	
}

void CChildView::OnSave() 
{
	if (!m_testModel)
	{
		return;
	}

	char warning[MAX_PATH];

	sprintf(warning, "Are you sure you want to overwrite the file \n %s ?", m_filePath);

	if (IDYES == MessageBox(warning, "Really?", MB_ICONQUESTION|MB_YESNO))
	{
		if (!m_testModel->saveResource())
		{
			MessageBox("Unable To Save File", "Bad News");
		}
	}
	
}

void CChildView::setNameStrings(const char* title, const char* path)
{
	if (title)
	{
		lstrcpy(m_fileTitle, title);
	}
	else
	{
		lstrcpy(m_fileTitle, "<no file loaded>");
	}

	if (path)
	{
		lstrcpy(m_filePath, path);
	}
	else
	{
		lstrcpy(m_filePath, "");
	}

}
