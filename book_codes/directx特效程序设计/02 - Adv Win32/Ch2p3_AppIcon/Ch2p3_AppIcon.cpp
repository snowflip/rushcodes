//应用程序图标，鼠标样式指定


/*
#############################################################################

  Ch2p3_AppIcon.cpp: a program that uses the Windows GDI to output text
  using different colors, fonts, and functions.

#############################################################################
*/

// include files ////////////////////////////////////////////////////////////
#include <windows.h> // needed for windows
#include "resource.h"

// global variables /////////////////////////////////////////////////////////

const char *g_strAppName = "Custom App Icon";    // name of our application

/****************************************************************************

 PaintWindow: called by WindowProc when our window needs painting.  Draws
 200 shapes in various pen and fill styles.

 ****************************************************************************/
void PaintWindow(HDC hdc)
{
  int ypos=0; // the text outputs at this vertical coordinate.

  char strText[] = 
    "This window has a custom icon and cursor.  Check it out!";

  TextOut(hdc, 0, 0, strText, strlen(strText));
  
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
  
  // we get the WM_CLOSE message when our close (X) box has been clicked.
  // we need to respond to it to make sure our program stops when its window
  // is closed.

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
  
  ///@brief 设置应用程序图标，显示在资源管理器里
  wc.hIcon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_APP_ICON_BIG));

  ///@brief 应用程序小图标，显示在标题栏
  wc.hIconSm = LoadIcon(hinst, MAKEINTRESOURCE(IDI_APP_ICON_SMALL));

  ///@brief 设置鼠标样式
  wc.hCursor = LoadCursor(hinst, MAKEINTRESOURCE(IDC_SMILE));

	///@brief 默认的类似于灰色的就是workspace颜色
  wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE); // this can be changed for different colors
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "MyCoolWindow";
  RegisterClassEx(&wc);
  
  // create a window
  hwnd = CreateWindowEx(0, "MyCoolWindow", g_strAppName,
	    WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, 
      10, 10, 500, 200, NULL, NULL, hinst, NULL);
  
  if (!hwnd) {
    ::MessageBox(NULL, "CreateWindow failed!", g_strAppName, MB_ICONSTOP);
    exit(-1);
  }
  
  ShowWindow(hwnd, nCmdShow);
  return(hwnd);
} 

///@brief 创建第二个窗口，观察图标的行为
HWND InitWindow2(HINSTANCE hinst, int nCmdShow)
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

	///@brief 默认的类似于灰色的就是workspace颜色
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE); // this can be changed for different colors
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MyCoolWindow2";
	RegisterClassEx(&wc);

	// create a window
	hwnd = CreateWindowEx(0, "MyCoolWindow2", g_strAppName,
		WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, 
		10, 10, 500, 200, NULL, NULL, hinst, NULL);

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
	InitWindow2(hInstance, nCmdShow);

	//HWND hwnd = InitWindow2(hInstance, nCmdShow);

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