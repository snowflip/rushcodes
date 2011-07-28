/*************************************************************/
/*                          MAIN.CPP                         */
/*                                                           */
/* Purpose: The place where all the cool stuff goes on.      */
/*          This is the file where all the initlaization and */
/*          main loop etc are stored.                        */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
/* Linear Interpolation Demo                                 */
/* In this demo, a ball moves along a track in accordance    */
/* with time.  It can be sped up or down by pressing the     */
/* up/down arrows                                            */
//-------------------------------------------------------------
//                        INCLUDES                            -
//-------------------------------------------------------------
#include <windows.h>
#include <gl/gl.h>
#include "../basecode/app.h"

//EndPoints
float Point0[] = {-4, -3};
float Point1[] = {4, 3};
//Interpolated Point
float InterpPoint[2];
//Total time
float fTotalTime = 10.0f; //seconds
//A timer so we can interpolate with respect to time
CTimer Timer;
//Total interpolation so far
float fLastInterp;
//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
void Main()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	
	//Print info
	APP->Print(.75f, .9f, "FPS: %4.2f", APP->GetFPS()); 
	APP->Print(.65f, .8f, "(%2.1f, %2.1f)", Point1[0], Point1[1]);
	APP->Print(-.65f, -.8f, "(%2.1f, %2.1f)", Point0[0], Point0[1]);
	
	APP->Print(-.7f, .9f, "Current Time: %f s", fLastInterp * fTotalTime);
	APP->Print(-.7f, .8f, "Total Time: %.2f s", fTotalTime);
	APP->Print(-.7f, .7f, "Current Interpolation Value: %f", fLastInterp);
	APP->Print(-.7f, .6f, "Current Position: (%f, %f)", InterpPoint[0], InterpPoint[1]);
	APP->Print(-.75f, .4f, "Press the up/down arrows to change the total time");

	//Get Input
	if(KeyPressOnce(VK_UP) && fTotalTime < 100.0f)
		fTotalTime += 0.5f;
	if(KeyPressOnce(VK_DOWN) && fTotalTime > 0.5f)
		fTotalTime -= 0.5f;

	glLoadIdentity();
	glPointSize(5);
	glTranslatef(0.0f, 0.0f, -10.0f);
	//Draw the endpoints
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2fv(Point0);
	glVertex2fv(Point1);
	glEnd();
	//Draw the connecting line
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2fv(Point0);
	glVertex2fv(Point1);
	glEnd();

	//Find the interpolation value between frames
	float fInterp = Timer.GetSeconds() / fTotalTime;
	//Add the accumulated interpolation value
	fInterp += fLastInterp;

	//If it is above 1, go back to the beggining
	if(fInterp > 1.0f)
		fInterp = 0.0f;

	//Set the total accumulated interpolation so far, for use next frame.
	fLastInterp = fInterp; 

	//Interpolate the X and Y values using P0 + (P1 - P0) * t 
	InterpPoint[0] = Point0[0] + (Point1[0] - Point0[0]) * fInterp;
	InterpPoint[1] = Point0[1] + (Point1[1] - Point0[1]) * fInterp;
	
	//Draw the point on the screen
	glPointSize(16);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2fv(InterpPoint);
	glEnd();

}

//-------------------------------------------------------------
//- Init
//- Initialization code 
//-------------------------------------------------------------
void Init()
{
	Timer.Init();	
}

//-------------------------------------------------------------
//- WinMain
//- Entry point for the application
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	APP->Init(Main);
	APP->CreateWnd(0, 0, 800, 600, 32, FLAG_WINDOWED);
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