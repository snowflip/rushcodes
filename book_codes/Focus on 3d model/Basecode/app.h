/*************************************************************/
/*                          APP.H                            */
/*                                                           */
/* Purpose: Definitions for the CApp class and related       */
/*          functions fpr a singleton app class.             */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#ifndef APP_H
#define APP_H

//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
//Make our code a little more readable
#define APP CApp::Get()
#include "window.h"
#include "log.h"
#include "timer.h"



//-------------------------------------------------------------
//                         CApp                               -
// author: Evan Pipho (evan@codershq.com)                     -
// date  : Jul 08, 2002                                       -
//-------------------------------------------------------------
class CApp
{
public:
	
	//Initialize the app
	void Init(void (*MainLoop)());
	//Create a window for the application
	bool CreateWnd(unsigned int uiX = 0, unsigned int uiY = 0, 
		unsigned int uiWidth = 640, unsigned int uiHeight = 480,
		unsigned int uiBpp = 16, unsigned int uiFlags = FLAG_CHOICE);
	void CreateFnt(const char * szFont, int iSize);
	//Write string to window
	void Print(float fX, float fY, const char * szFmt, ...);
	//Write string to log file
	void Log(const char * szColor, const char * szFmt, ...);
	//Destroy Application
	void Destroy();
	//Get current fps
	float GetFPS(unsigned int uiFrames = 1);
	//Return the window handle
	HWND GetHwnd();
	
	//Run application
	void Run();

	//Get Application Instance
	static CApp * Get();

private:

	//Private constructor to enforce singleton
	CApp();
	~CApp();
	//Static instance of application
	static CApp * m_pApp;

	//font displaylist
	unsigned int m_uiFont;
	
	//Handle window messages
	bool HandleMessages();
	//Mian loop function
	void (*MainLoopFunc)();

	//Application Log
	CLog * m_pLog;
	//Application Window
	CWindow * m_pWindow;
	//Timer for FPS and whatever else we need it for
	CTimer * m_pTimer;
};

//-------------------------------------------------------------
//                        FUNCTIONS                           -
//-------------------------------------------------------------
//-------------------------------------------------------------
//- Get
//- Returns an instance to the singleton class
//-------------------------------------------------------------
inline CApp * CApp::Get()
{
	if(m_pApp == 0)
		m_pApp = new CApp;
	return m_pApp;
}

//-------------------------------------------------------------
//- GetFPS
//- Return the current FPS of the game
//-------------------------------------------------------------
inline float CApp::GetFPS(unsigned int uiFrames)
{
	return m_pTimer->GetFPS(uiFrames);
}

//-------------------------------------------------------------
//- HandleMessages
//- Handles all windows messages for the application
//-------------------------------------------------------------
inline bool CApp::HandleMessages()
{
	MSG msg;
	//These are functions that are needed to process the messages
	//that the window is recieving.
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{ 
	    TranslateMessage(&msg);
	    DispatchMessage(&msg); 

		if(msg.message==WM_QUIT)
			return false;
	}
	return true;
}

//-------------------------------------------------------------
//- GetHwnd
//- Return the window handle
//-------------------------------------------------------------
inline HWND CApp::GetHwnd()
{
	return m_pWindow->m_hWnd;
}

#endif //APP_H