/*************************************************************/
/*                        SKELETAL1.CPP                      */
/*                                                           */
/* Purpose: File containing function implementations for the */
/*          basic skeletal animation demo.                   */
/*                                                           */
/*      Evan Pipho (Sept 21, 2002)                           */
/*                                                           */
/*************************************************************/
#include <cstdio>
#include "skeletal1.h"
#include "../basecode/app.h"
#include "../basecode/resource.h"
#include <commctrl.h>

#define hCntrlDlg CSkeletalAnimation::m_hDlg
//-------------------------------------------------------------
//- SetupWindow
//- Set up the GL window for this program
//-------------------------------------------------------------
void CSkeletalAnimation::SetupWindow(HWND hWnd)
{
	//Display the control dialog
	m_hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SKELETAL1), hWnd, CSkeletalAnimation::DlgProc);
	if(m_hDlg == 0)
	{
		DWORD dwError = GetLastError();
		APP->Log(COLOR_RED, "Error creating control dialog : ERROR %i", dwError);
		return;
	}
	ShowWindow(m_hDlg, SW_SHOW);
	//Load the bones from file
	FILE * f = fopen("bones.txt", "rt");
	if(!f)
	{
		APP->Log(COLOR_RED, "Could not open bones.txt");
		return;
	}
	//Load the joint information
	for(int x = 0; x < 4; x++)
		fscanf(f, "(%f,%f,%f) %i\n", &m_Joints[x].m_vecOrigPosition[0], &m_Joints[x].m_vecOrigPosition[1], &m_Joints[x].m_vecOrigPosition[2], &m_Joints[x].m_sParent);

	//Load vertex information
	fscanf(f, "%hd\n", &m_sNumVerts);
	m_pVertices = new SVertex[m_sNumVerts];
	for(x = 0; x < m_sNumVerts; x++)
		fscanf(f, "<%f,%f,%f> %i\n", &m_pVertices[x].m_vecPos[0], &m_pVertices[x].m_vecPos[1], &m_pVertices[x].m_vecPos[2], &m_pVertices[x].m_sBone); 

	//Load face information
	fscanf(f, "%hd\n", &m_sNumFaces);
	m_pFaces = new SFace[m_sNumFaces];
	for(x = 0; x < m_sNumFaces; x++)
		fscanf(f, "%hu/%hu/%hu\n", &m_pFaces[x].usIndices[0], &m_pFaces[x].usIndices[1], &m_pFaces[x].usIndices[2]);

	//Close file
	fclose(f);
	g_Skeletal.SetChanged(true);
}

//-------------------------------------------------------------
//- Reset
//- Return bones and joints to their original positions
//-------------------------------------------------------------
void CSkeletalAnimation::Reset()
{
	SJoint * pJoint = 0;
	//Set rotations and quaternions back to the originals
	for(int x = 0; x < 4; x++)
	{
		pJoint = &m_Joints[x];
		pJoint->m_vecRotation = CVector3(0.0f,0.0f,0.0f);
		pJoint->m_vecTranslation = CVector3(0.0f,0.0f,0.0f);
		pJoint->m_matLocal.Identity();
		pJoint->m_matCurrent.Identity();
		pJoint->m_matLocal.SetRotation(0.0f, 0.0f,0.0f);
		pJoint->m_matLocal.SetTranslation(0.0f, 0.0f, 0.0f);
	}
	//Build matrices and quaternions
	for(x = 0; x < 4; x++)
	{
		pJoint = &m_Joints[x];
		pJoint->m_qCurrent.FromEulers(0,0,0);
	}
	//Update edit boxes
	char cBuffer[32];
	sprintf(cBuffer, "%2.3f", 0.0f);
	SetDlgItemText(hCntrlDlg, IDC_ROTX, cBuffer);
	sprintf(cBuffer, "%2.3f", 0.0f);
	SetDlgItemText(hCntrlDlg, IDC_ROTY, cBuffer);
	sprintf(cBuffer, "%2.3f", 0.0f);
	SetDlgItemText(hCntrlDlg, IDC_ROTZ, cBuffer);
	sprintf(cBuffer, "%2.3f", 0.0f);
	SetDlgItemText(hCntrlDlg, IDC_TRANSX, cBuffer);
	sprintf(cBuffer, "%2.3f", 0.0f);
	SetDlgItemText(hCntrlDlg, IDC_TRANSY, cBuffer);
	sprintf(cBuffer, "%2.3f", 0.0f);
	SetDlgItemText(hCntrlDlg, IDC_TRANSZ, cBuffer);

	SetChanged(true);
}

