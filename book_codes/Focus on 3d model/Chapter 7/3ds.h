/*************************************************************/
/*                           3DS.H                           */
/*                                                           */
/* Purpose: Definitions for C3ds, a class to load, render and*/
/*          animate the 3ds 3d file format.  Also contains   */
/*           all nessasary data structures for 3DS           */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#ifndef _3DS_H
#define _3DS_H

//-------------------------------------------------------------
//                       INCLUDES                             -
//-------------------------------------------------------------
#include "../basecode/image.h"
#include "../basecode/timer.h"
#include "model.h"
#include <vector>

using std::vector;

//-------------------------------------------------------------
//- constants for the 3DS file
//-------------------------------------------------------------
enum E3dsChunkIDs
{
	CHUNK_MAIN = 0x4D4D,		//Main chunk
	CHUNK_VERSION = 0x0002,		//Editor version
	CHUNK_EDIT3DS = 0x3D3D,		//Start of editor chunk
	CHUNK_MATERIAL = 0xAFFF,	//Start of materials chunk
	CHUNK_EDITCFG = 0x3D3E,		//Editor config (uneeded)
	CHUNK_MESH = 0x4000			//Start of mesh object
};

//Materials constants
enum E3dsMaterials
{
	CHUNK_MATNAME = 0xA000,		//Name of material		
	CHUNK_AMBIENT = 0xA010,     //Ambient color
	CHUNK_DIFFUSE =	0xA020,		//Holds the color of the material
	CHUNK_SPECULAR = 0xA030,    //Specular colors
	CHUNK_SHININESS = 0xA040,   //Shininess of material
	CHUNK_TRANSPARENT = 0xA050, //Transparency of material
	CHUNK_TEXTURE = 0xA200,		//Texture
	CHUNK_TEXFILE = 0xA300      //Texture filename
};

//Object constants
enum E3dsObject
{
	CHUNK_TRIOBJ = 0x4100,		//Triangle polygon mesh, contains subchunks
	CHUNK_VERTLIST = 0x4110,	//Vertices
	CHUNK_TRILIST = 0x4120,		//Faces
	CHUNK_OBJMAT = 0x4130,      //Material info 
	CHUNK_UVMAP = 0x4140,		//U/V mapping coordinates
	CHUNK_AXIS = 0x4160         //Local axis
};

//Animation constants
enum E3dsAnimation
{
	CHUNK_KEYFRAME = 0xB000,    //Start of animation chunk
	CHUNK_MESHINFO = 0xB002,    //Mesh anim data
	CHUNK_FRAMES = 0xB008,      //Number of keyframes
	CHUNK_PARAM = 0xB010,       //Object paramaeters for animation
	CHUNK_PIVOT = 0xB013,       //Object pivot point
	CHUNK_HIERARCHY = 0xB030,   //Object hierarchy position
	CHUNK_POSITION = 0xB020,    //Track position
	CHUNK_ROTATION = 0xB021,    //Track rotation
	CHUNK_SCALE = 0xB022        //Track scale
};

#pragma pack(push, packing)
#pragma pack(1)

//-------------------------------------------------------------
//- S3dsChunkHeader
//- Header for each chunk
struct S3dsChunkHeader
{
	unsigned short m_usID;
	unsigned int m_uiLength;
};

//-------------------------------------------------------------
//- S3dsVertex
//- Vertex structure for 3DS models
struct S3dsVertex
{
	float m_fVert[3];
};

//-------------------------------------------------------------
//- S3dsTexCoord
//- Texture coordinates for the model
struct S3dsTexCoord
{
	float m_fCoord[2];
};

//-------------------------------------------------------------
//- S3dsFace
//- Face of a 3ds model
struct S3dsFace
{
	unsigned short m_usIndices[3];		//Vertex indices
	CVector3 m_vecNormal;               //Face Normal
};

//-------------------------------------------------------------
//- S3dsMaterial
//- Material structure
struct S3dsMaterial
{
	char m_cName[64];     //Name of material
	float m_fAmbient[4];  //Ambient color
	float m_fDiffuse[4];  //Diffuse color
	float m_fSpecular[4]; //Specular color
	float m_fShininess;   //Matl shininess
	CImage m_Texture;     //Texturemap
};

