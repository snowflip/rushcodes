//非模式对话框

/*
#############################################################################

  Ch2p5_ModelessDialog.cpp: a program that demonstrates how to use a modeless
  dialog box.  It prompts for a name, and then draws that name in different 
  colors all over the window when you hit redraw.

#############################################################################
*/

// include files ////////////////////////////////////////////////////////////

#include <windows.h> // needed for windows
#include "resource.h"

// global variables /////////////////////////////////////////////////////////

const char *g_strAppName = "How To Use A Modeless Dialog Box"; // app name
int g_iScreenWidth = 640;                        // screen width, in pixels.
int g_iScreenHeight = 480;                       // screen height, in pixels.
HINSTANCE g_hInst = NULL;                        // instance handle
HWND g_hwndDlg = NULL;                           // handle to modeless dlg
HWND g_hwndMain = NULL;                          // handle to main window

char g_strText[256] = "";  // the text we'll eventually prompt the user for

/****************************************************************************

 RandomNumber: returns a random number between iMin and iMax.

 ****************************************************************************/
int RandomNumber(int iMin, int iMax)
{
  return((rand() % (abs(iMax-iMin)+1))+iMin);
}
/****************************************************************************

 PaintWindow: called by WindowProc when our window needs painting.  Draws
 200 shapes in various pen and fill styles.

 ****************************************************************************/
void PaintWindow(HDC hdc)
{
  int oldbkmode = SetBkMode(hdc, TRANSPARENT);
  // draw 100 random shapes.
  for (int shapecount=0; shapecount < 200; shapecount++) {
    // get some random coordinates.
    
    int iRandomX = RandomNumber(0,g_iScreenWidth);
    int iRandomY = RandomNumber(0,g_iScreenHeight);

    // create a random text color
    COLORREF textcolor = RGB(RandomNumber(0,255),RandomNumber(0,255),RandomNumber(0,255));
    COLORREF oldtextcolor = SetTextColor(hdc, textcolor);

    TextOut(hdc, iRandomX, iRandomY, g_strText, strlen(g_strText));

    SetTextColor(hdc, oldtextcolor);

  } // for loop

  char buf[] = "This is a modeless dialog box example.";
  TextOut(hdc, 0, 0, buf, strlen(buf));

  SetBkMode(hdc, oldbkmode);

}
/****************************************************************************

 DialogProc: this is the equivalent of WindowProc for our dialog box.
 
 ****************************************************************************/
BOOL CALLBACK DialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
)
{
  switch(uMsg) {

  case WM_INITDIALOG:
    // set focus to text box.  This will put our cursor in the edit box
    // by default.
    SetFocus(GetDlgItem(hwndDlg, IDC_NAME));
    return(false); // focus was set

  case WM_COMMAND:
    {
      WORD idControl = LOWORD(wParam); // control, identifier 
      HWND hwndCtl = (HWND) lParam;      // handle of control

      switch(idControl) {

      case IDC_CLOSE: // Close button hit!
        // destroy ourselves
        DestroyWindow(g_hwndDlg); g_hwndDlg = NULL;

        // terminate the program.
        PostQuitMessage(0);
        return(true);
        
      case IDC_REDRAW:// the redraw button has been pressed!
        {
          // grab the text from the edit box.
          HWND hwndEdit = GetDlgItem(hwndDlg, IDC_NAME);
          GetWindowText(hwndEdit, g_strText, sizeof(g_strText));

          // tell our main window to redraw itself.
          InvalidateRect(g_hwndMain, NULL, true);
        }
        
        return(true);
        
      }
    }
    break;
  }
  return(0); // did not process message
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

  case WM_CREATE:
    // our window is being created... let's show the modeless dialog.
    g_hwndDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_NAME), hwnd, DialogProc);
    ShowWindow(g_hwndDlg, SW_SHOW);
    break;

  case WM_CHAR:
    // a key has been hit!  repaint!

    // InvalidateRect tells windows that our window contents have become
    // invalid somehow... Windows will turn around and issue a paint
    // message.  We could have just as easily called PaintWindow here,
    // but this is the somewhat more "Windows" way of doing things.
    InvalidateRect(hwnd, NULL, true); 
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
  wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE); // this can be changed for different colors
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "MyCoolWindow";
  RegisterClassEx(&wc);
  
  // create a window that's 640 pixels wide, 480 tall.
  hwnd = CreateWindowEx(0, "MyCoolWindow", g_strAppName,
	    WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, 
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
  // stash our instance handle in the global variable.
  g_hInst = hInstance;
  // create a window
 	g_hwndMain = InitWindow(hInstance, nCmdShow);

  // message pump
  while (1) {
    MSG msg;

    if (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
    {
      int result = GetMessage(&msg, 0, 0, 0);
      if (!result) return(msg.wParam);

      // if the modeless dialog is up, IsDialogMessage will dispatch
      // the message to that dialog.
      if (!IsWindow(g_hwndDlg) || !IsDialogMessage(g_hwndDlg, &msg)) {
        // otherwise, we wind up here, and we dispatch the msg ourselves.
        TranslateMessage(&msg); 
        DispatchMessage(&msg);
      }
    }
  }

  // we'll never get here, but the compiler will complain if we don't
  // have this line.
  return 0;
}