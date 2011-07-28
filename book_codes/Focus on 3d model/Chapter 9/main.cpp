/*************************************************************/
/*                          MAIN.CPP                         */
/*                                                           */
/* Purpose: The place where all the cool stuff goes on.      */
/*          This is the file where all the initlaization and */
/*          main loop etc are stored.                        */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
#include <windows.h>
#include <gl/gl.h>
#include "../basecode/app.h"
#include "Md3.h"

const char * szTitle = "Md3_02";

CMd3 g_Md3_1;
CMd3 g_Md3_2;
CMd3 g_Md3_3;
//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
void Main()
{
	static float fRot = 0;
	static unsigned int uiStart1 = 0, uiEnd1 = 0, uiStart2 = 0, uiEnd2 = 0;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	
	APP->Print(.75f, .9f, "FPS: %4.2f", APP->GetFPS()); 
	
	//Get Input
	if(KeyPressOnce('Q') && uiStart1 < uiEnd1)
		uiStart1+=5;
	if(KeyPressOnce('A') && uiStart1 > 0)
		uiStart1-=5;
	if(KeyPressOnce('W'))
		uiEnd1+=5;
	if(KeyPressOnce('S') && uiEnd1 > uiStart1)
		uiEnd1-=5;

	if(KeyPressOnce('E') && uiStart2 < uiEnd2)
		uiStart2+=5;
	if(KeyPressOnce('D') && uiStart2 > 0)
		uiStart2-=5;
	if(KeyPressOnce('R'))
		uiEnd2+=5;
	if(KeyPressOnce('F') && uiEnd2 > uiStart2)
		uiEnd2-=5;


	glLoadIdentity();
	glTranslatef(0, -5, -90);
	glRotatef(-90, 1, 0, 0);

	glRotatef(fRot, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	
	//Attach the torso to the legs
	g_Md3_1.Attach(&g_Md3_2, 0);
	g_Md3_2.Attach(&g_Md3_3, 2);

	g_Md3_1.Animate(0, 200, 10);
	g_Md3_2.Animate(0, 200, 10);
	g_Md3_3.Animate(0, 200, 10);
	g_Md3_1.RenderT();


	glDisable(GL_TEXTURE_2D);

	fRot += 0.2f;
}

//-------------------------------------------------------------
//- Init
//- Initialization code 
//-------------------------------------------------------------
void Init()
{
	g_Md3_1.Load("chicken_lower.md3");
	g_Md3_2.Load("chicken_upper.md3");
	g_Md3_3.Load("chicken_head.md3");
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