//-------------------------------------------------------------
//- S3ds Objmat
//- Structure that holds what faces go to a material
struct S3dsObjMat
{
	unsigned short m_usMatIdx;
	vector<unsigned short> m_vFaceIndices;
};

//-------------------------------------------------------------
//- S3dsRotKeyFrame
//- Rotation keyframe for 3ds animation
//-------------------------------------------------------------
struct S3dsRotKeyFrame
{
	unsigned int m_uiKeyframe; //Keyframe position
	CVector3 m_vecAxis;	//Axis of rotation
	float m_fRot;       //Angle
};

//-------------------------------------------------------------
//- S3dsTransKeyFrame
//- Translation or Scale keyframe for 3ds animation 
//-------------------------------------------------------------
struct S3dsTransKeyFrame
{
	unsigned int m_uiKeyframe; //Keyframe position
	CVector3 m_vecPos;	//Axis of rotation
};

//-------------------------------------------------------------
//- S3dsAnimObject
//- Object for animation
struct S3dsAnimObject
{
	char m_cName[128];         //Object name
	unsigned int uiMeshIndex;  //Mesh that the animation affects
	short m_sParent;           //Parent object in heirarchy
	short m_sHierarchy;        //Hierarchy position
	CVector3 m_vecPivot;       //Pivot point
	vector<S3dsTransKeyFrame *> m_vPos;//Position keyframes
	vector<S3dsRotKeyFrame *> m_vRot; //Rotation keyframes
	vector<S3dsTransKeyFrame *> m_vScale; //Scaling keyframes
	int iCurRotFrame;
	int iCurTransFrame;

	CMatrix4X4 m_matLocal;    //Local transformation matrix
	CMatrix4X4 m_matAbsolute; //Abs transformation matrix (Local * parent's final)
	CMatrix4X4 m_matFinal;    //Final Transformation


	~S3dsAnimObject()
	{
		for(int x = 0; x < m_vRot.size(); x++)
			delete m_vRot[x];
		for(x = 0; x < m_vPos.size(); x++)
			delete m_vPos[x];
		for(x = 0; x < m_vScale.size(); x++)
			delete m_vScale[x];
	}
};

//-------------------------------------------------------------
//- S3dsMesh
//- Group Mesh Data
struct S3dsMesh
{
	char m_cName[128];
	vector<S3dsVertex *> m_vVertices;
	vector<S3dsTexCoord *> m_vTexCoords;
	vector<S3dsFace *> m_vFaces;
	unsigned short m_usNumFaces;
	vector<S3dsObjMat *> m_vMaterials;
	CMatrix4X4 m_matAxis;                //Local axis

	//Let it clean up after itself
	~S3dsMesh()
	{
		for(int x = 0; x < m_vVertices.size(); x++)
			delete m_vVertices[x];
		for(x = 0; x < m_vTexCoords.size(); x++)
			delete m_vTexCoords[x];
		for(x = 0; x < m_vFaces.size(); x++)
			delete m_vFaces[x];
		for(x = 0; x < m_vMaterials.size(); x++)
			delete m_vMaterials[x];
	}
};

#pragma pack(pop, packing)

//-------------------------------------------------------------
//                        C3DS                                -
// author: Evan Pipho (evan@codershq.com)                     -
// date  : Aug 17, 2002                                       -
//-------------------------------------------------------------
class C3ds : public CModel
{
public:

	//Load a 3DS model
	bool Load(const char * szFilename);
	//Render the model
	void Render();
	//Animate the model
	void Animate(unsigned int uiStartFrame, unsigned int uiEndFrame, unsigned int uiFPS = 10, bool bLoop = true);

	~C3ds();
	C3ds()
	{
		m_Timer.Init();
	}

private:

	//Size of file
	unsigned int m_uiFileSize;
	//Pointer to data
	unsigned char * m_ucpPtr;
	//Keyframe start and end
	unsigned int m_uiStartKF;
	unsigned int m_uiEndKF;
	//Timer for animation
	CTimer m_Timer;
	//Read file chunks and act accordingly
	void ReadChunks();
	//Set up animation
	void SetupAnimation();


	//Meshes
	vector<S3dsMesh *> m_vMeshes;
	//Materials
	vector<S3dsMaterial *> m_vMaterials;
	//Objects for animation
	vector<S3dsAnimObject *> m_vAnimObjects;


};


#endif	//_3DS_H