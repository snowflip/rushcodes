//对gdi text的绘制方法深入理解
//绘制方法，字体创建等等

//另外，gdi在绘制字体时，不是你调一个函数就马上绘制的，应该是积累了一定的buffer以后一起绘制，以增加效率

/*
#############################################################################

  Ch2p2_Text.cpp: a program that uses the Windows GDI to output text
  using different colors, fonts, and functions.

  Note that this program is exactly the same as Ch2p1_Shapes, except the
  PaintWindow function is different.  
  
#############################################################################
*/

// include files ////////////////////////////////////////////////////////////

#include <windows.h> // needed for windows
#include <stdio.h>   // for sprintf

// global variables /////////////////////////////////////////////////////////

const char *g_strAppName = "Windows GDI Text";   // name of our application
int g_iScreenWidth = 640;                        // screen width, in pixels.
int g_iScreenHeight = 480;                       // screen height, in pixels.

/****************************************************************************

 DrawTextAndMoveYPos: This is a helper function that PaintWindow calls.
 Basically, given a y position and some text, it uses DrawText to render
 the text (with word-wrapping), and adds the height of the text block
 onto the y position.

  Note that ypos is a reference, so our changes to it are not lost.

 ****************************************************************************/
void DrawTextAndMoveYPos(HDC hdc, int &ypos, char *text)
{
  // there are two steps to using DrawText.  First, we know the width we
  // can use, but we don't know how much vertical space our text block needs
  // for that width.  So we make a call to DrawText and tell it to calculate
  // our rectangle.
  RECT rectDrawText; // the rectangle Windows will draw the text in.

  // populate our rect with everything but the bottom.

  rectDrawText.left = 0;
  rectDrawText.right = g_iScreenWidth;
  rectDrawText.top = ypos;
  
  int textheight=0;

  ///@brief 返回值是所需的高，如果定义了DT_CALCRECT，则实际rectDrawText已经被正确填充了，这里的代码有点重复了
  ///@brief If the DT_CALCRECT flag is specified, the rectangle specified by lpRect will be updated to reflect the width and height needed to draw the text.
  textheight = DrawText(hdc, text, strlen(text), &rectDrawText, 
    DT_CALCRECT | DT_WORDBREAK);

  ///@brief 同上，这里有点重复了
  // extend our window by as much as DrawText says it takes.
  rectDrawText.bottom = rectDrawText.top + textheight;

  // now that we know how much room we need, we can render the text.
  DrawText(hdc, text, strlen(text), &rectDrawText, 
    DT_WORDBREAK);

  ypos += textheight + 3; // move our cursor down and give it 3 pixels
                          // of extra space.
}
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
  int ypos=0; // the text outputs at this vertical coordinate.

  // set our mapping mode as text.  this means 1 logical unit = 1 pixel.
  SetMapMode(hdc, MM_TEXT);

  ///@brief 默认gdi text在画的时候有白色的背景，也就是OPAQUE模式
  // set our text background to transparent.
  int oldbkmode = SetBkMode(hdc, TRANSPARENT);

  char strTextMadeWithGDI[] = 
    "This is text made using the GDI. Press a key to redraw!";

  TextOut(hdc, 0, ypos, strTextMadeWithGDI, strlen(strTextMadeWithGDI));
  
  // move our "cursor" down 20.
  ypos += 20;

  // --------------------------------------------------------------

  char strTextMadeWithDrawText[] =
    "This is a whole bunch of text that really doesn't mean anything in and"
    " of itself, but illustrates how the GDI DrawText function can break"
    " lines apart at word boundries for you.  Size the window to see it!";

  DrawTextAndMoveYPos(hdc, ypos, strTextMadeWithDrawText);
  
  // --------------------------------------------------------------

  // create a 50 pixel tall Times New Roman font!
  HFONT fontTimesNewRoman = CreateFont(
    -25, // height (negative means use character heights
    0, // width (0 means use default)
    0, // escapement (0 means none)
    0, // orientation (0 means none)
    FW_NORMAL, // "boldness" of font
    false, // italic?  true or false
    false, // underline? true or false
    false, // strikeout?  true or false
    ANSI_CHARSET, // desired character set
    OUT_TT_PRECIS, // output precision - use TrueType only
    CLIP_DEFAULT_PRECIS, // clip precision - use default
    PROOF_QUALITY, // proof quality
    DEFAULT_PITCH | FF_DONTCARE, // pitch and family
    "Times New Roman" // name of font face desired
    );

  // select our font
  HFONT oldfont = (HFONT)SelectObject(hdc, fontTimesNewRoman);

  // draw text
  char strTimesNewRomanText[] = "This is text in 25 pixel tall Times New Roman.";

  DrawTextAndMoveYPos(hdc, ypos, strTimesNewRomanText);

  // --------------------------------------------------------------

  int red = RandomNumber(0,255);
  int green = RandomNumber(0,255);
  int blue = RandomNumber(0,255);

  char strColoredText[128];
  sprintf(strColoredText, "This is colored text.  It is colored RGB (%d,%d,%d).",
    red, green, blue);

  ///@brief 设置字体的颜色
  COLORREF oldcolor = SetTextColor(hdc, RGB(red,green,blue));
  
  DrawTextAndMoveYPos(hdc, ypos, strColoredText);

  // --------------------------------------------------------------

  char strGreenTextOnBlue[] = "This is centered green text on a blue background.";
  
  ///@brief 设置字体绘制的背景色
  COLORREF oldbackground = SetBkColor(hdc, RGB(0,0,255));
  SetTextColor(hdc, RGB(0,255,0));

  ///@brief 设置背景模式为不透明，这样背景色就能看到了
  //所以其实gdi 在绘制字体时，默认背景色为白色，并且背景模式为opaque
  SetBkMode(hdc, OPAQUE);
  
  // draw this text centered! (can't use DrawText for this!)

  ///@brief 设置剧中对齐
  // set up alignment
  UINT oldtextalign = SetTextAlign(hdc, TA_CENTER);

  // draw the text
  ExtTextOut(hdc, g_iScreenWidth/2, ypos, 0, NULL, 
    strGreenTextOnBlue, strlen(strGreenTextOnBlue), NULL);
  
  // deselect font object and delete
  SelectObject(hdc, oldfont);
  DeleteObject(fontTimesNewRoman);

  // --------------------------------------------------------------

  // set our text color and background back to whatever it was before.
  SetTextAlign(hdc, oldtextalign);
  SetTextColor(hdc, oldcolor);
  SetBkColor(hdc, oldbackground);
  SetBkMode(hdc, oldbkmode);

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