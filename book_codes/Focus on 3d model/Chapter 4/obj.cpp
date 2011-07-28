/*************************************************************/
/*                          OBJ.CPP                          */
/*                                                           */
/* Purpose: Implementation for loader and renderer of OBJ    */
/*          3d models.                                       */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
//-------------------------------------------------------------
//                       INCLUDES                             -
//-------------------------------------------------------------
#include "obj.h"
#include "../basecode/app.h"
#include <cstdio>

//-------------------------------------------------------------
//- Load
//- Load an OBJ file and prep it for rendering
//-------------------------------------------------------------
bool CObj::Load(const char * szFilename)
{
	char cLine[256];	//A line of the obj file

	//Open up the file
	FILE * fp = fopen(szFilename, "rt");
	if(!fp)
	{
		APP->Log(COLOR_RED, "Could not open %s", szFilename);
		return false;
	}

	//Read until we hit the end of the file
	while(!feof(fp))
	{
		//Check the first char in the line
		int iStart = fgetc(fp);

		//If the first letter is v, it is either a vertex, a text coord, or a vertex normal
		if(iStart == 'v')
		{
			//get the second char
			int iNext = fgetc(fp);
			float fTemp[3];

			//if its a space, its a vertex coordinate
			if(iNext == ' ' || iNext == '\t')
			{
				//get the line
				fgets(cLine, 256, fp);
				//get the vertex coords
				sscanf(cLine, " %f %f %f", &fTemp[0], &fTemp[1], &fTemp[2]);
				//add to the vertex array
				m_vVertices.push_back(fTemp);
			}
			//if its a t, its a texture coord
			else if(iNext == 't')
			{
				//get the line
				fgets(cLine, 256, fp);
				//get the vertex coords
				sscanf(cLine, " %f %f", &fTemp[0], &fTemp[1]);
				//add to the vertex array
				m_vTexCoords.push_back(fTemp);
				m_bHasTexCoords = true;
			}
			//if its an n its a normal
			else if(iNext == 'n')
			{
				//get the line
				fgets(cLine, 256, fp);
				//get the vertex coords
				sscanf(cLine, " %f %f %f", &fTemp[0], &fTemp[1], &fTemp[2]);
				//add to the vertex array
				m_vNormals.push_back(fTemp);
				m_bHasNormals = true;
			}
			//else its something we don't support
			else
			{
				//scan the line and discard it
				fgets(cLine, 256, fp);
			}


		}
		//if the first letter is f, its a face
		else if(iStart == 'f')
		{
			//temp buffer to hold vertex indices
			int iTemp[3][3];
			memset(iTemp, 0, 36);
			//read in the line
			fgets(cLine, 256, fp);

			//If it has texture coords AND vertex normals
			if(m_bHasTexCoords && m_bHasNormals)
			{
				//extract the face info
				sscanf(cLine, " %i/%i/%i %i/%i/%i %i/%i/%i", &iTemp[0][0], &iTemp[1][0], &iTemp[2][0], 
															 &iTemp[0][1], &iTemp[1][1], &iTemp[2][1],
															 &iTemp[0][2], &iTemp[1][2], &iTemp[2][2]);
				//store the info in the faces structure
				m_vFaces.push_back(&iTemp[0][0]);
				
			}
			//Just has tex coords
			else if(m_bHasTexCoords && !m_bHasNormals)
			{
				//extract the face info
				sscanf(cLine, " %i/%i %i/%i %i/%i", &iTemp[0][0], &iTemp[1][0], 
													&iTemp[0][1], &iTemp[1][1], 
													&iTemp[0][2], &iTemp[1][2]);
				
				//store the info in the faces structure
				m_vFaces.push_back(&iTemp[0][0]);
			}
			//just normals
			else if(!m_bHasTexCoords && m_bHasNormals)
			{
				sscanf(cLine, " %i//%i %i//%i %i//%i", &iTemp[0][0], &iTemp[2][0], 
													&iTemp[0][1], &iTemp[2][1], 
													&iTemp[0][2], &iTemp[2][2]);
				//store the info in the faces structure
				m_vFaces.push_back(&iTemp[0][0]);

			}
			//Just vertices
			else
			{
				//extract the face info
				sscanf(cLine, " %i ", &iTemp[0][0], &iTemp[1][0], &iTemp[2][0]);
				//store the info in the faces structure
				m_vFaces.push_back(&iTemp[0][0]);

			}			
			
		}
		//if it isn't any of those, we don't care about it
		else
		{
			//read the whole line to advance
			fgets(cLine, 256, fp);
		}
	}

	m_pVerts = &m_vVertices[0];
	m_pTexCoords = &m_vTexCoords[0];
	m_pNormals = &m_vNormals[0];
	m_pFaces = &m_vFaces[0];

	return true;
}

//-------------------------------------------------------------
//- Render
//- Render the Obj file, no animation
//-------------------------------------------------------------
void CObj::Render()
{
	static int iNumFaces = m_vFaces.size();

	APP->Print(0.2f, 0.9f, "Triangles: %i", iNumFaces);


	glEnable(GL_TEXTURE_2D);

	//bind the texture
	if(m_pImage)
		m_pImage->Bind();

	//render the triangles
	glBegin(GL_TRIANGLES);

	//Texcoords, vertices AND normals
	if(m_bHasTexCoords && m_bHasNormals)
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[0])].m_fVec);
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[0])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);

			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[1])].m_fVec);
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[1])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);

			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[2])].m_fVec);
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[2])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}

	//Normals and vertices only
	else if(!m_bHasTexCoords && m_bHasNormals)
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[0])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);

			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[1])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);
			
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[2])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}
	
	//Texcoords and vertices only
	else if(m_bHasTexCoords && !m_bHasNormals)
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[0])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);
			
			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[1])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);
			
			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[2])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}
	//Verts only
	else
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);
			
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);
			
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}

	glEnd();
}

//-------------------------------------------------------------
//- SetSkin
//- Sets the current texture for the model, takes a reference to a CImage class
//-------------------------------------------------------------
void CObj::SetSkin(CImage& rSkin)
{
	m_pImage = &rSkin;
}

//-------------------------------------------------------------
//- Constructors
//- 1. Default Constructor
//- 2. takes filename, calls load
//-------------------------------------------------------------
CObj::CObj()
{
	m_pImage = 0;
	m_pVerts = 0;
	m_pTexCoords = 0;
	m_pNormals = 0;
	m_pFaces = 0;
	m_bHasTexCoords = false;
	m_bHasNormals = false;
}

CObj::CObj(const char * szFilename)
{
	m_pImage = 0;
	m_pTexCoords = 0;
	m_pNormals = 0;
	m_pVerts = 0;
	m_pFaces = 0;
	m_bHasTexCoords = false;
	m_bHasNormals = false;
	Load(szFilename);
}