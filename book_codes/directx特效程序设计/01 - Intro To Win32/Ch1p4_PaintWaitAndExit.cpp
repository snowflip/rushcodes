/*
#############################################################################

  Ch1p4_PaintWaitAndExit.cpp: a program that creates a window, paints a 
  message to it, waits for a keystroke, then exits.

#############################################################################
*/

#include <windows.h>
/****************************************************************************
 
 PaintWindow: paints our message in the window.

 ****************************************************************************/
void PaintWindow(HWND hwnd)
{
  PAINTSTRUCT ps;

  BeginPaint(hwnd, &ps);
  HDC hdc = ps.hdc;
  char buf[] = "Press a key to exit.";

  TextOut(hdc, 0, 0, buf, strlen(buf));
  EndPaint(hwnd, &ps);
}

/****************************************************************************
 
 WindowProc: callback function for our window.

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

  case WM_PAINT: // we get this message when we're supposed to paint.
    PaintWindow(hwnd);
    return(0);

  case WM_CHAR: // we get this message when a key is pressed.
    PostQuitMessage(0);
    return(0);

  case WM_CLOSE: // we get this message when our window is closed.
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
  wc.hIcon = NULL;
  wc.hIconSm = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW); // this can be changed for different colors
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "MyCoolWindow";
  RegisterClassEx(&wc);
  
  // create a window that's 200 pixels wide, 100 tall.

  // NOTE:  You can change the window's style slightly by adding or removing WS_ styles.
  // see what happens when you take out WS_MINIMIZEBOX, WS_MAXIMIZEBOX, WS_SYSMENU, and/or
  // WS_THICKFRAME.
  //
  // notice how the window changes?
  //
  // another fun one to play with is WS_EX_TOOLWINDOW, which goes in the 1st parameter, because
  // it's an extended style.

                     // ||
                     // VV WS_EX_TOOLWINDOW goes here!

  hwnd = CreateWindowEx(0, "MyCoolWindow", "Paint, Wait, and Exit v1.00",
	    WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, 
      50, 50, 200, 100, NULL, NULL, hinst, NULL);
  
  if (!hwnd) {
    ::MessageBox(NULL, "CreateWindow failed!", "Paint, Wait, and Exit v1.00", MB_ICONSTOP);
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



