/*************************************************************/
/*                         WINDOW.CPP                        */
/*                                                           */
/* Purpose: Implementation for the CWindow class and related */
/*          functions used for window creation (win32)       */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
#include "app.h"
#include "window.h"
#include "resource.h"

#include <gl/gl.h>
#include <gl/glu.h>

const char szTitle[] = "Window";

//-------------------------------------------------------------
//-                   VARIABLES                               -
//-------------------------------------------------------------
//Upper-Left corner
unsigned int CWindow::m_uiX = 0;
unsigned int CWindow::m_uiY = 0;
//Width
unsigned int CWindow::m_uiWidth = 640;
//Height
unsigned int CWindow::m_uiHeight = 480;
//Depth
unsigned int CWindow::m_uiBpp = 16;
//Handles
HINSTANCE CWindow::m_hInstance = 0;
HWND CWindow::m_hWnd = 0;
HDC CWindow::m_hDc = 0;
HGLRC CWindow::m_hGlRc = 0;
WNDCLASS CWindow::m_wc;
//Creation flags
unsigned int CWindow::m_uiFlags = 0;
//font display list
unsigned int CWindow::m_uiFont = 0;

//-------------------------------------------------------------
//- Create
//- Create a window 
//-------------------------------------------------------------
bool CWindow::Create(unsigned int uiX, unsigned int uiY, unsigned int uiWidth, unsigned int uiHeight,
					 unsigned int uiBpp, unsigned int uiFlags)
{
	APP->Log(COLOR_GRAY, "Window creation started");

	RECT Wnd;
	DWORD dwStyle;
	DWORD dwExStyle;
	PIXELFORMATDESCRIPTOR pfd;
	unsigned int uiPixelFmt = 0;

	m_uiFlags = uiFlags;

	if(m_uiFlags != FLAG_CHOICE)
	{
	
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		m_uiBpp = uiBpp;
	}

	m_uiX = uiX;
	m_uiY = uiY;

	Wnd.left = 0;
	Wnd.top = 0;
	Wnd.right = m_uiWidth;
	Wnd.bottom = m_uiHeight;

	m_hInstance = GetModuleHandle(NULL);

	//Set up the WndClass
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hbrBackground = NULL;
	m_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	m_wc.hInstance = m_hInstance;
	m_wc.lpfnWndProc = WndProc;
	m_wc.lpszClassName = szTitle;
	m_wc.lpszMenuName = NULL;
	m_wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	
	if(!RegisterClass(&m_wc))
	{
		APP->Log(COLOR_RED, "Failed to register window class");
		return false;
	}
	
	APP->Log(COLOR_GREEN, "WNDCLASS Registered");

	if(m_uiFlags == FLAG_CHOICE)
	{
		DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_OPTIONS), NULL, DlgProc);
	}

	//Set fullscreen settings
	if(m_uiFlags == FLAG_FULLSCREEN)
	{
		DEVMODE dmScreen;
		memset(&dmScreen, 0, sizeof(DEVMODE));
		dmScreen.dmSize = sizeof(DEVMODE);
		dmScreen.dmPelsWidth = m_uiWidth;
		dmScreen.dmPelsHeight = m_uiHeight;
		dmScreen.dmBitsPerPel = m_uiBpp;
		dmScreen.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreen, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			APP->Log(COLOR_RED, "Fullscreen mode (%ix%ix%i) not supported, using windowed mode ", m_uiWidth, m_uiHeight, m_uiBpp);
			m_uiFlags = FLAG_WINDOWED;
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}
		else
		{
			dwStyle = WS_POPUP;
			dwExStyle = WS_EX_APPWINDOW;
			ShowCursor(FALSE);
		}
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	
	APP->Log(COLOR_GREEN, "Using %ix%ix%i resolution", m_uiWidth, m_uiHeight, m_uiBpp);

	AdjustWindowRectEx(&Wnd, dwStyle, FALSE, dwExStyle);
	
	//Create the window
	if(!(m_hWnd = CreateWindowEx(dwExStyle, szTitle, szTitle, dwStyle ,
		m_uiX, m_uiY, m_uiWidth, m_uiHeight, NULL, NULL, m_hInstance, NULL)))
	{
		APP->Log(COLOR_RED, "Error creating window");
		Destroy();
		return false;
	}
	
	APP->Log(COLOR_GREEN, "Window Created");

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	
	//Set the pixel format
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1, 
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = m_uiBpp;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cStencilBits = 0;
	pfd.cDepthBits = 16;
	pfd.dwVisibleMask = PFD_MAIN_PLANE;

	if(!(m_hDc = GetDC(m_hWnd)))
	{
		APP->Log(COLOR_RED, "Failed to get device context");
		Destroy();
		return false;
	}

	if(!(uiPixelFmt = ChoosePixelFormat(m_hDc, &pfd)))
	{
		APP->Log(COLOR_RED, "Failed to find suitable pixel format");
		Destroy();
		return false;
	}

	if(!SetPixelFormat(m_hDc, uiPixelFmt, &pfd))
	{
		APP->Log(COLOR_RED, "Failed to set pixel format");
		Destroy();
		return false;
	}

	if(!(m_hGlRc = wglCreateContext(m_hDc)))
	{
		APP->Log(COLOR_RED, "Failed to create rendering context");
		Destroy();
		return false;
	}

	if(!wglMakeCurrent(m_hDc, m_hGlRc))
	{
		APP->Log(COLOR_RED, "Failed to activate rendering context");
		Destroy();
		return false;
	}
	
	APP->Log(COLOR_GREEN, "OpenGL set up");

	//Display the window
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	if(m_uiFlags == FLAG_FULLSCREEN)
		Resize(m_uiWidth, m_uiHeight);

	//Standard GL settings
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	APP->Log(COLOR_GRAY, "Window creation complete");

	return true;
}

