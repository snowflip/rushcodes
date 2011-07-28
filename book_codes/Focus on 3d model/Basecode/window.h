/*************************************************************/
/*                          WINDOW.H                         */
/*                                                           */
/* Purpose: Definitions for the CWindow class and related    */
/*          functions used for window creation (win32)       */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#ifndef WINDOW_H
#define WINDOW_H

//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <gl/gl.h>
#include "app.h"

enum EWINDOWFLAGS
{
	FLAG_FULLSCREEN		= 1,
	FLAG_WINDOWED		= 1 << 1,
	FLAG_CHOICE			= 1 << 2
};

//-------------------------------------------------------------
//                        CWindow                             -
// author: Evan Pipho (evan@codershq.com)                     -
// date  : Jul 08, 2002                                       -
//-------------------------------------------------------------
class CWindow
{
	friend class CApp;
	friend class CImage;

public:

	//Create the window
	static bool Create(unsigned int uiX = 0, unsigned int uiY = 0,
		unsigned int uiWidth = 640, unsigned int uiHeight = 480,
		unsigned int uiBpp = 16, unsigned int uiFlags = FLAG_CHOICE);
	//Resize window
	static void Resize(unsigned int uiWidth, unsigned int uiHeight);
	//Destroy window
	static void Destroy();
	
	//Create font
	static void CreateFnt(const char * szFont, int iSize);
	//Print characters on screen
	static void Print(float fX, float fY, const char * szFmt, ...);

	//Swap buffers
	static void BufferSwap();

	//static variables for positions


private:

	//Window Procedure
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//Dimentions
	static unsigned int m_uiX;
	static unsigned int m_uiY;
	static unsigned int m_uiWidth;
	static unsigned int m_uiHeight;
	
	//Properties
	static unsigned int m_uiBpp;

	//handles
	static HWND m_hWnd;
	static HINSTANCE m_hInstance;
	static HDC m_hDc;
	static HGLRC m_hGlRc;

	//Window class
	static WNDCLASS m_wc;

	//font list
	static unsigned int m_uiFont;

	//Window flag
	static unsigned int m_uiFlags;
};

//-------------------------------------------------------------
//                       FUNCTIONS                            -
//-------------------------------------------------------------
//-------------------------------------------------------------
//- Bufferswap
//- Swap the buffers of the HDC
//-------------------------------------------------------------
inline void CWindow::BufferSwap()
{
	SwapBuffers(m_hDc);
}

//-------------------------------------------------------------
//- Print
//- Print characters to screen
//-------------------------------------------------------------
inline void CWindow::Print(float fX, float fY, const char * szFmt, ...)
{
	char caFinal[1024];
	va_list va;
	va_start(va, szFmt);
	vsprintf(caFinal, szFmt, va);
	va_end(va);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2f(fX, fY);
	glListBase(m_uiFont);
	glCallLists(strlen(caFinal), GL_UNSIGNED_BYTE, caFinal);
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
}

//-------------------------------------------------------------
//- KeyPressOnce
//- Check to see if a key has been pressed, only trigger once 
//- per press (no press and hold) 
//-------------------------------------------------------------
inline bool KeyPressOnce(int iKey)
{
	if(GetAsyncKeyState(iKey) == -32767)
		return true;
	return false;
}

//-------------------------------------------------------------
//- KeyPress
//- Check to see if a key is 
//-------------------------------------------------------------
inline bool KeyPress(int iKey)
{
	if(GetAsyncKeyState(iKey) == -32767 || GetAsyncKeyState(iKey) == -32768)
		return true;
	return false;
}

#endif //WINDOW_H