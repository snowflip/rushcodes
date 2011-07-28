/*************************************************************/
/*                          MAIN.CPP                         */
/*                                                           */
/* Purpose: The place where all the cool stuff goes on.      */
/*          This is the file where all the initlaization and */
/*          main loop etc are stored.                        */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
const char * szTitle = "Md2";
//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
#include <windows.h>
#include <gl/gl.h>
#include "../basecode/app.h"
#include "md2.h"

CMd2 g_MD2;
CImage g_Skin;

//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
int x = 0;
void Main()
{
	static unsigned int uiStartFrame = 0, uiEndFrame = 182;
	static float fAnimSpeed = 16.0f;
	//Input
	if(KeyPressOnce('W') && uiStartFrame < uiEndFrame-1)
		uiStartFrame++;
	if(KeyPressOnce('S') && uiStartFrame > 0)
		uiStartFrame--;
	if(KeyPressOnce('E'))
		uiEndFrame++;
	if(KeyPressOnce('D') && uiEndFrame > uiStartFrame+1)
		uiEndFrame--;
	if(KeyPressOnce('Q') && fAnimSpeed < 50.0f)
		fAnimSpeed += 0.5;
	if(KeyPressOnce('A') && fAnimSpeed > 0.0f)
		fAnimSpeed -= 0.5;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//Text

	APP->Print(.5f, .6f, "Frame Range: %u to %u", uiStartFrame, uiEndFrame);
	APP->Print(.5f, .5f, "Animation Speed: %2.1f", fAnimSpeed);
	glColor3f(1.0f, 1.0f, 0.0f);
	APP->Print(-.85f, .85f, "Controls:");
	APP->Print(-.85f, .75f, "Q/A Increase/Decrease the Animation Speed");
	APP->Print(-.85f, .65f, "W/S Increase/Decrease the starting frame range");
	APP->Print(-.85f, .55f, "E/D Increase/Decrease the ending frame range");

	glColor3f(1.0f, 1.0f, 1.0f);

	APP->Print(.75f, .9f, "FPS: %4.2f", APP->GetFPS()); 

	glLoadIdentity();
	glTranslatef(0, -0.75f, -5);
	static float rot = 0;
	glRotatef(rot, 0, 1, 0);
	rot += 0.1f;
	g_MD2.Animate(fAnimSpeed, uiStartFrame, uiEndFrame, true);


}

//-------------------------------------------------------------
//- Init
//- Initialization code 
//-------------------------------------------------------------
void Init()
{
	g_MD2.Load("hellpig.md2");
	g_Skin.Load("hellpig.bmp");
	g_MD2.SetSkin(g_Skin);
}

//-------------------------------------------------------------
//- WinMain
//- Entry point for the application
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	APP->Init(Main);
	APP->CreateWnd();
	Init();
	APP->CreateFnt("Arial", 16);
	APP->Run();
	APP->Destroy();
	return 0;
}

//-------------------------------------------------------------
//- WndProc
//- Windows Procedure for the window, put here so each app can have a custom one
//-------------------------------------------------------------
LRESULT CALLBACK CWindow::WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_CREATE:
		{
			SetWindowText(hWnd, szTitle);
			break;
		}
	case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
			//Prevent the screensaver from activating
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			}
			break;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_SIZE:
		{
			Resize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				{
				PostQuitMessage(0);
				break;
				}
			}
			return 0;
		}
	}
	return (DefWindowProc(hWnd, uiMsg, wParam, lParam));
}