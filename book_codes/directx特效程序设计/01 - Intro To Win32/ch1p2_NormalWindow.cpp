//peekMessage是实时游戏程序所使用的消息处理方式，但不适合于普通windows程序
//当应用peekmessage流程时，由于并不像使用GetMessage时会堵塞程序的运行流程，从而不停的执行while流程
//造成系统的极大开销，在我的机器上能占据50%的cpu时间，即使完全不做什么的代码。
//可见一个最简单的游戏，你想让他在运行时的cpu低于50%是绝对不可能的，除非他使用getMessage方式，这就不符合实时游戏的标准了

/*
#############################################################################

  Ch1p2_NormalWindow.cpp: a program that creates a window that behaves
  normally.

  Includes message pump and DefWindowProc() stuff.

#############################################################################
*/

// Include Directives ///////////////////////////////////////////////////////
#include <windows.h>


/****************************************************************************

 WindowProc: our windowproc function.  Notice how we watch for and act on the
 WM_CLOSE message.

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
  case WM_CLOSE:
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
  wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE); // this can be changed for different colors
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

  hwnd = CreateWindowEx(0, "MyCoolWindow", "My First Window",
	    WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, 
      50, 50, 200, 100, NULL, NULL, hinst, NULL);
  
  if (!hwnd) {
    ::MessageBox(NULL, "CreateWindow failed!", "Ch1_NormalWindow", MB_ICONSTOP);
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
    
	///@brief 这种模式下，会占据大量cpu（不停的执行while循环）
    if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
      int result = GetMessage(&msg, NULL, 0, 0);
      if (!result) return(msg.wParam);
      TranslateMessage(&msg); 
      DispatchMessage(&msg);
    }

	///@brief 这种方式，也是一般windows阻塞消息模式，不会占用额外cpu
	//if (GetMessage(&msg, NULL, 0, 0))
	//{
	//	int result = GetMessage(&msg, NULL, 0, 0);
	//	if (!result) return(msg.wParam);
	//	TranslateMessage(&msg); 
	//	DispatchMessage(&msg);
	//}
  }

  // we'll never get here, but the compiler will complain if we don't
  // have this line.
  return 0;
}



