/*************************************************************/
/*                          MAIN.CPP                         */
/*                                                           */
/* Purpose: The place where all the cool stuff goes on.      */
/*          This is the file where all the initlaization and */
/*          main loop etc are stored.                        */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/

const char * szModelFile = "evh.mdl";
//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
#include <windows.h>
#include <gl/gl.h>
#include "../basecode/app.h"
#include "hl_src\mathlib.h"
#include "hl_src\studio.h"
#include "hl_src\mdlviewer.h"

CTimer g_Timer;  //Timer for animation
StudioModel g_MDL; //Model
//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
void Main()
{
	static float rot = 0;
	static int iCurSeq = 0; //Current sequence
	static float fTimeMult = 1.0f; //Time multiplier (speed)

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	
	APP->Print(.75f, .9f, "FPS: %4.2f", APP->GetFPS()); 
	APP->Print(.55f, .7f, "Current Sequence: %i", iCurSeq);
	APP->Print(.55f, .6f, "Current Speed %2.1f", fTimeMult);
	APP->Print(-.8f, .8f, "Controls");
	APP->Print(-.8f, .7f, "Up/Down arrows - Change speed of animation");
	APP->Print(-.8f, .6f, "Q/W - Cycle through animation sequences");
	//Get Input
	//Next Sequence
	if(KeyPressOnce('Q'))
	{
		int iSeq = g_MDL.GetSequence();
		iCurSeq++;
		//If it is the last sequence
		if(g_MDL.SetSequence(iSeq + iCurSeq) == iSeq)
		{
			//Set sequence to 0
			g_MDL.SetSequence(0);
			iCurSeq = 0;
		}
	}
	//Previous Sequence
	if(KeyPressOnce('W'))
	{
		if(iCurSeq == 0)
			g_MDL.SetSequence(0);
		else
		{
			g_MDL.SetSequence(iCurSeq);
			iCurSeq--;
		}
	}
	//Increase speed
	if(KeyPressOnce(VK_UP))
		fTimeMult += 0.1f;
	//Decrease speed
	if(KeyPressOnce(VK_DOWN) && fTimeMult > 0.01f)
		fTimeMult -= 0.1f;


	glEnable(GL_TEXTURE_2D);
	//Get the elapsed time
	float fSec = g_Timer.GetSeconds() * fTimeMult;
	//Advance the model
	g_MDL.AdvanceFrame(fSec);
	
	glLoadIdentity();	
	glTranslatef(0, 0, -150);
	glRotatef(-90, 1, 0, 0);
	glRotatef(rot, 0, 0, 1);


	g_MDL.DrawModel();

		
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
	
	//Init the timer
	g_Timer.Init();
	//Load the file
	APP->Log(COLOR_GRAY, "Loading %s", szModelFile);
	g_MDL.Init((char *)szModelFile);

	//start with the first animation sequence
	g_MDL.SetSequence(0);
	//Set all bone controllers to 0
	g_MDL.SetController(0, 0.0);
	g_MDL.SetController(1, 0.0);	
	g_MDL.SetController(2, 0.0);	
	g_MDL.SetController(3, 0.0);
	g_MDL.SetMouth(0);
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