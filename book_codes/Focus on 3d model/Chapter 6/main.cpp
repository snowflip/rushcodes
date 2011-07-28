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
#include "../math/math.h"
#include "ms3d.h"
const char * szTitle = "Ms3d";
CMs3d g_ms3d;
//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
void Main()
{
	static float fRot = 0.0f;
	static float fAnimSpeed = 1.0f;

	//Input
	//Pressing 'b' toggles the display of bones on and off
	if(KeyPressOnce('B'))
		g_ms3d.SwapBoneDraw();
	//Pressing 'w' toggles the display of the meshes
	if(KeyPressOnce('M'))
		g_ms3d.SwapMeshDraw();	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//Pressing left and right arrows rotate the model on the y axis
	if(KeyPress(VK_RIGHT))
		fRot += 0.3f;
	if(KeyPress(VK_LEFT))
		fRot -= 0.3f;
	//Pressing the up and down arrows change the animation speed of the model
	if(KeyPressOnce(VK_UP))
		if(fAnimSpeed < 4.9f)
			fAnimSpeed += 0.1f;
	if(KeyPressOnce(VK_DOWN))
		if(fAnimSpeed > 0.1f)
			fAnimSpeed -= 0.1f;
	//Take Screenshot
	if(KeyPressOnce('P'))
		CImage::ScreenShot();

	//Draw controls and information on the screen
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_LIGHTING);

	//Controls
	APP->Print(0.75f, 0.9f, "FPS: %4.2f", APP->GetFPS()); 
	APP->Print(-0.95f, 0.6f, "Controls");
	APP->Print(-0.95f, 0.5f, "B - Toggle the display of bones");
	APP->Print(-0.95f, 0.4f, "M - Toggle the display of the mesh");
	APP->Print(-0.95f, 0.3f, "Right/Left Arrows - Rotate model on Y axis");
	APP->Print(-0.95f, 0.2f, "Up/Down Arrows - Change animation speed");
	APP->Print(-0.95f, 0.1f, "P - Take Screenshot");
	
	//Info
	APP->Print(0.5f, 0.6f, "Animation Speed: %1.1f", fAnimSpeed);
	APP->Print(0.5f, 0.5f, "Bones: %s", (g_ms3d.AreBonesEnabled() ? "Enabled" : "Disabled"));
	APP->Print(0.5f, 0.4f, "Mesh: %s", (g_ms3d.AreMeshesEnabled() ? "Enabled" : "Disabled"));

	//On to the real GL stuff
	glLoadIdentity();

	glTranslatef(6.0f, -30.0f, -85.0f);
	glRotatef(fRot, 0.0f, 1.0f, 0.0f);
	glEnable(GL_LIGHTING);						

	g_ms3d.Animate(fAnimSpeed, 0, 2, true);
}	

//-------------------------------------------------------------
//- Init
//- Initialization code 
//-------------------------------------------------------------
void Init()
{
	static float LightAmbient [] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
	static float LightDiffuse [] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float LightPosition [] = { 0, 50, 14, 1};
	
	g_ms3d.Load("thug jump.ms3d");

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);			
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);			
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);		
	glEnable(GL_LIGHT1);		
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