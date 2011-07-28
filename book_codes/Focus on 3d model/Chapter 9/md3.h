/*************************************************************/
/*                           MD3.H                           */
/*                                                           */
/* Purpose: Definitions for CMd3, a class to load, render and*/
/*          animate the Id Software's Quake3 model format    */
/*          http://www.idsoftware.com                        */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#ifndef MD3_H
#define MD3_H

//-------------------------------------------------------------
//                       INCLUDES                             -
//-------------------------------------------------------------
#include "../basecode/image.h"
#include "../basecode/timer.h"
#include "model.h"

#pragma pack(push,packing)
#pragma pack(1)

//-------------------------------------------------------------
//- SMd3Header
//- File header for the md3 file, similar to md2
struct SMd3Header
{
	int m_iId;            //Must be IDP3 (860898377)
	int m_iVersion;       //Must be 15
	char m_cFilename[68]; //Full filename
	int m_iNumFrames;     //Number of animation keyframes
	int m_iNumTags;       //Number of tags
	int m_iNumMeshes;     //Number of sub-meshes
	int m_iMaxSkins;      //Maximum number of skins for model
	int m_iHeaderSize;    //Size of this header
	int m_iTagStart;      //File offset for tags
	int m_iTagEnd;        //End of tags
	int m_iFileSize;      //Size of file
};

//-------------------------------------------------------------
//- SMd3BoneFrame
//- A bounding box frame
struct SMd3BoneFrame
{
	float m_fMin[3]; //Minimum values for x,y,z
	float m_fMax[3]; //Max values for x,y,z
};

//-------------------------------------------------------------
//- SMd3Tag
//- Tag structure for Md3
struct SMd3Tag
{
	char m_cName[64];    //Name of tag
	CVector3 m_vecPos;   //Position of the tag
	CQuaternion m_qRot;  //Rotation for the frame

};

//-------------------------------------------------------------
//- SMd3MeshHeader
//- Header for each mesh in the md3 file
struct SMd3MeshHeader
{
	char m_cMeshId[4];    //Mesh ID
	char m_cName[68];     //Mesh name
	int m_iNumMeshFrames; //Number of animation frames in mesh
	int m_iNumSkins;      //Number of skins for this mesh
	int m_iNumVerts;      //Number of vertices
	int m_iNumTriangles;  //Number of triangles
	int m_iTriOffset;     //Offset for the triangles
	int m_iHeaderSize;    //Size of this header
	int m_iUVOffset;      //Texture coordinate offset
	int m_iVertexOffset;  //Offset for the vertices
	int m_iMeshSize;      //Total size of the mesh
};

//-------------------------------------------------------------
//- SMd3Vertex
//- A single vertex in the MD3 file
struct SMd3Vertex 
{
	float m_fVert[3];
};

//-------------------------------------------------------------
//- SMd3Face
//- A single triangle in the md3 file
struct SMd3Face
{
	unsigned int m_uiIndices[3];
};

//-------------------------------------------------------------
//- SMd3TexCoord
//- A U/F pair of texture coordinates for a vertex
struct SMd3TexCoord
{
	float m_fTexCoord[2];
};

struct SMd3Skin
{
	char m_cName[68];
	CImage m_Image;
};

//-------------------------------------------------------------
//- SMd3KeyFrame
//- A single animation keyframe, contains new vertex positions
struct SMd3KeyFrame
{
	SMd3Vertex * m_pVertices;
	SMd3KeyFrame()
	{
		m_pVertices = 0;
	}
	~SMd3KeyFrame()
	{
		if(m_pVertices)
		{
			delete [] m_pVertices;
			m_pVertices = 0;
		}
	}
};

//-------------------------------------------------------------
//- SMd3Mesh
//- A single mesh in the md3 file
struct SMd3Mesh
{
	SMd3MeshHeader m_Header;
	SMd3KeyFrame * m_pKeyFrames;
	SMd3Face * m_pFaces;
	SMd3TexCoord * m_pTexCoords;
	SMd3Skin * m_pSkins;
	SMd3Vertex * m_pInterp;  //Interpolated vertices

	SMd3Mesh()
	{
		m_pKeyFrames = 0;
		m_pFaces = 0;
		m_pTexCoords = 0;
		m_pSkins = 0;
		m_pInterp = 0;
	}
	~SMd3Mesh()
	{
		if(m_pKeyFrames)
		{
			delete [] m_pKeyFrames;
			m_pKeyFrames = 0;
		}
		if(m_pFaces)
		{
			delete [] m_pFaces;
			m_pFaces = 0;
		}
		if(m_pTexCoords)
		{
			delete [] m_pTexCoords;
			m_pTexCoords = 0;
		}
		if(m_pSkins)
		{
			delete [] m_pSkins;
			m_pSkins = 0;
		}
		if(m_pInterp)
		{
			delete [] m_pInterp;
			m_pInterp = 0;
		}
	}
};

//-------------------------------------------------------------
//- SMd3Attachments
//- A list of attachments and their joints

#pragma pack(pop, packing)

//-------------------------------------------------------------
//                        CMD3                                -
// author: Evan Pipho (evan@codershq.com)                     -
// date  : Sept 17, 2002                                      -
//-------------------------------------------------------------
class CMd3 : public CModel
{
public:

	//Load an Md3 model
	bool Load(const char * szFilename);
	//Render the model
	void Render();
	//Render the vertices at the current animation position
	void RenderT();
	//Animate the model
	void Animate(unsigned int uiStartFrame, unsigned int uiEndFrame, unsigned int uiFPS = 10, bool bLoop = true);
	//Attach another md3 to this one at a certain 
	void Attach(CMd3 * pModel, unsigned int uiTag);

	CMd3();
	~CMd3();


private:

	//Animation
	unsigned int m_uiLastFrame;
	float m_fLastInterp;
	//File header
	SMd3Header m_Header;
	//Tags
	SMd3Tag * m_pTags;
	//Meshes
	SMd3Mesh * m_pMeshes;
	//Interpolated tags
	SMd3Tag * m_pInterpTags;
	//Timer for interpolation
	CTimer m_Timer;
	
	//Child models, max 1 per tag
	CMd3 ** m_ppAttachments;
};


#endif	//MD3_H