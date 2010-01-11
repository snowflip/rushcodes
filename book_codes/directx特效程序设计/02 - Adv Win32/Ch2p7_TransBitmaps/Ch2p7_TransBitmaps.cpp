/*
#############################################################################

Ch2p7_TransBitmaps.cpp: a program that uses the Windows GDI to draw 
transparent bitmaps.

#############################################################################
*/

// include files ////////////////////////////////////////////////////////////

#include <windows.h> // needed for windows
#include <stdio.h>   // for sprintf

#include "GDIBitmap.h"

// global variables /////////////////////////////////////////////////////////

const char *g_strAppName = "Transparent Windows Bitmaps"; // app name
int g_iScreenWidth = 640;                        // screen width, in pixels.
int g_iScreenHeight = 480;                       // screen height, in pixels.

CGDIBitmap g_bmpBackground;
CGDIBitmap g_bmpSprite;
CGDIBitmap g_bmpSpriteMask;

/****************************************************************************

PaintWindow: called by WindowProc when our window needs painting.  Draws
a bitmap into the window.

****************************************************************************/
void PaintWindow(HDC hdc)
{
	// set our mapping mode as text.  this means 1 logical unit = 1 pixel.
	SetMapMode(hdc, MM_TEXT);

	// this function will draw the bitmap without stretching.
	/*BitBlt(hdc,
	0, 0, g_iScreenWidth, g_iScreenHeight,
	g_hDCBitmap, 0, 0, SRCCOPY);
	*/

	// this function will stretch the bitmap so that it fits in the window.
	StretchBlt(hdc,
		0, 0, g_iScreenWidth, g_iScreenHeight,
		g_bmpBackground.GetBitmapDC(), 
		0, 0, g_bmpBackground.GetWidth(), g_bmpBackground.GetHeight(), 
		SRCCOPY);

}
/****************************************************************************

WindowProc: Windows calls this function when we dispatch messages.

****************************************************************************/
LRESULT CALLBACK WindowProc(
							HWND hwnd,      // handle to window
							UINT uMsg,      // message identifier
							WPARAM wParam,  // first message parameter
							LPARAM lParam   // second message parameter
							)
{
	switch(uMsg) {

  case WM_CREATE:
	  {
		  // our window is being created!  load a bitmap into an off-screen DC.

		  // get a DC for this window
		  HDC hdcWindow = GetDC(hwnd);

		  // let our class handle the details of loading the bitmaps!
		  g_bmpBackground.Load(hdcWindow, "ch2p7_bg.bmp");
		  g_bmpSprite.Load(hdcWindow, "ch2p7_purball.bmp");
		  g_bmpSpriteMask.Load(hdcWindow, "ch2p7_purball_mask.bmp");

		  // release the DC we obtained
		  ReleaseDC(hwnd, hdcWindow);
	  }
	  break;

  case WM_DESTROY:
	  // we don't have to do anything here, because CGDIBitmap's
	  // destructor will take care of deleting the bitmap for us.
	  break;

  case WM_PAINT:
	  {
		  PAINTSTRUCT ps;

		  // get the DC we're supposed to use from Windows
		  BeginPaint(hwnd, &ps);
		  HDC hdc = ps.hdc;

		  // paint our window (function is defined above)
		  PaintWindow(hdc);

		  // tell Windows we're done painting.
		  EndPaint(hwnd, &ps);
	  }
	  // we processed the message, so return 0.
	  return(0);

  case WM_CHAR:
	  {
		  // plot a new purple ball
		  int randomx = RandomNumber(0,g_iScreenWidth);
		  int randomy = RandomNumber(0,g_iScreenHeight);

		  HDC hdc = GetDC(hwnd);

		  // first blit the mask, using SRCAND.
		  BitBlt(hdc, randomx, randomy, g_iScreenWidth, g_iScreenHeight,
			  g_bmpSpriteMask.GetBitmapDC(), 0, 0, SRCAND);

		  // then blit the image using the OR operator (SRCPAINT).
		  BitBlt(hdc, randomx, randomy, g_iScreenWidth, g_iScreenHeight,
			  g_bmpSprite.GetBitmapDC(), 0, 0, SRCPAINT);

		  ReleaseDC(hwnd, hdc);

	  }
	  break;


  case WM_CLOSE: // our close button has been hit!  quit!
	  PostQuitMessage(0);
	  return(0);

  case WM_SIZE: // our windows dimensions have changed

	  // stash the new width and height in the global variables.  PaintWindow
	  // uses these global variables when it draws our window contents.

	  g_iScreenWidth  = LOWORD(lParam);  // new width of client area 
	  g_iScreenHeight = HIWORD(lParam); // new height of client area

	  // break, not return.  We want to fall through this case and let
	  // DefWindowProc handle the specifics of sizing the window itself.
	  break; 
	}

	// all the other messages, we don't care about, so we let DefWindowProc
	// handle them.
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

/****************************************************************************

InitWindow: creates a window class and a window.

****************************************************************************/
HWND InitWindow(HINSTANCE hinst, int nCmdShow)
{
	HWND                hwnd;
	WNDCLASSEX          wc;

	// set up and register window class
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc; // change this to NULL and crash!
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinst;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL; // don't repaint background.  this kills flicker.
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MyCoolWindow";
	RegisterClassEx(&wc);

	// create a window that's 640 pixels wide, 480 tall.
	hwnd = CreateWindowEx(0, "MyCoolWindow", g_strAppName,
		WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, 
		10, 10, g_iScreenWidth, g_iScreenHeight, NULL, NULL, hinst, NULL);

	if (!hwnd) {
		::MessageBox(NULL, "CreateWindow failed!", g_strAppName, MB_ICONSTOP);
		exit(-1);
	}

	ShowWindow(hwnd, nCmdShow);
	return(hwnd);
} 

/****************************************************************************

WinMain: program execution starts here.

****************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	// create a window
	HWND hwnd = InitWindow(hInstance, nCmdShow);

	// message pump
	while (1) {
		MSG msg;
		if (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
		{
			int result = GetMessage(&msg, 0, 0, 0);
			if (!result) return(msg.wParam);

			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	// we'll never get here, but the compiler will complain if we don't
	// have this line.
	return 0;
}