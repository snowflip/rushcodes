/*************************************************************/
/*                          MD3.CPP                          */
/*                                                           */
/* Purpose: Implementations for CMd3`                        */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#include "md3.h"
#include "../basecode/app.h"
#include <cstdio>

//-------------------------------------------------------------
//- Load
//- Loads an MD3 model from a file
//-------------------------------------------------------------
bool CMd3::Load(const char * szFilename)
{
	unsigned char * ucpBuffer = 0;
	unsigned char * ucpTmp = 0;   //Pointer to increment to avoid causing problems with ucpBuffer; 
	int iFileSize = 0;

	//Read file into memory
	FILE * f = fopen(szFilename, "rb");
	if(!f)
	{
		APP->Log(COLOR_RED, "Could not open %s", szFilename);
	}
	fseek(f, 0, SEEK_END);
	int iEnd = ftell(f);
	fseek(f, 0, SEEK_SET);
	int iStart = ftell(f);
	iFileSize = iEnd - iStart;
	ucpBuffer = new unsigned char[iFileSize];
	ucpTmp = ucpBuffer;
	if(!ucpBuffer)
	{
		APP->Log(COLOR_RED, "Out of memory for %s", szFilename);
		return false;
	}
	fread(ucpBuffer, 1, iFileSize, f);
	fclose(f);

	//Make sure header is valid
	m_Header = *(SMd3Header *)ucpTmp;
	ucpTmp += sizeof(SMd3Header);
	if(m_Header.m_iId != 860898377 || m_Header.m_iVersion != 15)
	{
		APP->Log(COLOR_RED, "%s is not a valid md3 file");
		return false;
	}
	
	//Skip "boneframes" because they appear to be useless for md3
	ucpTmp += m_Header.m_iNumFrames * 56;

	//Read tags
	//There is a set of tags for every animation frame
	m_pTags = new SMd3Tag[m_Header.m_iNumTags * m_Header.m_iNumFrames];
	m_pInterpTags = new SMd3Tag[m_Header.m_iNumTags];
	m_ppAttachments = new CMd3 *[m_Header.m_iNumTags];
	for(int x = 0; x < m_Header.m_iNumTags; x++)
	{
		m_ppAttachments[x] = 0;
	}
	for(x = 0; x < m_Header.m_iNumTags * m_Header.m_iNumFrames; x++)
	{
		memcpy(m_pTags[x].m_cName, ucpTmp, 64);
		ucpTmp += 64;
		memcpy(m_pTags[x].m_vecPos.Get(), ucpTmp, 12);
		ucpTmp += 12;
		CMatrix3X3 tmp;	
		memcpy(tmp.Get(), ucpTmp, 36);
		m_pTags[x].m_qRot.FromMatrix(tmp);
		ucpTmp += 36;
		
	}

	//Read mesh data
	m_pMeshes = new SMd3Mesh[m_Header.m_iNumMeshes];
	unsigned char * ucpTmp2 = ucpTmp;
	for(x = 0; x < m_Header.m_iNumMeshes; x++)
	{
		memcpy(&m_pMeshes[x].m_Header, ucpTmp2, sizeof(SMd3MeshHeader));
		ucpTmp2 += m_pMeshes[x].m_Header.m_iHeaderSize;
		//Get the skin filenames
		m_pMeshes[x].m_pSkins = new SMd3Skin[m_pMeshes[x].m_Header.m_iNumSkins];
		for(int y = 0; y < m_pMeshes[x].m_Header.m_iNumSkins; y++)
		{
			char cName[68];
			memcpy(cName, ucpTmp2, 68);
			ucpTmp2 += 68;
			//slice off the last filename
			char * cpStart = strrchr(cName, '\\');
			 if(cpStart == 0)
				 cpStart = strrchr(cName, '/');
			if(cpStart)
				strcpy(m_pMeshes[x].m_pSkins[y].m_cName, cpStart+1);
			else
				strcpy(m_pMeshes[x].m_pSkins[y].m_cName, cName);

			m_pMeshes[x].m_pSkins[y].m_Image.Load(m_pMeshes[x].m_pSkins[y].m_cName);
		}
		//Get vertices
		//Vertex structure in file is 3 shorts + 2 bytes
		//the shorts must be divided by 64 to get the final position
		m_pMeshes[x].m_pKeyFrames = new SMd3KeyFrame[m_pMeshes[x].m_Header.m_iNumMeshFrames];
		ucpTmp2 = ucpTmp + m_pMeshes[x].m_Header.m_iVertexOffset;
		// Read the vertices one keyframe at a time
		for(int z = 0; z < m_pMeshes[x].m_Header.m_iNumMeshFrames; z++)
		{
			m_pMeshes[x].m_pKeyFrames[z].m_pVertices = new SMd3Vertex[m_pMeshes[x].m_Header.m_iNumVerts];
			for(y = 0; y < m_pMeshes[x].m_Header.m_iNumVerts; y++)
			{
				short sTmp = *(short *)ucpTmp2;
				m_pMeshes[x].m_pKeyFrames[z].m_pVertices[y].m_fVert[0] = (float)sTmp / 64.0f;
				sTmp = *(short *)&ucpTmp2[2];
				m_pMeshes[x].m_pKeyFrames[z].m_pVertices[y].m_fVert[1] = (float)sTmp / 64.0f;
				sTmp = *(short *)&ucpTmp2[4];
				m_pMeshes[x].m_pKeyFrames[z].m_pVertices[y].m_fVert[2] = (float)sTmp / 64.0f;
				ucpTmp2 += 6;
				//extra two bytes
				ucpTmp2 += 2;
			}
		}
		//Get faces
		m_pMeshes[x].m_pFaces = new SMd3Face[m_pMeshes[x].m_Header.m_iNumTriangles];
		ucpTmp2 = ucpTmp + m_pMeshes[x].m_Header.m_iTriOffset;
		for(y = 0; y < m_pMeshes[x].m_Header.m_iNumTriangles; y++)
		{
			memcpy(m_pMeshes[x].m_pFaces[y].m_uiIndices, ucpTmp2, 12);
			ucpTmp2 += 12;
		}
		//Get texture coordinates
		m_pMeshes[x].m_pTexCoords = new SMd3TexCoord[m_pMeshes[x].m_Header.m_iNumVerts];
		ucpTmp2 = ucpTmp + m_pMeshes[x].m_Header.m_iUVOffset;
		for(y = 0; y < m_pMeshes[x].m_Header.m_iNumVerts; y++)
		{
			memcpy(m_pMeshes[x].m_pTexCoords[y].m_fTexCoord, ucpTmp2, 8);
			ucpTmp2 += 8;
		}
		m_pMeshes[x].m_pInterp = 0;
		ucpTmp += m_pMeshes[x].m_Header.m_iMeshSize;
		ucpTmp2 = ucpTmp;
	}

	//Clean up
	if(ucpBuffer)
		delete [] ucpBuffer;
	APP->Log(COLOR_GREEN, "MD3 Model %s Loaded", szFilename);
	return true;
}

