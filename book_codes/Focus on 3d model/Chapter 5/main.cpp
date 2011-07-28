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
#include "skeletal1.h"

CSkeletalAnimation g_Skeletal;

//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
void Main()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	static bool bHelp = true;
	//Turn help on and off
	if(KeyPressOnce('H'))
		bHelp = !bHelp;

	//Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	APP->Print(0.75f, 0.9f, "FPS: %4.2f", APP->GetFPS()); 
	
	//On to the real GL stuff
	glLoadIdentity();
	g_Skeletal.Update();

	//Print help
	APP->Print(-0.93f, -0.8f, "Press H to toggle help");

	if(bHelp)
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		APP->Print(-0.3f, 0.9f, "Skeletal01");
		APP->Print(-0.3f, 0.75f, "The demo is meant to show you the basics of");
		APP->Print(-0.3f, 0.68f, "skeletal animation.  You can select a joint");
		APP->Print(-0.3f, 0.61f, "by clicking the radio buttons in the top-left");
		APP->Print(-0.3f, 0.54f, "You can then use the 6 edit boxes to change");
		APP->Print(-0.3f, 0.47f, "the selected joint's rotation and translation");
		APP->Print(-0.3f, 0.40f, "The transformation matrix and rotation");
		APP->Print(-0.3f, 0.33f, "quaternion are shown as well");
		
		glColor3f(0.5f, 1.0f, 0.5f);
		APP->Print(-0.3f, 0.23f, "The joints are numbered in the following way");
		APP->Print(-0.3f, 0.16f, "From the top (when reset):1,0,2,3 with 0 being the root joint");

		glColor3f(0.5f, 0.5f, 1.0f);
		APP->Print(-0.3f, 0.0f, "Controls");
		APP->Print(-0.3f, -0.15f, "Left/Right - Rotate model on the Y axis"); 
	}
}	

//-------------------------------------------------------------
//- Init
//- Initialization code 
//-------------------------------------------------------------
void Init()
{	
	g_Skeletal.SetupWindow(APP->GetHwnd());
}

//-------------------------------------------------------------
//- WinMain
//- Entry point for the application
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	APP->Init(Main);
	APP->CreateWnd(0,0,800,600,32,FLAG_WINDOWED);
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
			SetWindowText(hWnd, "Skeletal Animation Demo");
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
	case WM_MOVE:
		{
			//Move the dialog box along with the main window
			SetWindowPos(g_Skeletal.GetDialogHandle(), NULL, LOWORD(lParam), HIWORD(lParam), 0, 0, SWP_NOSIZE);
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