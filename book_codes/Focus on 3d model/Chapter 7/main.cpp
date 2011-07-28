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
#include "3ds.h"

C3ds g_3DS;

//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
void Main()
{
	static float rot = 0;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	
	APP->Print(.75f, .9f, "FPS: %4.2f", APP->GetFPS()); 

	glLoadIdentity();

	glTranslatef(0, -5, -25);
	glRotatef(-90, 1, 0, 0);
	glRotatef(rot, 0, 0, 1);
	g_3DS.Render();


	rot += 0.2f;
}

//-------------------------------------------------------------
//- Init
//- Initialization code 
//-------------------------------------------------------------
void Init()
{
	static float LightAmbient [] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
	static float LightDiffuse [] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float LightPosition [] = { 0, 30, 1000, 1};
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);			
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);			
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);		
	glEnable(GL_LIGHT1);		

	g_3DS.Load("jeep1.3ds");
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
			SetWindowText(hWnd, "3ds");
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