//-------------------------------------------------------------
//- Resize
//- Change the size of the window
//-------------------------------------------------------------
void CWindow::Resize(unsigned int uiWidth, unsigned int uiHeight)
{
	//prevent divide by 0 error
	if(uiHeight == 0)
		uiHeight = 1;

	//Set the viewport
	glViewport(0, 0, uiWidth, uiHeight);
	//Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f,(float)uiWidth/(float)uiHeight,1.0f,2000.0f);

	glMatrixMode(GL_MODELVIEW);	
}

//-------------------------------------------------------------
//- Destroy
//- Exit and clean up after the window
//-------------------------------------------------------------
void CWindow::Destroy()
{
	APP->Log(COLOR_GRAY, "Shutting Down Window");

	//Change back to windowed mode and restore old settings 
	if(m_uiFlags == FLAG_FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
		APP->Log(COLOR_GREEN, "Fullscreen mode released");
	}
	
	//Release all handles 
	if(m_hGlRc)
	{
		if(!wglMakeCurrent(NULL, NULL))
			APP->Log(COLOR_RED, "Release of DC and RC failed");
		
		if(!wglDeleteContext(m_hGlRc))
			APP->Log(COLOR_RED, "Release of Rendering Context failed");
		else
			APP->Log(COLOR_GREEN, "Released Rendering Context");
	}

	
	if(m_hDc && !ReleaseDC(m_hWnd, m_hDc))
		APP->Log(COLOR_RED, "Release of Device Context failed");
	else
		APP->Log(COLOR_GREEN, "Released Device Context");
	
	//Kill the window
	if(m_hWnd && !DestroyWindow(m_hWnd))
		APP->Log(COLOR_RED, "Release of HWND failed");
	else
		APP->Log(COLOR_GREEN, "Released HWND");

	//unregister the window class
	if(!UnregisterClass(szTitle, m_hInstance))
		APP->Log(COLOR_RED, "Could not unregister WNDCLASS");
	else
		APP->Log(COLOR_GREEN, "Unregistered WNDCLASS");


	m_hGlRc = NULL;
	m_hDc = NULL;
	m_hWnd = NULL;
	m_hInstance = NULL;

	APP->Log(COLOR_GRAY, "Window Shutdown Complete");

	glDeleteLists(m_uiFont, 256);
}

