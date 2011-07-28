/*************************************************************/
/*                           MD2.H                           */
/*                                                           */
/* Purpose: Definitions for CMd2, a class to load, render and*/
/*          animate id Software's .md2 file format.  Also    */
/*          contains all nessasary data structures for MD2   */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#ifndef MD2_H
#define MD2_h

//-------------------------------------------------------------
//                       INCLUDES                             -
//-------------------------------------------------------------
#include "../basecode/image.h"
#include "../basecode/timer.h"
#include "model.h"


//-------------------------------------------------------------
//- SMD2Header
//- Header for all Md2 files, 
struct SMD2Header
{
   int m_iMagicNum; //Always IDP2 (844121161)
   int m_iVersion;  //8
   int m_iSkinWidthPx;  
   int m_iSkinHeightPx; 
   int m_iFrameSize; 
   int m_iNumSkins; 
   int m_iNumVertices; 
   int m_iNumTexCoords; 
   int m_iNumTriangles; 
   int m_iNumGLCommands; 
   int m_iNumFrames; 
   int m_iOffsetSkins; 
   int m_iOffsetTexCoords; 
   int m_iOffsetTriangles; 
   int m_iOffsetFrames; 
   int m_iOffsetGlCommands; 
   int m_iFileSize; 
};

//-------------------------------------------------------------
//- SMD2Vert
//- Vertex structure for MD2
struct SMD2Vert
{
	float m_fVert[3];
	unsigned char m_ucReserved;
};


//-------------------------------------------------------------
//- SMD2Frame
//- Frame information for the model file 
struct SMD2Frame
{
	float m_fScale[3];
	float m_fTrans[3];
	char m_caName[16];
	SMD2Vert * m_pVerts;

	//Cleans up after itself
	SMD2Frame()
	{
		m_pVerts = 0;
	}

	~SMD2Frame()
	{
		if(m_pVerts)
			delete [] m_pVerts;
	}
};

//-------------------------------------------------------------
//- SMD2Tri
//- Triangle information for the MD2
struct SMD2Tri
{
	unsigned short m_sVertIndices[3];
	unsigned short m_sTexIndices[3];
};

//-------------------------------------------------------------
//- SMD2TexCoord
//- Texture coord information for the MD2
struct SMD2TexCoord
{
	float m_fTex[2];
};

//-------------------------------------------------------------
//- SMD2Skin
//- Name of a single skin in the md2 file
struct SMD2Skin
{
	char m_caSkin[64];	//filename
	CImage m_Image;		//Image file ready for texturing
};

//-------------------------------------------------------------
//                        CTIMER                              -
// author: Evan Pipho (evan@codershq.com)                     -
// date  : Jul 10, 2002                                       -
//-------------------------------------------------------------
class CMd2 : public CModel
{
public:

	//Set skin to one of the files specified in the md2 files itself
	void SetSkin(unsigned int uiSkin);
	//Set skin to a different image
	void SetSkin(CImage& skin);

	//Load the file
	bool Load(const char * szFilename);
	
	//Render file at the initial position
	void Render();
	//Render the file at a certain frame
	void Render(unsigned int uiFrame);

	//Animate the md2 model (start and end frames of 0 and 0 will loop through the WHOLE model
	void Animate(float fSpeed = 30.0f, unsigned int uiStartFrame = 0, unsigned int uiEndFrame = 0, bool bLoop = true);

	//constructors/destructo
	CMd2();
	CMd2(const char * szFile);
	~CMd2();

private:
	
	CTimer m_Timer;
	//file header information
	SMD2Header m_Head; 
	//Frame information
	SMD2Frame * m_pFrames;
	//Triangles
	SMD2Tri * m_pTriangles;
	//Texure coords
	SMD2TexCoord * m_pTexCoords;
	//Skin files
	SMD2Skin * m_pSkins;
	//Interpolated vertices
	SMD2Vert * m_pVerts;
	//Current skin
	unsigned int m_uiSkin;
	//Using a custom skin?
	bool m_bIsCustomSkin;
	//The custom skin
	CImage * m_pCustSkin;

};


#endif //MD2_H