//-------------------------------------------------------------
//- Animate
//- Animate the MD3 model from a start to an end frame at a given speed
//-------------------------------------------------------------
void CMd3::Animate(unsigned int uiStartFrame, unsigned int uiEndFrame, unsigned int uiFPS, bool bLoop)
{
	if(m_uiLastFrame < uiStartFrame)
		m_uiLastFrame = uiStartFrame;

	int iNumMeshes = m_Header.m_iNumMeshes;

	//Prevent invalid framecounts
	if(uiEndFrame >= (unsigned)m_Header.m_iNumFrames)
		uiEndFrame = m_Header.m_iNumFrames-1;

	if(m_uiLastFrame > uiEndFrame)
		m_uiLastFrame = uiStartFrame;

	float fTime = m_Timer.GetSeconds();

	float fInterp = fTime * uiFPS  + m_fLastInterp;
	//Get an interpolation value between 0 and 1
	while(fInterp > 1.0f)
	{
		m_uiLastFrame++;
		if(m_uiLastFrame >= uiEndFrame)
			m_uiLastFrame = uiStartFrame;
		fInterp -= 1.0f;
	}
	m_fLastInterp = fInterp;
				
	unsigned int uiNextFrame = m_uiLastFrame+1;
	if(m_uiLastFrame == uiEndFrame-1)
		uiNextFrame = uiStartFrame;

	//Interpolate the tags
	for(int x = 0; x < m_Header.m_iNumTags; x++)
	{
		if(uiStartFrame == uiEndFrame)
			memcpy(&m_pInterpTags[x], &m_pTags[m_uiLastFrame * m_Header.m_iNumTags + x], sizeof(SMd3Tag));

		else
		{
			SMd3Tag * pLastTag = &m_pTags[m_uiLastFrame * m_Header.m_iNumTags + x];
			SMd3Tag * pNextTag = &m_pTags[uiNextFrame * m_Header.m_iNumTags + x];
			m_pInterpTags[x].m_qRot = SLERP(pLastTag->m_qRot, pNextTag->m_qRot, fInterp);
			m_pInterpTags[x].m_vecPos[0] = pLastTag->m_vecPos[0] + (pNextTag->m_vecPos[0] - pLastTag->m_vecPos[0]) * fInterp;
			m_pInterpTags[x].m_vecPos[1] = pLastTag->m_vecPos[1] + (pNextTag->m_vecPos[1] - pLastTag->m_vecPos[1]) * fInterp;
			m_pInterpTags[x].m_vecPos[2] = pLastTag->m_vecPos[2] + (pNextTag->m_vecPos[2] - pLastTag->m_vecPos[2]) * fInterp;
		}	
	}

	for(x = 0; x < iNumMeshes; x++)
	{	
		//Make sure there is a place to put the interpolated vertices
		if(!m_pMeshes[x].m_pInterp)
			m_pMeshes[x].m_pInterp = new SMd3Vertex[m_pMeshes[x].m_Header.m_iNumVerts];
		
		//Precalculations
		SMd3Vertex * pVertices = m_pMeshes[x].m_pInterp;
		//Interpolate vertices
		if(uiStartFrame == uiEndFrame)
			memcpy(m_pMeshes[x].m_pInterp, m_pMeshes[x].m_pKeyFrames[m_uiLastFrame].m_pVertices, sizeof(SMd3Vertex) * m_pMeshes[x].m_Header.m_iNumVerts);
		else
		{
			SMd3KeyFrame * pLastFrame = &m_pMeshes[x].m_pKeyFrames[m_uiLastFrame];
			SMd3KeyFrame * pNextFrame = &m_pMeshes[x].m_pKeyFrames[uiNextFrame];

			int iNumVerts = m_pMeshes[x].m_Header.m_iNumVerts;
			for(int y = 0; y < iNumVerts; y++)
			{
				pVertices[y].m_fVert[0] = pLastFrame->m_pVertices[y].m_fVert[0] + (pNextFrame->m_pVertices[y].m_fVert[0] - pLastFrame->m_pVertices[y].m_fVert[0]) * fInterp;
				pVertices[y].m_fVert[1] = pLastFrame->m_pVertices[y].m_fVert[1] + (pNextFrame->m_pVertices[y].m_fVert[1] - pLastFrame->m_pVertices[y].m_fVert[1]) * fInterp;
				pVertices[y].m_fVert[2] = pLastFrame->m_pVertices[y].m_fVert[2] + (pNextFrame->m_pVertices[y].m_fVert[2] - pLastFrame->m_pVertices[y].m_fVert[2]) * fInterp;
			}
		}
	}
}

