/*************************************************************/
/*                          3DS.CPP                          */
/*                                                           */
/* Purpose: Implementations for C3ds`                        */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#include "3ds.h"
#include "../basecode/app.h"
#include <cstdio>

//-------------------------------------------------------------
//- Load
//- Loads a 3DS model from a file
//-------------------------------------------------------------
bool C3ds::Load(const char * szFilename)
{
	unsigned char * ucpBuffer = 0;
	FILE * fp = fopen(szFilename, "rb");
	if(!fp)
	{
		APP->Log(COLOR_RED, "Could not open %s", szFilename);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	unsigned int uiEnd = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned int uiStart = ftell(fp);
	m_uiFileSize = uiEnd - uiStart;

	ucpBuffer = new unsigned char[m_uiFileSize];
	m_ucpPtr = ucpBuffer;
	if(!ucpBuffer)
	{
		APP->Log(COLOR_RED, "Out of memory");
		return false;
	}

	//Read the file into memory
	if(fread(ucpBuffer, 1, m_uiFileSize, fp) != m_uiFileSize)
	{
		APP->Log(COLOR_RED, "Could not read from %s", szFilename);
		delete [] ucpBuffer;
		return false;
	}
	//close the file
	fclose(fp);

	S3dsChunkHeader * curChunk = 0;
	//Read the first chunk header, should be CHUNK_MAIN
	curChunk = (S3dsChunkHeader *)m_ucpPtr;
	if(curChunk->m_usID != CHUNK_MAIN)
	{
		APP->Log(COLOR_RED, "%s not a valid 3DS file, missing CHUNK_MAIN chunk", szFilename);
		delete [] ucpBuffer;
		return false;
	}
	m_ucpPtr += 6;
	
	//Read the rest of the chunks and act accordingly
	ReadChunks();
	APP->Log(COLOR_GREEN, "3DS File: %s Loaded", szFilename);
	delete [] ucpBuffer;
	return true;
}

//-------------------------------------------------------------
//- Render
//- Renders the intial 3ds model in its inital position
//-------------------------------------------------------------
void C3ds::Render()
{	
	int iNumMeshes = m_vMeshes.size();

	for(int i = 0; i < iNumMeshes; i++)
	{
		if(m_vMeshes[i]->m_vMaterials.size() > 0)
		{
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
			glEnable(GL_BLEND);
			S3dsVertex ** pVert = &m_vMeshes[i]->m_vVertices[0];
			S3dsFace ** pFaces = &m_vMeshes[i]->m_vFaces[0];
			S3dsTexCoord ** pTexCoords = &m_vMeshes[i]->m_vTexCoords[0];
			unsigned int uiNumMaterials = m_vMeshes[i]->m_vMaterials.size();
			//Setup and render faces for each material
			for(int j = 0; j < uiNumMaterials; j++)
			{
				unsigned int uiIdx = m_vMeshes[i]->m_vMaterials[j]->m_usMatIdx;
				S3dsMaterial * pMat = m_vMaterials[uiIdx];
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMat->m_fAmbient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMat->m_fDiffuse);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMat->m_fSpecular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pMat->m_fShininess);
				m_vMaterials[uiIdx]->m_Texture.Bind();

				S3dsObjMat * pObjMat = m_vMeshes[i]->m_vMaterials[j];

				int iNumFaces = m_vMeshes[i]->m_vMaterials[j]->m_vFaceIndices.size();
				glBegin(GL_TRIANGLES);
				for(int k = 0; k < iNumFaces; k++)
				{
					glNormal3fv(pFaces[pObjMat->m_vFaceIndices[k]]->m_vecNormal.Get());
					glTexCoord2fv(pTexCoords[pFaces[pObjMat->m_vFaceIndices[k]]->m_usIndices[0]]->m_fCoord);
					glVertex3fv(pVert[pFaces[pObjMat->m_vFaceIndices[k]]->m_usIndices[0]]->m_fVert);
					glTexCoord2fv(pTexCoords[pFaces[pObjMat->m_vFaceIndices[k]]->m_usIndices[1]]->m_fCoord);
					glVertex3fv(pVert[pFaces[pObjMat->m_vFaceIndices[k]]->m_usIndices[1]]->m_fVert);
					glTexCoord2fv(pTexCoords[pFaces[pObjMat->m_vFaceIndices[k]]->m_usIndices[2]]->m_fCoord);
					glVertex3fv(pVert[pFaces[pObjMat->m_vFaceIndices[k]]->m_usIndices[2]]->m_fVert);
				}
				glEnd();
			}
		}		

		//No materials
		else
		{
			//No blending
			glDisable(GL_BLEND);
			//precalculatr
			int iNumFaces = m_vMeshes[i]->m_vFaces.size();	
			S3dsVertex ** pVertex = &m_vMeshes[i]->m_vVertices[0];
			//Draw
			glBegin(GL_TRIANGLES);
			for(int j = 0; j < iNumFaces; j++)
			{
				S3dsFace * pFace = m_vMeshes[i]->m_vFaces[j];
				glVertex3fv(pVertex[pFace->m_usIndices[0]]->m_fVert);
				glVertex3fv(pVertex[pFace->m_usIndices[1]]->m_fVert);
				glVertex3fv(pVertex[pFace->m_usIndices[2]]->m_fVert);
			}
			glEnd();	
		}
	}
}

