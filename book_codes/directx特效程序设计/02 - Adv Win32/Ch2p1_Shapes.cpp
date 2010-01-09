//对random的用法是出神入化了
//gdi 图元的用法

/*
#############################################################################

  Ch2p1_Shapes.cpp: a program that uses the Windows GDI to draw shapes of
  various outlines and colors.

#############################################################################
*/

// include files ////////////////////////////////////////////////////////////

#include <windows.h> // needed for windows

// global variables /////////////////////////////////////////////////////////

const char *g_strAppName = "Windows GDI Shapes"; // name of our application
int g_iScreenWidth = 640;                        // screen width, in pixels.
int g_iScreenHeight = 480;                       // screen height, in pixels.

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
  // draw 100 random shapes.
  for (int shapecount=0; shapecount < 20; shapecount++) {
    // get some random coordinates.
    RECT randomrect;

    // the top left of this shape can be anywhere.
    randomrect.left = RandomNumber(0,g_iScreenWidth);
    randomrect.top = RandomNumber(0,g_iScreenHeight);

    // the width and height of this shape can be anywhere between 10 and 100 pixels.
    randomrect.right = randomrect.left + RandomNumber(10,100);
    randomrect.bottom = randomrect.top + RandomNumber(10,100);

    // create a brush
    HBRUSH randombrush;

    COLORREF brushcolor = RGB(RandomNumber(0,255),RandomNumber(0,255),RandomNumber(0,255));

    switch(RandomNumber(0,1)) {
      case 0:
        // create a solid brush of a random color.
        randombrush = CreateSolidBrush(brushcolor);
        break;

      case 1:
        {
          // create a hatched brush of a random color and random hatch.
          int style = HS_BDIAGONAL;
          switch(RandomNumber(0,5)) {
            case 0: style = HS_BDIAGONAL;  break;
            case 1: style = HS_CROSS;      break;
            case 2: style = HS_DIAGCROSS;  break;
            case 3: style = HS_FDIAGONAL;  break;
            case 4: style = HS_HORIZONTAL; break;
            case 5: style = HS_VERTICAL;   break;
          }
          randombrush = CreateHatchBrush(style, brushcolor);
        }
        break;
    }

    // create a pen
    COLORREF pencolor = RGB(RandomNumber(0,255),RandomNumber(0,255),RandomNumber(0,255));
  
    int style = PS_SOLID;
    switch(RandomNumber(0,5)) {
      case 0: style = PS_SOLID;      break;
      case 1: style = PS_DASH;       break;
      case 2: style = PS_DOT;        break;
      case 3: style = PS_DASHDOT;    break;
      case 4: style = PS_DASHDOTDOT; break;
      case 5: style = PS_NULL;       break;
    }
    HPEN randompen = CreatePen(style, style == PS_SOLID ? RandomNumber(1,10) : 1, pencolor);
  
    // select our brush
    HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, randombrush);
  
    // select our pen
    HPEN oldpen = (HPEN)SelectObject(hdc, randompen);

    // draw our shape
    switch(RandomNumber(0,1)) {
    case 0: default:
        Rectangle(hdc, randomrect.left, randomrect.top, randomrect.right, randomrect.bottom);
        break;
      case 1:
        Ellipse(hdc, randomrect.left, randomrect.top, randomrect.right, randomrect.bottom);
        break;
    }

    // de-select our brush
    SelectObject(hdc, oldbrush);
  
    // de-select our pen
    SelectObject(hdc, oldpen);
  
    // delete the brush
    DeleteObject(randombrush);
    
    // delete the pen
    DeleteObject(randompen);
  } // for loop

  char buf[] = "These are shapes made using the Windows GDI. Press a key to redraw!";
  TextOut(hdc, 0, 0, buf, strlen(buf));

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