//-------------------------------------------------------------
//- Update
//- Update and draw the bones on the screen
//-------------------------------------------------------------
void CSkeletalAnimation::Update()
{
	static float fRot = 0.0f;
	
	if(KeyPress(VK_RIGHT))
		fRot += 1.0f;
	if(KeyPress(VK_LEFT))
		fRot -= 1.0f;

	SJoint * pJoint = 0;
	//Transform to new position
	for(int x = 0; x < 4; x++)
	{
		pJoint = &m_Joints[x];
		pJoint->m_matLocal.SetRotation(pJoint->m_vecRotation.Get());
		pJoint->m_matLocal.SetTranslation(pJoint->m_vecTranslation.Get());
	}
	for(x = 0; x < 4; x++)
	{
		pJoint = &m_Joints[x];
		if(pJoint->m_sParent == -1)
			pJoint->m_matCurrent = pJoint->m_matLocal;
		else
			pJoint->m_matCurrent = pJoint->m_matLocal * m_Joints[pJoint->m_sParent].m_matCurrent;
		pJoint->m_qCurrent.FromEulers(pJoint->m_vecRotation.Get());
	}

	glLoadIdentity();
	glTranslatef(5.0f,0.0f,-45.f);
	glRotatef(fRot, 0.0f, 1.0f, 0.0f);
	glPointSize(5);

	//draw joints
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(x = 0; x < 4; x++)
	{
		m_Joints[x].m_vecPos = m_Joints[x].m_vecOrigPosition;
		m_Joints[x].m_vecPos.Transform4(m_Joints[x].m_matCurrent);
		glVertex3fv(m_Joints[x].m_vecPos.Get());
	}
	glEnd();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	//draw bones
	glBegin(GL_LINES);
	for(x = 0; x < 4; x++)
	{
		glVertex3fv(m_Joints[x].m_vecPos.Get());
		if(m_Joints[x].m_sParent != -1)
			glVertex3fv(m_Joints[m_Joints[x].m_sParent].m_vecPos.Get());
		else
			glVertex3fv(m_Joints[x].m_vecPos.Get());
	}
	glEnd();

	//Draw the mesh
	glColor3f(1.0f, 1.0f, 1.0f);
	//Transform the vertex
	CVector3 vecPos;
	//Set wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Display the faces, transforming each vertex as needed
	glBegin(GL_TRIANGLES);
	for(x = 0; x < m_sNumFaces; x++)
	{
		//3 vertices for each triangle
		for(int y = 0; y < 3; y++)
		{
			SVertex * pVert = &m_pVertices[m_pFaces[x].usIndices[y]];
			vecPos = pVert->m_vecPos;
			vecPos.Transform4(m_Joints[pVert->m_sBone].m_matCurrent);
			glVertex3fv(vecPos.Get());
		}
	}
	glEnd();

	//Print Axis
	glBegin(GL_LINES);
	//X axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -17.0f, 0.0f);
	glVertex3f(5.0f, -17.0f, 0.0f);
	//Y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -17.0f, 0.0f);
	glVertex3f(0.0f, -13.0f, 0.0f);
	//Z axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, -17.0f, 0.0f);
	glVertex3f(0.0f, -17.0f, -4.0f);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	APP->Print(-0.1f, -0.75f, "X Axis");
	glColor3f(0.0f, 1.0f, 0.0f);
	APP->Print(-0.1f, -0.85f, "Y Axis");
	glColor3f(0.0f, 0.0f, 1.0f);
	APP->Print(-0.1f, -0.95f, "Z Axis");

	//Print joint positions
	glColor3f(1.0f, 1.0f, 1.0f);
	for(x = 0; x < 4; x++)
		APP->Print(.5f, .7f - 0.1f * x, "Joint %i: (%3.2f %3.2f %3.2f)", x, m_Joints[x].m_vecPos[0], m_Joints[x].m_vecPos[1], m_Joints[x].m_vecPos[2]);

	//Update text if it changed
	if(m_bChanged)
	{
		if(IsDlgButtonChecked(hCntrlDlg, IDC_JOINT0))
			SetDlgText(0);
		if(IsDlgButtonChecked(hCntrlDlg, IDC_JOINT1))
			SetDlgText(1);
		if(IsDlgButtonChecked(hCntrlDlg, IDC_JOINT2))
			SetDlgText(2);
		if(IsDlgButtonChecked(hCntrlDlg, IDC_JOINT3))
			SetDlgText(3);
	}
	SetChanged(false);
}