//-------------------------------------------------------------
//- RenderT
//- Render the transformed vertices, called after "Animate"
//- Will also render all children nodes
//-------------------------------------------------------------
void CMd3::RenderT()
{
	int iNumMeshes = m_Header.m_iNumMeshes;
	for(int x = 0; x < iNumMeshes; x++) 
	{
		int iNumFaces = m_pMeshes[x].m_Header.m_iNumTriangles;
		SMd3TexCoord * pTexCoords = m_pMeshes[x].m_pTexCoords;
		SMd3Face * pFaces = m_pMeshes[x].m_pFaces;
		SMd3Vertex * pVertices = m_pMeshes[x].m_pInterp;

		//Bind the first skin for the mesh
		m_pMeshes[x].m_pSkins[0].m_Image.Bind();

		glBegin(GL_TRIANGLES);
		for(int y = 0; y < iNumFaces; y++)
		{
			glTexCoord2fv(pTexCoords[m_pMeshes[x].m_pFaces[y].m_uiIndices[0]].m_fTexCoord);
			glVertex3fv(pVertices[pFaces[y].m_uiIndices[0]].m_fVert);
			
			glTexCoord2fv(pTexCoords[pFaces[y].m_uiIndices[1]].m_fTexCoord);
			glVertex3fv(pVertices[pFaces[y].m_uiIndices[1]].m_fVert);

			glTexCoord2fv(pTexCoords[m_pMeshes[x].m_pFaces[y].m_uiIndices[2]].m_fTexCoord);
			glVertex3fv(pVertices[pFaces[y].m_uiIndices[2]].m_fVert);
		}
		glEnd();
	}
	//Render all attached models
	for(x = 0; x < m_Header.m_iNumTags; x++)
	{
		if(m_ppAttachments[x])
		{
			CMatrix4X4 matTmp;
			matTmp.Identity();
			matTmp.FromQuaternion(m_pInterpTags[x].m_qRot);
		//	matTmp.Transpose();
			matTmp.SetTranslation(m_pInterpTags[x].m_vecPos.Get());
			glPushMatrix();
			glMultMatrixf(matTmp.Get());
				m_ppAttachments[x]->RenderT();
			glPopMatrix();
		}
	}
}

