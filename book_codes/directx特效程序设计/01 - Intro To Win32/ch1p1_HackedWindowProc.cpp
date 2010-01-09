/*
#############################################################################

  Ch1p1_HackedWindowProc.cpp: illustrates what happens if you just create a 
  blank windowproc.

#############################################################################
*/

// Include Directives ///////////////////////////////////////////////////////

#include <windows.h> // we need this header for all windows programs

/****************************************************************************

 WindowProc: our hacked WindowProc function.  We don't actually do anything
 here, we just always return 1 to indicate success.

 ****************************************************************************/
LRESULT CALLBACK WindowProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
  return(1);
}
 
/****************************************************************************
 
 InitWindow: creates a window class and a window.

 ****************************************************************************/
HWND InitWindow(HINSTANCE hinst, int nCmdShow)
{
  HWND                hwnd; // the window handle we obtain will be put here
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
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "MyCoolWindow";
  RegisterClassEx(&wc);
  
  // create a window that's 200 pixels wide, 100 tall
  hwnd = CreateWindowEx(0, "MyCoolWindow", "My First Window",
	    WS_POPUP, 50, 50, 200, 100, NULL, NULL, hinst, NULL);
  
  if (!hwnd) {
    ::MessageBox(NULL, "CreateWindow failed!", "Ch1p1_HackedWindowProc", MB_ICONSTOP);
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

  // exit!
  return 0;
}