//-------------------------------------------------------------
//- SetDlgText
//- Sets the read-only edit box parts of the control dialog box
//-------------------------------------------------------------
void CSkeletalAnimation::SetDlgText(unsigned int uiJoint)
{
	SJoint * pJoint = &m_Joints[uiJoint];
	char cBuffer[32];
	//Set Quaternion values
	sprintf(cBuffer, "%2.3f", pJoint->m_qCurrent[0]);
	SetDlgItemText(hCntrlDlg, IDC_Q0, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_qCurrent[1]);
	SetDlgItemText(hCntrlDlg, IDC_Q1, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_qCurrent[2]);
	SetDlgItemText(hCntrlDlg, IDC_Q2, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_qCurrent[3]);
	SetDlgItemText(hCntrlDlg, IDC_Q3, cBuffer);
	//Set matrix values
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[0]);
	SetDlgItemText(hCntrlDlg, IDC_M0, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[1]);
	SetDlgItemText(hCntrlDlg, IDC_M1, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[2]);
	SetDlgItemText(hCntrlDlg, IDC_M2, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[3]);
	SetDlgItemText(hCntrlDlg, IDC_M3, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[4]);
	SetDlgItemText(hCntrlDlg, IDC_M4, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[5]);
	SetDlgItemText(hCntrlDlg, IDC_M5, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[6]);
	SetDlgItemText(hCntrlDlg, IDC_M6, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[7]);
	SetDlgItemText(hCntrlDlg, IDC_M7, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[8]);
	SetDlgItemText(hCntrlDlg, IDC_M8, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[9]);
	SetDlgItemText(hCntrlDlg, IDC_M9, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[10]);
	SetDlgItemText(hCntrlDlg, IDC_M10, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[11]);
	SetDlgItemText(hCntrlDlg, IDC_M11, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[12]);
	SetDlgItemText(hCntrlDlg, IDC_M12, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[13]);
	SetDlgItemText(hCntrlDlg, IDC_M13, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[14]);
	SetDlgItemText(hCntrlDlg, IDC_M14, cBuffer);
	sprintf(cBuffer, "%2.3f", pJoint->m_matCurrent[15]);
	SetDlgItemText(hCntrlDlg, IDC_M15, cBuffer);
	//Parent Joint
	sprintf(cBuffer, "%i", pJoint->m_sParent);
	SetDlgItemText(hCntrlDlg, IDC_PARENT, cBuffer);
}

//-------------------------------------------------------------
//- SetChanged
//- Set a boolean variable if the values have chanegd since lat update
//-------------------------------------------------------------
void CSkeletalAnimation::SetChanged(bool bChanged)
{
	m_bChanged = bChanged;
}