//-------------------------------------------------------------
//- Render
//- Renders the Md3 model in its inital position (frame 0);
//-------------------------------------------------------------
void CMd3::Render()
{
	int iNumMeshes = m_Header.m_iNumMeshes;
	for(int x = 0; x < iNumMeshes; x++)
	{		
		//Bind the first skin for the mesh
		m_pMeshes[x].m_pSkins[0].m_Image.Bind();
		//Precalculations
		int iNumFaces = m_pMeshes[x].m_Header.m_iNumTriangles;
		SMd3TexCoord * pTexCoords = m_pMeshes[x].m_pTexCoords;
		SMd3Vertex * pVertices = m_pMeshes[x].m_pKeyFrames[0].m_pVertices;
		SMd3Face * pFaces = m_pMeshes[x].m_pFaces;

		glBegin(GL_TRIANGLES);
		for(int y = 0; y < iNumFaces; y++)
		{
			glTexCoord2fv(pTexCoords[m_pMeshes[x].m_pFaces[y].m_uiIndices[0]].m_fTexCoord);
			glVertex3fv(pVertices[pFaces[y].m_uiIndices[0]].m_fVert);
			
			glTexCoord2fv(pTexCoords[pFaces[y].m_uiIndices[1]].m_fTexCoord);
			glVertex3fv(pVertices[pFaces[y].m_uiIndices[1]].m_fVert);

			glTexCoord2fv(pTexCoords[m_pMeshes[x].m_pFaces[y].m_uiIndices[2]].m_fTexCoord);
			glVertex3fv(pVertices[pFaces[y].m_uiIndices[2]].m_fVert);
		}
		glEnd();
	}
}

//-------------------------------------------------------------
//- Attach
//- Attach another model to a a tag
//-------------------------------------------------------------
void CMd3::Attach(CMd3* pModel, unsigned int uiTag)
{
	m_ppAttachments[uiTag] = pModel;
}

//-------------------------------------------------------------
//- Ctor/Dtor
//- Default constructor
//-------------------------------------------------------------
CMd3::CMd3()
{
	m_pTags = 0;
	m_pInterpTags = 0;
	m_pMeshes = 0;
	m_uiLastFrame = 0;
	m_fLastInterp= 0.0f;
	m_Timer.Init();
}

CMd3::~CMd3()
{
	if(m_pTags)
	{
		delete [] m_pTags;
		m_pTags = 0;
	}
	if(m_pMeshes)
	{
		delete [] m_pMeshes;
		m_pMeshes = 0;
	}
	if(m_pInterpTags)
	{
		delete [] m_pInterpTags;
		m_pInterpTags = 0;
	}
	if(m_ppAttachments)
	{
		delete [] m_ppAttachments;
		m_ppAttachments = 0;
	}
}
