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
#include "model.h"

typedef float Vertex[3];
typedef int Triangle[3];

Vertex verts[10] = {{-1,1,-1}, {0,1.2,-1}, {1,1,-1}, {-1.5,0,-1}, {-0.5,0,0}, {0.5,0,0}, {1.5,0,-1}, 
{-1,-1,-1}, {0,-1.2,-1}, {1,-1,-1}};
Triangle Tris[10] = {{0,1,4},{1,5,4},{1,2,5},{0,4,3},{2,6,5},{3,4,7},{4,8,7},{4,5,8},{5,9,8},{5,6,9}};
Vertex facenormals[10];
Vertex vertnormals[10];

//-------------------------------------------------------------
//- CalcNorms
//- Calculate the vertex and face normals
//-------------------------------------------------------------
void CalcNorms()
{
	//Calculate face normals first
	for(int x = 0; x < 10; x++)
	{
		CVector3 vTmp = CalcFaceNormal(CVector3(verts[Tris[x][2]]), CVector3(verts[Tris[x][1]]), CVector3(verts[Tris[x][0]]));
		memcpy(facenormals[x], vTmp.Get(), 12);
	}
	//Calculate the vertex normals
	for(x = 0; x < 10; x++)
	{
		int iShared[10];  //Indices of shared faces
		int iNumShared = 0;   //Number of faces that share vertex
		
		//first find out which faces share the vertex
		for(int y = 0; y < 10; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(Tris[y][z] == x)
				{
					iShared[iNumShared] = y;
					iNumShared ++;
				}
			}
		}
		//Calculate a normal by averaging the face normals of the shared faces
		CVector3 finalNorm;
		for(y = 0; y < iNumShared; y++)
		{
			finalNorm += facenormals[iShared[y]];
		}
		finalNorm /= (float)iNumShared;
		memcpy(&vertnormals[x], finalNorm.Get(), 12); 
	}
}

//-------------------------------------------------------------
//- Main
//- Main game loop
//-------------------------------------------------------------
void Main()
{
	static float rot = 0;

	static bool bFaceNormals = true;
	static bool bWireframe = false;
	static bool bDrawNormals = true;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	
	APP->Print(.75f, .9f, "FPS: %4.2f", APP->GetFPS()); 
	APP->Print(-.9f, .9f, "Face Normals are %s", bFaceNormals ? "enabled" : "disabled");
	APP->Print(-.9f, .8f, "Vertex Normals are %s", bFaceNormals ? "disabled" : "enabled");
	APP->Print(-.9f, .7f, "Wireframe is %s", bWireframe ? "enabled" : "disabled");
	APP->Print(-.9f, .5f, "Press N to toggle normal types");
	APP->Print(-.9f, .4f, "Press W to toggle wireframe");
	APP->Print(-.9f, .3f, "Press D to toggle the normal display");

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
	//Draw light
	glColor3f(1.0f, 1.0f, 0.0f);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex3f(0.0f, 3.5f, 0.0f);
	glEnd();

	glEnable(GL_LIGHTING);
	glRotatef(rot, 1.0f, 1.0f, 1.0f);

	//GetInput
	if(KeyPressOnce('N'))
		bFaceNormals = !bFaceNormals;
	if(KeyPressOnce('W'))
		bWireframe = !bWireframe;
	if(KeyPressOnce('D'))
		bDrawNormals = !bDrawNormals;

	//Draw the mesh
	//Draw with one normal per face
	if(bFaceNormals)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLES);
		for(int x = 0; x < 10; x++)
		{
			glNormal3fv( facenormals[x] );
			glVertex3fv( verts[Tris[x][0]] );
			glVertex3fv( verts[Tris[x][1]] );
			glVertex3fv( verts[Tris[x][2]] );
		}
		glEnd();
		
		if(bDrawNormals)
		{
			//Draw the normals
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			for(x = 0; x < 10; x++)
			{
				CVector3 vCenter = (CVector3(verts[Tris[x][0]]) + CVector3(verts[Tris[x][1]]) + CVector3(verts[Tris[x][2]])) / 3;
				CVector3 vEnd = vCenter + CVector3(facenormals[x]);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3fv(vCenter.Get());
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3fv(vEnd.Get());
			}
			glEnd();
		}
	}
	//Use vertex normals
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLES);
		for(int x = 0; x < 10; x++)
		{
			glNormal3fv( vertnormals[Tris[x][0]] );
			glVertex3fv( verts[Tris[x][0]] );
			glNormal3fv( vertnormals[Tris[x][1]] );
			glVertex3fv( verts[Tris[x][1]] );
			glNormal3fv( vertnormals[Tris[x][2]] );
			glVertex3fv( verts[Tris[x][2]] );
		}
		glEnd();
		if(bDrawNormals)
		{
			//Draw the normals
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			for(x = 0; x < 10; x++)
			{
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3fv(verts[x]);
				CVector3 vEnd = CVector3(verts[x]) + CVector3(vertnormals[x]);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3fv(vEnd.Get());
			}
			glEnd();
		}
	}

	
	if(bWireframe)
	{
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		for(int x = 0; x < 10; x++)
		{
			glVertex3fv( verts[Tris[x][0]]);
			glVertex3fv( verts[Tris[x][1]]);
			glVertex3fv( verts[Tris[x][2]]);
		}
		glEnd();
	}

	rot+=0.2f;

}

//-------------------------------------------------------------
//- Init
//- Initialization code 
//-------------------------------------------------------------
void Init()
{
	static float LightAmbient [] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
	static float LightDiffuse [] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float LightPosition [] = { 0, 3.5, 0, 1};
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);			
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);			
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);		
	glEnable(GL_LIGHT1);
	
	//Calculate the normals
	CalcNorms();

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