//-------------------------------------------------------------
//- CreateFont
//- Create a bitmap font
//-------------------------------------------------------------
void CWindow::CreateFnt(const char * szFont, int iSize)
{
	HFONT hFont = 0;

	m_uiFont= glGenLists(256);

	hFont= CreateFont(-iSize,
					  0,
					  0,
			 		  0,
					  FW_NORMAL,
					  FALSE,
					  FALSE,
					  FALSE,
					  ANSI_CHARSET,
					  OUT_TT_PRECIS,
					  CLIP_DEFAULT_PRECIS,
					  ANTIALIASED_QUALITY,
					  FF_DONTCARE | DEFAULT_PITCH,
					  szFont);

	SelectObject(m_hDc, hFont);
	wglUseFontBitmaps(m_hDc, 0, 256, m_uiFont);
	DeleteObject(hFont);
}

//-------------------------------------------------------------
//- DlgProc
//- Dialog Proc for options dialog
//-------------------------------------------------------------
BOOL CALLBACK CWindow::DlgProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			//Window coords
			RECT rectWnd;
			GetWindowRect(hWnd, &rectWnd);
			//Center the dialog box
			SetWindowPos(
				hWnd, 
				NULL, 
				GetSystemMetrics(SM_CXSCREEN) / 2 - rectWnd.right  / 2, 
				GetSystemMetrics(SM_CYSCREEN) / 2 - rectWnd.bottom / 2, 
				0, 
				0, 
				SWP_NOSIZE);

			//Set Defaults
			CheckRadioButton(hWnd, IDC_640, IDC_1600, IDC_800);
			CheckRadioButton(hWnd, IDC_16BPP, IDC_32BPP, IDC_16BPP);
			return TRUE;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDRUN:
				{
					//Fullscreen Checked?
					if(IsDlgButtonChecked(hWnd, IDC_FULLSCREEN))
						m_uiFlags = FLAG_FULLSCREEN;
					else
						m_uiFlags = FLAG_WINDOWED;

					//Screen Size
					if(IsDlgButtonChecked(hWnd, IDC_640))
					{
						m_uiWidth = 640;
						m_uiHeight = 480;
					}
					else if(IsDlgButtonChecked(hWnd, IDC_800))
					{
						m_uiWidth = 800;
						m_uiHeight = 600;
					}
					else if(IsDlgButtonChecked(hWnd, IDC_1024))
					{
						m_uiWidth = 1024;
						m_uiHeight = 768;
					}		
					else if(IsDlgButtonChecked(hWnd, IDC_1280))
					{
						m_uiWidth = 1280;
						m_uiHeight = 1024;
					}
					else if(IsDlgButtonChecked(hWnd, IDC_1600))
					{
						m_uiWidth = 1600;
						m_uiHeight = 1200;
					}

					//Screen depth
					if(IsDlgButtonChecked(hWnd, IDC_16BPP))
						m_uiBpp = 16;
					else if(IsDlgButtonChecked(hWnd, IDC_32BPP))
						m_uiBpp = 32;
	
					//Continue
					EndDialog(hWnd, 0);
					return TRUE;

				}
				switch (HIWORD(wParam))
				{
				case BN_CLICKED:
					CheckDlgButton(hWnd, lParam, BST_CHECKED);
					break;
				}
				break;
				
			case IDCANCEL:
				PostQuitMessage(0);
				return TRUE;
			}
			break;
		}
	}
	//Message not handled
	return FALSE;
}