//-------------------------------------------------------------
//- Default Constructor
//-------------------------------------------------------------
CSkeletalAnimation::CSkeletalAnimation()
{
	m_pVertices = 0;
	m_pFaces = 0;
}

CSkeletalAnimation::~CSkeletalAnimation()
{
	if(m_pVertices)
	{
		delete [] m_pVertices;
		m_pVertices = 0;
	}
	if(m_pFaces)
	{
		delete [] m_pFaces;
		m_pFaces = 0;
	}
}



//-------------------------------------------------------------
//- DlgProc
//- Dialog Procedure for the control boxes
//-------------------------------------------------------------
BOOL CALLBACK CSkeletalAnimation::DlgProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	//Temp buffer
	char cBuffer[32];
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			//Set buddy windows for the scrollbars
			//Connect SRX updown to ROTX editbox, SRY for ROTY and so on
			SendMessage(GetDlgItem(hWnd, IDC_SRX), UDM_SETBUDDY, (WPARAM)GetDlgItem(hWnd, IDC_ROTX), 0);
			SendMessage(GetDlgItem(hWnd, IDC_SRY), UDM_SETBUDDY, (WPARAM)GetDlgItem(hWnd, IDC_ROTY), 0);
			SendMessage(GetDlgItem(hWnd, IDC_SRZ), UDM_SETBUDDY, (WPARAM)GetDlgItem(hWnd, IDC_ROTZ), 0);
			SendMessage(GetDlgItem(hWnd, IDC_STX), UDM_SETBUDDY, (WPARAM)GetDlgItem(hWnd, IDC_TRANSX), 0);
			SendMessage(GetDlgItem(hWnd, IDC_STY), UDM_SETBUDDY, (WPARAM)GetDlgItem(hWnd, IDC_TRANSY), 0);
			SendMessage(GetDlgItem(hWnd, IDC_STZ), UDM_SETBUDDY, (WPARAM)GetDlgItem(hWnd, IDC_TRANSZ), 0);
			
			//Set Defaults
			CheckRadioButton(hWnd, IDC_JOINT0, IDC_JOINT3, IDC_JOINT0);
			return TRUE;
		}

	case WM_NOTIFY:
		{
			//Message info
			LPNMHDR pnmh = (LPNMHDR)lParam;
			//Updown control info
			LPNMUPDOWN lpnmud;
			char cBuffer[32];
			float fValue;
			switch(wParam)
			{
				//Rotation
				//From spin control on x rotation
			case IDC_SRX:
				{
					if(pnmh->code == UDN_DELTAPOS)
					{
						lpnmud = (LPNMUPDOWN)lParam;
						GetDlgItemText(g_Skeletal.m_hDlg, IDC_ROTX, cBuffer, 32);
						fValue = (float)atof(cBuffer);
						fValue -= (float)lpnmud->iDelta * 1.0f;
						sprintf(cBuffer, "%2.3f", fValue);
						SetDlgItemText(g_Skeletal.m_hDlg, IDC_ROTX, cBuffer);
						return TRUE;
					}
					break;
				}//End case IDC_SRX
				//From spin control for y rotation
			case IDC_SRY:
				{
					if(pnmh->code == UDN_DELTAPOS)
					{
						lpnmud = (LPNMUPDOWN)lParam;
						GetDlgItemText(g_Skeletal.m_hDlg, IDC_ROTY, cBuffer, 32);
						fValue = (float)atof(cBuffer);
						fValue -= (float)lpnmud->iDelta * 1.0f;
						sprintf(cBuffer, "%2.3f", fValue);
						SetDlgItemText(g_Skeletal.m_hDlg, IDC_ROTY, cBuffer);
						return TRUE;
					}
					break;
				}//End case IDC_SRY
				//From spin control for z axis
			case IDC_SRZ:
				{
					if(pnmh->code == UDN_DELTAPOS)
					{
						lpnmud = (LPNMUPDOWN)lParam;
						GetDlgItemText(g_Skeletal.m_hDlg, IDC_ROTZ, cBuffer, 32);
						fValue = (float)atof(cBuffer);
						fValue -= (float)lpnmud->iDelta * 1.0f;
						sprintf(cBuffer, "%2.3f", fValue);
						SetDlgItemText(g_Skeletal.m_hDlg, IDC_ROTZ, cBuffer);
						return TRUE;
					}
					break;
				} //End case IDC_SRZ
			//X translation
			case IDC_STX:
				{
					if(pnmh->code == UDN_DELTAPOS)
					{
						lpnmud = (LPNMUPDOWN)lParam;
						GetDlgItemText(g_Skeletal.m_hDlg, IDC_TRANSX, cBuffer, 32);
						fValue = (float)atof(cBuffer);
						fValue -= (float)lpnmud->iDelta * 0.1f;
						sprintf(cBuffer, "%2.3f", fValue);
						SetDlgItemText(g_Skeletal.m_hDlg, IDC_TRANSX, cBuffer);
						return TRUE;
					}
					break;
				}//End case IDC_STX
				//Y translation
			case IDC_STY:
				{
					if(pnmh->code == UDN_DELTAPOS)
					{
						lpnmud = (LPNMUPDOWN)lParam;
						GetDlgItemText(g_Skeletal.m_hDlg, IDC_TRANSY, cBuffer, 32);
						fValue = (float)atof(cBuffer);
						fValue -= (float)lpnmud->iDelta * 0.1f;
						sprintf(cBuffer, "%2.3f", fValue);
						SetDlgItemText(g_Skeletal.m_hDlg, IDC_TRANSY, cBuffer);
						return TRUE;
					}
					break;
				} //End case IDC_STY
				//Translation on Z
			case IDC_STZ:
				{
					if(pnmh->code == UDN_DELTAPOS)
					{
						lpnmud = (LPNMUPDOWN)lParam;
						GetDlgItemText(g_Skeletal.m_hDlg, IDC_TRANSZ, cBuffer, 32);
						fValue = (float)atof(cBuffer);
						fValue -= (float)lpnmud->iDelta * 0.1f;
						sprintf(cBuffer, "%2.3f", fValue);
						SetDlgItemText(g_Skeletal.m_hDlg, IDC_TRANSZ, cBuffer);
						return TRUE;
					}
					break;
				}//End case IDC_STZ
			}
			break;
		}//End case WM_NOTIFY
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_RESET:
				{
					//Reset all bones
					g_Skeletal.Reset();
					return TRUE;
				}
				
			}
			switch(HIWORD(wParam))
			{
				//A checkbox or radio button has been clicked
			case BN_CLICKED:
				{
					switch(LOWORD(wParam))
					{
						//Joint0 button clicked
					case IDC_JOINT0:
						{
							//Set the edit boxes to the current values
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[0].m_vecRotation[0]);
							SetDlgItemText(hWnd, IDC_ROTX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[0].m_vecRotation[1]);
							SetDlgItemText(hWnd, IDC_ROTY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[0].m_vecRotation[2]);
							SetDlgItemText(hWnd, IDC_ROTZ, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[0].m_vecTranslation[0]);
							SetDlgItemText(hWnd, IDC_TRANSX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[0].m_vecTranslation[1]);
							SetDlgItemText(hWnd, IDC_TRANSY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[0].m_vecTranslation[2]);
							SetDlgItemText(hWnd, IDC_TRANSZ, cBuffer); 
							return TRUE;
						}
						//Joint1 checked
					case IDC_JOINT1:
						{
							//Set the edit boxes to the current values
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[1].m_vecRotation[0]);
							SetDlgItemText(hWnd, IDC_ROTX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[1].m_vecRotation[1]);
							SetDlgItemText(hWnd, IDC_ROTY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[1].m_vecRotation[2]);
							SetDlgItemText(hWnd, IDC_ROTZ, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[1].m_vecTranslation[0]);
							SetDlgItemText(hWnd, IDC_TRANSX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[1].m_vecTranslation[1]);
							SetDlgItemText(hWnd, IDC_TRANSY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[1].m_vecTranslation[2]);
							SetDlgItemText(hWnd, IDC_TRANSZ, cBuffer); 
							return TRUE;
						}
						//Joint2 checked
					case IDC_JOINT2:
						{
							//Set the edit boxes to the current values
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[2].m_vecRotation[0]);
							SetDlgItemText(hWnd, IDC_ROTX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[2].m_vecRotation[1]);
							SetDlgItemText(hWnd, IDC_ROTY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[2].m_vecRotation[2]);
							SetDlgItemText(hWnd, IDC_ROTZ, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[2].m_vecTranslation[0]);
							SetDlgItemText(hWnd, IDC_TRANSX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[2].m_vecTranslation[1]);
							SetDlgItemText(hWnd, IDC_TRANSY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[2].m_vecTranslation[2]);
							SetDlgItemText(hWnd, IDC_TRANSZ, cBuffer); 
							return TRUE;
						}
						//Joint3 checked
					case IDC_JOINT3:
						{
							//Set the edit boxes to the current values
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[3].m_vecRotation[0]);
							SetDlgItemText(hWnd, IDC_ROTX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[3].m_vecRotation[1]);
							SetDlgItemText(hWnd, IDC_ROTY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[3].m_vecRotation[2]);
							SetDlgItemText(hWnd, IDC_ROTZ, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[3].m_vecTranslation[0]);
							SetDlgItemText(hWnd, IDC_TRANSX, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[3].m_vecTranslation[1]);
							SetDlgItemText(hWnd, IDC_TRANSY, cBuffer); 
							sprintf(cBuffer, "%2.3f", g_Skeletal.m_Joints[3].m_vecTranslation[2]);
							SetDlgItemText(hWnd, IDC_TRANSZ, cBuffer); 
							return TRUE;
						}
					}//End switch(LOWORD(wParam))
					break;
				}//End case BN_CLICKED
				//and edit box has changed
			case EN_CHANGE:
				{
					//which edit box?
					switch(LOWORD(wParam))
					{
						//Rotation around X
					case IDC_ROTX:
						{
							//Get the text from the box
							GetDlgItemText(hWnd, IDC_ROTX, cBuffer, 32);
							//Which joint is currently being modified?
							//Joint0 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT0))
								g_Skeletal.m_Joints[0].m_vecRotation[0] = Deg2Rad((float)atof(cBuffer));
							//Joint1 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT1))
								g_Skeletal.m_Joints[1].m_vecRotation[0] = Deg2Rad((float)atof(cBuffer));
							//Joint2 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT2))
								g_Skeletal.m_Joints[2].m_vecRotation[0] = Deg2Rad((float)atof(cBuffer));
							//Joint3 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT3))
								g_Skeletal.m_Joints[3].m_vecRotation[0] = Deg2Rad((float)atof(cBuffer));
						
							g_Skeletal.SetChanged(true);
							return TRUE; //Message handled

						}//End case IDC_ROTX
						//Rotation around Y
					case IDC_ROTY:
						{
							//Get the text from the box
							GetDlgItemText(hWnd, IDC_ROTY, cBuffer, 32);
							//Which joint is currently being modified?
							//Joint0 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT0))
								g_Skeletal.m_Joints[0].m_vecRotation[1] = Deg2Rad((float)atof(cBuffer));
							//Joint1 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT1))
								g_Skeletal.m_Joints[1].m_vecRotation[1] = Deg2Rad((float)atof(cBuffer));
							//Joint2 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT2))
								g_Skeletal.m_Joints[2].m_vecRotation[1] = Deg2Rad((float)atof(cBuffer));
							//Joint3 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT3))
								g_Skeletal.m_Joints[3].m_vecRotation[1] = Deg2Rad((float)atof(cBuffer));
							
							g_Skeletal.SetChanged(true);
							return TRUE; //Message handled

						}//End case IDC_ROTY
						//Rotation around Z
					case IDC_ROTZ:
						{
							//Get the text from the box
							GetDlgItemText(hWnd, IDC_ROTZ, cBuffer, 32);
							//Which joint is currently being modified?
							//Joint0 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT0))
								g_Skeletal.m_Joints[0].m_vecRotation[2] = Deg2Rad((float)atof(cBuffer));
							//Joint1 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT1))
								g_Skeletal.m_Joints[1].m_vecRotation[2] = Deg2Rad((float)atof(cBuffer));
							//Joint2 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT2))
								g_Skeletal.m_Joints[2].m_vecRotation[2] = Deg2Rad((float)atof(cBuffer));
							//Joint3 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT3))
								g_Skeletal.m_Joints[3].m_vecRotation[2] = Deg2Rad((float)atof(cBuffer));
						
							g_Skeletal.SetChanged(true);
							return TRUE; //Message handled

						}//End case IDC_ROTZ

						//X Translation
					case IDC_TRANSX:
						{
							//Get the text from the box
							GetDlgItemText(hWnd, IDC_TRANSX, cBuffer, 32);
							//Which joint is currently being modified?
							//Joint0 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT0))
								g_Skeletal.m_Joints[0].m_vecTranslation[0] = (float)atof(cBuffer);
							//Joint1 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT1))
								g_Skeletal.m_Joints[1].m_vecTranslation[0] = (float)atof(cBuffer);
							//Joint2 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT2))
								g_Skeletal.m_Joints[2].m_vecTranslation[0] = (float)atof(cBuffer);
							//Joint3 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT3))
								g_Skeletal.m_Joints[3].m_vecTranslation[0] = (float)atof(cBuffer);
							
							g_Skeletal.SetChanged(true);
							return TRUE; //Message handled

						}//End case IDC_TRANSX
						//Translation on Y
					case IDC_TRANSY:
						{
							//Get the text from the box
							GetDlgItemText(hWnd, IDC_TRANSY, cBuffer, 32);
							//Which joint is currently being modified?
							//Joint0 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT0))
								g_Skeletal.m_Joints[0].m_vecTranslation[1] = (float)atof(cBuffer);
							//Joint1 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT1))
								g_Skeletal.m_Joints[1].m_vecTranslation[1] = (float)atof(cBuffer);
							//Joint2 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT2))
								g_Skeletal.m_Joints[2].m_vecTranslation[1] = (float)atof(cBuffer);
							//Joint3 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT3))
								g_Skeletal.m_Joints[3].m_vecTranslation[1] = (float)atof(cBuffer);
						
							g_Skeletal.SetChanged(true);
							return TRUE; //Message handled

						}//End case IDC_TRANSY
						//Translation on Z
					case IDC_TRANSZ:
						{
							//Get the text from the box
							GetDlgItemText(hWnd, IDC_TRANSZ, cBuffer, 32);
							//Which joint is currently being modified?
							//Joint0 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT0))
								g_Skeletal.m_Joints[0].m_vecTranslation[2] = (float)atof(cBuffer);
							//Joint1 is activated
							if(IsDlgButtonChecked(hWnd, IDC_JOINT1))
								g_Skeletal.m_Joints[1].m_vecTranslation[2] = (float)atof(cBuffer);
							//Joint2 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT2))
								g_Skeletal.m_Joints[2].m_vecTranslation[2] = (float)atof(cBuffer);
							//Joint3 is active
							if(IsDlgButtonChecked(hWnd, IDC_JOINT3))
								g_Skeletal.m_Joints[3].m_vecTranslation[2] = (float)atof(cBuffer);
						
							g_Skeletal.SetChanged(true);
							return TRUE; //Message handled

						}//End case IDC_TRANSZ
					} //End switch(LOWORD(wParam))
					break;
				} //End case EN_CHANGE:
			}//End switch(HIWORD(wParam))
			break;
		}//End case WM_COMMAND
	}//End switch(uiMsg)

	//Message not handled
	return FALSE;
}
