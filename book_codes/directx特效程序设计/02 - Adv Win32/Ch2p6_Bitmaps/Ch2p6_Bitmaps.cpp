//bitmap操作
//dc的灵活使用


/*
#############################################################################

Ch2p6_Bitmaps.cpp: a program that uses the Windows GDI to draw bitmaps.

#############################################################################
*/

// include files ////////////////////////////////////////////////////////////

#include <windows.h> // needed for windows
#include <stdio.h>   // for sprintf

// global variables /////////////////////////////////////////////////////////

const char *g_strAppName = "Windows Bitmaps";    // name of our application
int g_iScreenWidth = 640;                        // screen width, in pixels.
int g_iScreenHeight = 480;                       // screen height, in pixels.

HBITMAP g_hBitmap = NULL;    // our bitmap will go here
HBITMAP g_hOldBitmap = NULL; // the old bitmap will go here
HDC     g_hDCBitmap = NULL;  // dc to our bitmap
int     g_iBitmapWidth = 0;  // width of our bitmap
int     g_iBitmapHeight = 0; // height of our bitmap

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

	///@brief hdc是dest dc， g_hDCBitmap是源dc，将源dc的rect范围里的像素copy到目的dc的rect中
	// this function will stretch the bitmap so that it fits in the window.
	StretchBlt(hdc,
		0, 0, g_iScreenWidth, g_iScreenHeight,		//dest rect
		g_hDCBitmap, 0, 0, g_iBitmapWidth,			//src rect
		g_iBitmapHeight, SRCCOPY);

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

		  ///@brief 跟据窗口dc创建一个兼容的dc用于存放bitmap
		  // create a compatible in-memory device context
		  g_hDCBitmap = CreateCompatibleDC(hdcWindow);

		  // then load the image
		  g_hBitmap = (HBITMAP)LoadImage(NULL,"ch2p6_trippy.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

		  // select the image into the memory dc
		  g_hOldBitmap = (HBITMAP)SelectObject(g_hDCBitmap,g_hBitmap);

		  ///@brief BITMAP 接口存放了bitmap的info
		  // get the bitmap's properties
		  BITMAP bmp;
		  GetObject(g_hBitmap,sizeof(BITMAP),(LPVOID)&bmp);

		  // assign width and height
		  g_iBitmapWidth = bmp.bmWidth;
		  g_iBitmapHeight = bmp.bmHeight;

		  // release the DC we obtained
		  ReleaseDC(hwnd, hdcWindow);
	  }
	  break;

  case WM_DESTROY:
	  {
		  // our window is being destroyed!  
		  // now's the time to delete the bitmap we created.

		  // de-select the bitmap from our in-memory DC
		  SelectObject(g_hDCBitmap, g_hOldBitmap);

		  // delete the bitmap
		  DeleteObject(g_hBitmap);

		  // delete our in-memory DC
		  DeleteDC(g_hDCBitmap);
	  }
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
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			int result = GetMessage(&msg, NULL, 0, 0);
			if (!result) return(msg.wParam);

			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	// we'll never get here, but the compiler will complain if we don't
	// have this line.
	return 0;
}