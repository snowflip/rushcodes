/*************************************************************/
/*                          APP.CPP                          */
/*                                                           */
/* Purpose: Implementation of all functions and static vars  */
/*          for the CApp application class                   */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
#include "app.h"

//Static class instance
CApp * CApp::m_pApp = 0;

//-------------------------------------------------------------
//- Init
//- Initialize the application, set up the application log and
//- set the main loop function
//-------------------------------------------------------------
void CApp::Init(void (*MainLoop)())
{
	MainLoopFunc = MainLoop;
	m_pLog = new CLog("log.html");
	m_pTimer = new CTimer;
	m_pTimer->Init();
}

//-------------------------------------------------------------
//- CreateWnd
//- Create a window for the application
//-------------------------------------------------------------
bool CApp::CreateWnd(unsigned int uiX, unsigned int uiY, unsigned int uiWidth, unsigned int uiHeight,
					 unsigned int uiBpp, unsigned int uiFlags)
{
	//allocate memory
	if(!m_pWindow)
		m_pWindow = new CWindow;

	return(m_pWindow->Create(uiX, uiY, uiWidth, uiHeight, uiBpp, uiFlags));
}

//-------------------------------------------------------------
//- CreateFnt
//- Create a bitmap font
//-------------------------------------------------------------
void CApp::CreateFnt(const char * szFont, int iSize)
{
	m_pWindow->CreateFnt(szFont, iSize);
}

//-------------------------------------------------------------
//- Print
//- Prints text on the screen, x and y values go from -1 to 1 with 
//- 0,0 being the very center of the screen.  The values correspond to the
//- bottom-left corner of the whole text string
//-------------------------------------------------------------
void CApp::Print(float fX, float fY, const char * szFmt, ...)
{
	char caFinal[1024];
	va_list va;
	va_start(va, szFmt);
	vsprintf(caFinal, szFmt, va);
	va_end(va);
	m_pWindow->Print(fX, fY, caFinal);
}


//-------------------------------------------------------------
//- Log
//- Write to application log
//-------------------------------------------------------------
void CApp::Log(const char * szColor, const char * szFmt, ...)
{
	char caFinal[1024];
	va_list va;
	va_start(va, szFmt);
	vsprintf(caFinal, szFmt, va);
	va_end(va);
	m_pLog->Write(szColor, caFinal);
}

//-------------------------------------------------------------
//- Destroy
//- Kill and clean up after the application
//-------------------------------------------------------------
void CApp::Destroy()
{
	Log(COLOR_GRAY, "Shutting Down");
	
	if(m_pWindow)
	{
		m_pWindow->Destroy();
		delete m_pWindow;
		m_pWindow = NULL;
	}
	
	if(m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = 0;
	}

	Log(COLOR_GRAY, "Shutdown complete");
	
	delete m_pLog;
}

//-------------------------------------------------------------
//- Run
//- Run the app (main loop, message pump)
//-------------------------------------------------------------
void CApp::Run()
{
	for(;;)
	{
		if(!HandleMessages())
			return;

		MainLoopFunc();
		m_pWindow->BufferSwap();
	}
}

//-------------------------------------------------------------
//- Constructor
//- Default constructor
//-------------------------------------------------------------
CApp::CApp()
{
	m_pApp = 0;
	m_pWindow = 0;
	m_pLog = 0;
	m_pTimer = 0;
}

CApp::~CApp()
{
	if(m_pApp)
		delete m_pApp;
}