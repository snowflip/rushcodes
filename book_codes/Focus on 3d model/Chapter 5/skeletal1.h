/*************************************************************/
/*                         SKELETAL1.H                       */
/*                                                           */
/* Purpose: Header file for the basic skeletal animation demo*/
/*                                                           */
/*      Evan Pipho (Sept 21, 2002)                           */
/*                                                           */
/*************************************************************/
#ifndef SKELETAL1_H
#define SKELETAL1_H

#include <windows.h>
#include "../math/math.h"

//-------------------------------------------------------------
//- SFace
//- Trangle structure for skeletal animation demo
struct SFace
{
	//Vertex indices
	unsigned short usIndices[3];
};

//-------------------------------------------------------------
//- SVertex
//- Vertex structure for skeletal animation demo
struct SVertex
{
	CVector3 m_vecPos;
	short m_sBone;       //Bone vertex is attached to
};

//-------------------------------------------------------------
//- SJoint
//- Joint for skeletal animation demo
struct SJoint
{
	//Starting translation and rotation
	CVector3 m_vecOrigPosition;
	//parent joint, -1 = no parent
	short m_sParent;
	//Current positions ofthe joint, represented several different ways
	CVector3 m_vecPos;
	CVector3 m_vecRotation;
	CVector3 m_vecTranslation;
	CQuaternion m_qCurrent;
	CMatrix4X4 m_matLocal;
	CMatrix4X4 m_matCurrent;
};

class CSkeletalAnimation
{
public:

	//Adds the controls to the window and sets the viewport up
	//Also loads data
	void SetupWindow(HWND hWnd);
	//Update the model on the screen
	void Update();

	//Return the dialog handle
	HWND GetDialogHandle();

	//Ctor/detor
	CSkeletalAnimation();
	~CSkeletalAnimation();

private:
	
	//4 Joints
	SJoint m_Joints[4];
	//Vertices
	SVertex * m_pVertices;
	//Faces
	SFace * m_pFaces;
	//Number of vertices
	short m_sNumVerts;
	//Number of faces
	short m_sNumFaces;

	//Reset all the joints to the starting position
	void Reset();
	//Set the values of the read-only edit boxes
	void SetDlgText(unsigned int uiJoint);
	//Set if values have changed since last update
	void SetChanged(bool bChanged);
	//Dialog procedure for the control dialog
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//Dialog box handle
	HWND m_hDlg;
	//Data has changed
	bool m_bChanged;
};

//-------------------------------------------------------------
//- GetDialogHandle
//- Return the handle to the control dialong window
//-------------------------------------------------------------
inline HWND CSkeletalAnimation::GetDialogHandle()
{
	return m_hDlg;
}

//Global
extern CSkeletalAnimation g_Skeletal;

#endif