//-------------------------------------------------------------
//- ReadChunks (private)
//- Read the nessasary chunks of the data file
//-------------------------------------------------------------
void C3ds::ReadChunks()
{
	S3dsChunkHeader * curChunk = 0;
	unsigned int uiBytesRead = 6;
	while(uiBytesRead < m_uiFileSize)
	{
		curChunk = (S3dsChunkHeader *)m_ucpPtr;
		//Find out what type of chunk it is
		switch(curChunk->m_usID)
		{
			//a version chunk
		case CHUNK_VERSION:
			{
				if(*(int *)&m_ucpPtr[6] < 3)
					APP->Log(COLOR_PURPLE, "Warning: 3DS file may not load correctly, version < 3");
				m_ucpPtr += curChunk->m_uiLength;
				uiBytesRead += curChunk->m_uiLength;
				break;
			}

			//Main editor chunk, contains many subchunks
		case CHUNK_EDIT3DS:
			{
				m_ucpPtr += 6;
				uiBytesRead += 6;
				break;
			}

			//Start of material chunk
		case CHUNK_MATERIAL:
			{	
				static int iCurMat = 0;
				unsigned char * ucpTmp = m_ucpPtr;
				unsigned int uiMatBytesRead = 6;
				ucpTmp += 6;
				m_vMaterials.push_back(new S3dsMaterial);
				//Read material chunks for the whole length of the MATERIAL chunk
				while(uiMatBytesRead < curChunk->m_uiLength)
				{
					//Subchunks
					S3dsChunkHeader * subChunk = (S3dsChunkHeader *)ucpTmp;
					//What subchunk?
					switch(subChunk->m_usID)
					{
					case CHUNK_MATNAME:
						{
							ucpTmp += 6;
							strcpy(m_vMaterials[iCurMat]->m_cName, (char *)ucpTmp);
							ucpTmp += subChunk->m_uiLength-6;
							break;
						}
						//Ambient lighting
					case CHUNK_AMBIENT:
						{
							ucpTmp += 6;
							//Read and store color data
							///R, G, B
							for(int x = 0; x < 3; x++)
								m_vMaterials[iCurMat]->m_fAmbient[x] = (float)(255 - ucpTmp[x])/255.0f;
							
							m_vMaterials[iCurMat]->m_fAmbient[3] = 1.0f;
							ucpTmp += subChunk->m_uiLength - 6;
							break;
						}
						//Diffuse Lighting
					case CHUNK_DIFFUSE:
						{
							ucpTmp += 6;
							//Read and store color data
							///R, G, B
							for(int x = 0; x < 3; x++)
								m_vMaterials[iCurMat]->m_fDiffuse[x] = (float)(255 - ucpTmp[x])/255.0f;
							m_vMaterials[iCurMat]->m_fDiffuse[3] = 1.0f;
							ucpTmp += subChunk->m_uiLength - 6;
							break;
						}
						//Specular Color
					case CHUNK_SPECULAR:
						{
							ucpTmp += 6;
							//Read and store color data
							///R, G, B
							for(int x = 0; x < 3; x++)
								m_vMaterials[iCurMat]->m_fSpecular[x] = (float)(255 - ucpTmp[x])/255.0f;
							m_vMaterials[iCurMat]->m_fSpecular[3] = 1.0f;
							ucpTmp += subChunk->m_uiLength - 6;
							break;
						}
						//Shininess
					case CHUNK_SHININESS:
						{
							ucpTmp += 12;
							//2 byte integer from 0 - 100 (percent)
							unsigned short usPercent = *(unsigned short *)ucpTmp;
							m_vMaterials[iCurMat]->m_fShininess = (float)usPercent / 100.0f;		
							ucpTmp += subChunk->m_uiLength - 12;
							break;
						}
						//Transparency
					case CHUNK_TRANSPARENT:
						{
							ucpTmp += 12;
							//2 byte integer from 0 - 100 (percent)
							unsigned short usPercent = *(unsigned short *)ucpTmp;
							m_vMaterials[iCurMat]->m_fDiffuse[3] = (float)(100 - usPercent) / 100.0f;		
							ucpTmp += subChunk->m_uiLength - 12;
							break;
						}
						//texture filename
					case CHUNK_TEXTURE:
						{
							ucpTmp += 6;
							unsigned int uiTexRead = 6;
							while(uiTexRead < subChunk->m_uiLength)
							{
								S3dsChunkHeader * sub2Chunk = (S3dsChunkHeader *)ucpTmp;
								if(sub2Chunk->m_usID == CHUNK_TEXFILE)
								{
									ucpTmp += 6;
									char cFilename[64]; //Texture filename
									strncpy(cFilename, (char *)ucpTmp, sub2Chunk->m_uiLength - 6);
									m_vMaterials[iCurMat]->m_Texture.Load(cFilename);
									ucpTmp += sub2Chunk->m_uiLength-6;
								}
								else
									ucpTmp += sub2Chunk->m_uiLength;
	
								uiTexRead += sub2Chunk->m_uiLength;
							}
							break;
						}
					default:
						{
							ucpTmp += subChunk->m_uiLength;
						}
					}
					uiMatBytesRead += subChunk->m_uiLength;
				}
				uiBytesRead += curChunk->m_uiLength;
				m_ucpPtr += curChunk->m_uiLength;
				break;
			}

			//Editor configuration data, uneeded for the loader, just skip it
		case CHUNK_EDITCFG:
			{
				m_ucpPtr += curChunk->m_uiLength;
				uiBytesRead += curChunk->m_uiLength;
				break;
			}

			//Object 
		case CHUNK_MESH:
			{
				static int iCurMesh = 0;
				unsigned int uiMeshBytesRead = 6;
				unsigned int uiTotalMeshBytes = curChunk->m_uiLength;
				unsigned char * ucpTmp = m_ucpPtr;

				//add a mesh to the list
				//S3dsMesh * tmpMesh;
				m_vMeshes.push_back(new S3dsMesh);

				ucpTmp += 6;
				//Read the name of the mesh
				strcpy(m_vMeshes[iCurMesh]->m_cName, (char *)ucpTmp);
				ucpTmp += strlen(m_vMeshes[iCurMesh]->m_cName) + 1;
				APP->Log(COLOR_PURPLE, "MESH NAME: %s", m_vMeshes[iCurMesh]->m_cName);
				uiMeshBytesRead += strlen(m_vMeshes[iCurMesh]->m_cName) + 1;

				m_vMeshes[iCurMesh]->m_vVertices.push_back(new S3dsVertex);

				while(uiMeshBytesRead < uiTotalMeshBytes)
				{
					S3dsChunkHeader * subChunk = (S3dsChunkHeader *)ucpTmp;
					//Find out what subchunk it is
					switch(subChunk->m_usID)
					{
						//its the start of a triangle object!
					case CHUNK_TRIOBJ:
						{
							ucpTmp += 6;
							uiMeshBytesRead += 6;
							break;
						}
						//its a list of vertices!
					case CHUNK_VERTLIST:
						{
							//skip the header
							unsigned char * ucpTmp2 = ucpTmp;
							ucpTmp2 += 6;
							unsigned short usNumVerts = *(unsigned short *)ucpTmp2;
							ucpTmp2 += 2;
						//	//Read vertices 1 by 1
							for(int x = 0; x < usNumVerts; x++)
							{
								//Read the vertices, changing them to openGl's coordinate system
								m_vMeshes[iCurMesh]->m_vVertices.push_back(new S3dsVertex);
								m_vMeshes[iCurMesh]->m_vVertices[x]->m_fVert[0] = *(float *)&ucpTmp2[0];
								m_vMeshes[iCurMesh]->m_vVertices[x]->m_fVert[1] = *(float *)&ucpTmp2[4];
								m_vMeshes[iCurMesh]->m_vVertices[x]->m_fVert[2] = *(float *)&ucpTmp2[8];
								ucpTmp2 += 12;
//								APP->Log(COLOR_YELLOW, "%f %f %f", m_vMeshes[iCurMesh]->m_vVertices[x].fVert[0],m_vMeshes[iCurMesh]->m_vVertices[x].fVert[1],m_vMeshes[iCurMesh]->m_vVertices[x].fVert[2]);
							}
							ucpTmp += subChunk->m_uiLength;
							uiMeshBytesRead += subChunk->m_uiLength;
							break;
						}
						//Face information
					case CHUNK_TRILIST:
						{
							unsigned int uiTriBytesRead = 8;
							//skip the header
							ucpTmp += 6;
							//Get the number of triangles
							unsigned short usNumPolys = *(unsigned short *)ucpTmp;
							m_vMeshes[iCurMesh]->m_usNumFaces = usNumPolys;
							ucpTmp += 2;

							S3dsVertex ** pVert = &m_vMeshes[iCurMesh]->m_vVertices[0];					
							//Read the triangles
							for(int x = 0; x < usNumPolys; x++)
							{
								m_vMeshes[iCurMesh]->m_vFaces.push_back(new S3dsFace);
								S3dsFace * pFace = m_vMeshes[iCurMesh]->m_vFaces[x];
								pFace->m_usIndices[0] = *(unsigned short *)&ucpTmp[0];
								pFace->m_usIndices[1] = *(unsigned short *)&ucpTmp[2];
								pFace->m_usIndices[2] = *(unsigned short *)&ucpTmp[4];
								//Calculate Face Normal
								pFace->m_vecNormal = CalcFaceNormal(CVector3(pVert[pFace->m_usIndices[0]]->m_fVert), 
									                                CVector3(pVert[pFace->m_usIndices[1]]->m_fVert), 
																	CVector3(pVert[pFace->m_usIndices[2]]->m_fVert));
								ucpTmp += 8;
								uiTriBytesRead += 8;
							}
							int iCurObjMat = 0;
							while(uiTriBytesRead < subChunk->m_uiLength)
							{
								S3dsChunkHeader * sub2Chunk = (S3dsChunkHeader *)ucpTmp;
								//Material properties
								if(sub2Chunk->m_usID == CHUNK_OBJMAT)
								{
									
									ucpTmp += 6;
									char cMatName[64];
									//Get material name
									strcpy(cMatName, (char *)ucpTmp);
									//Find the index
									m_vMeshes[iCurMesh]->m_vMaterials.push_back(new S3dsObjMat);
									m_vMeshes[iCurMesh]->m_vMaterials[iCurObjMat]->m_usMatIdx = 0;
									for(int x = 0; x < m_vMaterials.size(); x++)
									{
										if(strcmp(cMatName, m_vMaterials[x]->m_cName) == 0)
											m_vMeshes[iCurMesh]->m_vMaterials[iCurObjMat]->m_usMatIdx = x;
									}
									//Read face indices
									unsigned int uiObjMatBytesRead = 9 + strlen(cMatName);
									ucpTmp += strlen(cMatName) + 3;

									while(uiObjMatBytesRead < sub2Chunk->m_uiLength)
									{
										unsigned short usFace =  *(unsigned short *)ucpTmp;
										m_vMeshes[iCurMesh]->m_vMaterials[iCurObjMat]->m_vFaceIndices.push_back(usFace);
										ucpTmp += 2;
										uiObjMatBytesRead += 2;
									}

									iCurObjMat++;
									uiTriBytesRead += sub2Chunk->m_uiLength;
								}
								else
								{
									ucpTmp += sub2Chunk->m_uiLength;
									uiTriBytesRead += sub2Chunk->m_uiLength;
								}
							}
							uiMeshBytesRead += subChunk->m_uiLength;
							break;
						}
						//U/V texture coordinates
					case CHUNK_UVMAP:
						{
							//skip the header
							ucpTmp += 6;
							//Find the number of texture coordinates
							unsigned short usNumTexCoords = *(unsigned short *)ucpTmp;
							ucpTmp += 2;
					
							//Allocate space for texture coords
							//Read the texture coordinates
							for(int x = 0; x < usNumTexCoords; x++)
							{
								m_vMeshes[iCurMesh]->m_vTexCoords.push_back(new S3dsTexCoord);
								memcpy(m_vMeshes[iCurMesh]->m_vTexCoords[x]->m_fCoord, ucpTmp, 8);
								ucpTmp += 8;
							}
							uiMeshBytesRead += subChunk->m_uiLength;
							break;
						}
						//Local axis
					case CHUNK_AXIS:
						{
							ucpTmp += 6;
							ucpTmp += 48;

								break;
						}
						//its nothing we care about!
					default:
						{
							ucpTmp += subChunk->m_uiLength;
							uiMeshBytesRead += subChunk->m_uiLength;
							break;
						}
					}
				}
		
				m_ucpPtr += curChunk->m_uiLength;
				uiBytesRead += uiTotalMeshBytes;
				iCurMesh++;
				break;
			}
			//Read animation data
			//If it gets this far it is either an unknown or unneeded chunk, so we just skip over it
			default:
			{
				uiBytesRead += curChunk->m_uiLength;
				m_ucpPtr += curChunk->m_uiLength;
			}

		}
	}
}


//-------------------------------------------------------------
//- Destructor
//-------------------------------------------------------------
C3ds::~C3ds()
{
	for(int x = 0; x < m_vMaterials.size(); x++)
		delete m_vMaterials[x];
	for(x = 0; x < m_vMeshes.size(); x++)
		delete m_vMeshes[x];
}









