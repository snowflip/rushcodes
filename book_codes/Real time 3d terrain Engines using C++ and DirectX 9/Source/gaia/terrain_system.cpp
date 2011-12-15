#define CTERRAINSYSTEM_CPP
#define GAIA_DLL
/***************************************************************\

    ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^        ^^^^^^^^     ^^     ^^^^^^^^
    ^^   ^^^  ^^    ^^     ^^     ^^    ^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^^^^^^^  ^^    ^^  ^^^^^^^^  ^^    ^^

    sample code from the book...
    Real Time 3D Terrain Engines Using C++ and DirectX

    by Greg Snook
    greg@mightystudios.com

\***************************************************************/

#include "game_host.h"
#include "terrain_system.h"
#include "terrain_section.h"
#include "texture.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "camera.h"

using namespace gaia;

// vertex definitions for the basic terrain.
// we use two vertex streams. The first is a 
// single vertex buffer shared by all the
// sectors of the terrain. The second is a
// vertex buffer created by each sector of the
// terrain to store local height and normal
// data
static D3DVERTEXELEMENT9 vertex_description[]=
{
	// local data (stream 0)
	{ 0, 0, D3DDECLTYPE_FLOAT2, 
		D3DDECLMETHOD_DEFAULT, 
		D3DDECLUSAGE_POSITION, 0 },
	{ 0, 8, D3DDECLTYPE_FLOAT2, 
		D3DDECLMETHOD_DEFAULT, 
		D3DDECLUSAGE_TEXCOORD, 0 },

	// sector data (stream 1)
	{ 1, 0, D3DDECLTYPE_FLOAT1, 
		D3DDECLMETHOD_DEFAULT, 
		D3DDECLUSAGE_POSITION, 1 },
	{ 1, 4, D3DDECLTYPE_FLOAT3, 
		D3DDECLMETHOD_DEFAULT, 
		D3DDECLUSAGE_NORMAL,   0 },
	D3DDECL_END()
};

//
// This function is the starting point 
// for converting a height map into 
// vertex data
//
bool cTerrain::create(
	cSceneNode* pRootNode, 
	cTexture* heightMap, 
	const cRect3d& worldExtents, 
	uint8 shift)
{
	bool result = false;

	m_sectorShift = shift;
	m_sectorUnits = 1<<shift;
	m_sectorVerts = m_sectorUnits+1; 

	m_pRootNode = pRootNode;
	m_worldExtents = worldExtents;
	m_worldSize = worldExtents.size();
	
	m_tableWidth = (uint16)heightMap->width();
	m_tableHeight = (uint16)heightMap->height();

	m_mapScale.x = m_worldSize.x/m_tableWidth;
	m_mapScale.y = m_worldSize.y/m_tableHeight;
	m_mapScale.z = m_worldSize.z/255.0f;

	// convert the height map to
	// data stored in local tables
	buildHeightAndNormalTables(heightMap);

	m_sectorCountX = 
		m_tableWidth>>m_sectorShift;
	m_sectorCountY = 
		m_tableHeight>>m_sectorShift;

	m_sectorSize.set(
		m_worldSize.x/m_sectorCountX,
		m_worldSize.y/m_sectorCountY);

	// create the vertex and index buffer
	// objects which are shared by the sectors
	if (buildVertexBuffer())
	{
		if (setVertexDescription())
		{
			if (buildIndexBuffer())
			{
				// now go build each sector of the terrain
				result = allocateSectors();
			}
		}
	}

	if(!result)
	{
		destroy();
	}

	return result;
}

//
// This function creates the individual 
// sectors of the terrain
// 
bool cTerrain::allocateSectors()
{
	m_pSectorArray = 
		new cTerrainSection[
			m_sectorCountX*m_sectorCountY];

	// create the sector objects themselves
	for (int y=0; y<m_sectorCountY; ++y)
	{
		for (int x=0; x<m_sectorCountX; ++x)
		{
			cVector2 sectorPos(
				m_worldExtents.x0+(x*m_sectorSize.x),
				m_worldExtents.y0+(y*m_sectorSize.y));

			cRect2d sectorRect(
				sectorPos.x, sectorPos.x+m_sectorSize.x,
				sectorPos.y, sectorPos.y+m_sectorSize.y);

			uint16 xPixel = x<<m_sectorShift;
			uint16 yPixel = y<<m_sectorShift;

			uint16 index = (y*m_sectorCountX)+x;
			
			if (!m_pSectorArray[index].create(
				m_pRootNode, 
				this, 
				x, y, 
				xPixel, yPixel, 
				m_sectorVerts, 
				m_sectorVerts, 
				sectorRect))
			{
				return false;
			}
		}
	}

	return true;
}


void cTerrain::destroy()
{
	safe_delete_array(m_pSectorArray);

	safe_release(m_pVertexGrid);
	safe_release(m_pTriangles);
	safe_release(m_pRenderMethod);

	safe_delete_array(m_heightTable);
	safe_delete_array(m_normalTable);
}

void cTerrain::setRenderMethod(cRenderMethod* pRenderMethod)
{
	safe_release(m_pRenderMethod);
	m_pRenderMethod = pRenderMethod;
	if (m_pRenderMethod)
	{
		m_pRenderMethod->AddRef();
	}
}

//
// Here we convert the height map data into
// floating point height and surface normals.
// each is stored within a table of values
// within the terrain system class
//
void cTerrain::buildHeightAndNormalTables(cTexture* pTexture)
{
	safe_delete_array(m_heightTable);
	safe_delete_array(m_normalTable);


	int maxY = m_tableHeight;
	int maxX = m_tableWidth;
	int x,y;

	m_heightTable = new float[maxX*maxY];
	m_normalTable = new cVector3[maxX*maxY];

	// first, build a table of heights
	D3DLOCKED_RECT lockedRect;
	if(SUCCEEDED(
		pTexture->getTexture()->
		LockRect(0, &lockedRect, 
			0, D3DLOCK_READONLY)))
	{
		uint8* pHeightMap = (uint8*)lockedRect.pBits;

		for(y=0; y<maxY; ++y)
		{
			for(x=0; x<maxX; ++x)
			{
				int iHeight = 
					pHeightMap[(y*lockedRect.Pitch)+(x*4)];
				float fHeight = 
					(iHeight * m_mapScale.z) 
					+ m_worldExtents.z0;

				m_heightTable[(y*maxX)+x] = 
					fHeight;
			}
		}

		pTexture->getTexture()->UnlockRect(0);
	}

	// create a normal map texture
	cTexture temp;
	temp.createTexture(
		m_tableWidth, 
		m_tableHeight, 
		1, 0, 
		D3DFMT_A8R8G8B8, 
		D3DPOOL_SCRATCH);

	// how much to scale the normals?
	float scale = 
		(m_tableWidth * m_worldExtents.sizeZ())
		/m_worldExtents.sizeX();

	// Use D3DX to convert our height map into a 
	// texture of surface normals
	temp.generateNormalMap(
		pTexture->getTexture(), 
		D3DX_CHANNEL_RED, 0, scale);

	// now read out the normals and 
	// store them in our intenal structure
	if(SUCCEEDED(temp.getTexture()->LockRect(
		0, &lockedRect, 0, D3DLOCK_READONLY)))
	{
		uint8* pNormalMap = 
			(uint8*)lockedRect.pBits;

		for(y=0; y<maxY; ++y)
		{
			for(x=0; x<maxX; ++x)
			{
				int index = 
					(y*lockedRect.Pitch)+(x*4);
				cVector3 normal;

				normal.z = pNormalMap[index+0] - 127.5f;
				normal.y = pNormalMap[index+1] - 127.5f;
				normal.x = pNormalMap[index+2] - 127.5f;

				normal.normalize();
				m_normalTable[(y*maxX)+x] = normal;
			}
		}

		temp.getTexture()->UnlockRect(0);
	}

	temp.releaseTexture();
}

float cTerrain::readWorldHeight(uint16 mapX, uint16 mapY)const
{
	if (mapX >= m_tableWidth) mapX = m_tableWidth-1;
	if (mapY >= m_tableHeight) mapY = m_tableHeight-1;

	return m_heightTable[(mapY*m_tableWidth)+mapX];
}

uint16 cTerrain::tableIndex(uint16 mapX, uint16 mapY)const
{
	// clamp to the table dimensions
	if (mapX >= m_tableWidth) mapX = m_tableWidth-1;
	if (mapY >= m_tableHeight) mapY = m_tableHeight-1;

	return (mapY*m_tableWidth)+mapX;
}

float cTerrain::readWorldHeight(uint16 mapIndex)const
{
	debug_assert(mapIndex<m_tableWidth*m_tableHeight, "invalid index");

	return m_heightTable[mapIndex];
}

const cVector3& cTerrain::readWorldNormal(uint16 mapX, uint16 mapY)const
{
	if (mapX >= m_tableWidth) mapX = m_tableWidth-1;
	if (mapY >= m_tableHeight) mapY = m_tableHeight-1;

	return m_normalTable[(mapY*m_tableWidth)+mapX];
}

float cTerrain::calcWorldHeight(float worldX, float worldY)const
{
	float fMapX = (worldX - m_worldExtents.x0) / m_worldExtents.sizeX();
	float fMapY = (worldY - m_worldExtents.y0) / m_worldExtents.sizeY();

	return calcMapHeight(fMapX, fMapY);
}

void cTerrain::calcWorldNormal(cVector3& normal, float worldX, float worldY)const
{
	float fMapX = (worldX - m_worldExtents.x0) / m_worldExtents.sizeX();
	float fMapY = (worldY - m_worldExtents.y0) / m_worldExtents.sizeY();

	calcMapNormal(normal, fMapX, fMapY);
}

float cTerrain::calcMapHeight(
	float mapX, 
	float mapY)const
{
	float fMapX = mapX * (m_tableWidth-1);
	float fMapY = mapY * (m_tableHeight-1);

	int iMapX0 = realToInt32_chop(fMapX);
	int iMapY0 = realToInt32_chop(fMapY);

	fMapX -= iMapX0;
	fMapY -= iMapY0;

	iMapX0 = clamp(iMapX0, 0, m_tableWidth-1);
	iMapY0 = clamp(iMapY0, 0, m_tableHeight-1);

	int iMapX1 = clamp(iMapX0+1, 0, m_tableWidth-1);
	int iMapY1 = clamp(iMapY0+1, 0, m_tableHeight-1);

	// read 4 map values
	float h0 = readWorldHeight(iMapX0, iMapY0);
	float h1 = readWorldHeight(iMapX1, iMapY0);
	float h2 = readWorldHeight(iMapX0, iMapY1);
	float h3 = readWorldHeight(iMapX1, iMapY1);

	float avgLo = (h1*fMapX) + (h0*(1.0f-fMapX));
	float avgHi = (h3*fMapX) + (h2*(1.0f-fMapX));

	return (avgHi*fMapY) + (avgLo*(1.0f-fMapY));;
}

void cTerrain::calcMapNormal(
	cVector3& normal, 
	float mapX, 
	float mapY)const
{
	float fMapX = mapX * (m_tableWidth-1);
	float fMapY = mapY * (m_tableHeight-1);

	int iMapX0 = realToInt32_chop(fMapX);
	int iMapY0 = realToInt32_chop(fMapY);

	fMapX -= iMapX0;
	fMapY -= iMapY0;

	iMapX0 = clamp(iMapX0, 0, m_tableWidth-1);
	iMapY0 = clamp(iMapY0, 0, m_tableHeight-1);

	int iMapX1 = clamp(iMapX0+1, 0, m_tableWidth-1);
	int iMapY1 = clamp(iMapY0+1, 0, m_tableHeight-1);

	// read 4 map values
	cVector3 h0 = readWorldNormal(iMapX0, iMapY0);
	cVector3 h1 = readWorldNormal(iMapX1, iMapY0);
	cVector3 h2 = readWorldNormal(iMapX0, iMapY1);
	cVector3 h3 = readWorldNormal(iMapX1, iMapY1);

	cVector3 avgLo = (h1*fMapX) + (h0*(1.0f-fMapX));
	cVector3 avgHi = (h3*fMapX) + (h2*(1.0f-fMapX));

	normal= (avgHi*fMapY) + (avgLo*(1.0f-fMapY));
	normal.normalize();
}


struct sample_data
{
	uint32 color;
	float scale;
};

void cTerrain::generateTerrainImage(
	cImage* pTerrainImage,
	terrainTextureData* pTextureList,
	int	textureCount)
{
	bool success = false;
	int x,y,i;

	int image_width = pTerrainImage->width();
	int image_height = pTerrainImage->height();
	
	float uStep = 1.0f/(image_width-1);
	float vStep = 1.0f/(image_height-1);

	sample_data* pSamples = new sample_data[textureCount];

	// lock all the textures we need
	pTerrainImage->lock();
	for (i=0; i<textureCount; ++i)
	{
		pTextureList[i].pImage->lock();
	}

	// step through and generate each pixel
	for (y=0; y<image_height; ++y)
	{
		for (x=0; x<image_width; ++x)
		{
			float u = x*uStep;
			float v = y*vStep;

			float totalBlend = 0.0f;
			float map_height = calcMapHeight(u,v);
			cVector3 normal;
			calcMapNormal(normal, u, v);

			// examine each elevation set
			for (i=0; i<textureCount; ++i)
			{
				// how much of this texture do we want?
				float elevationScale=0.0f;
				float slopeScale=0.0f;

				if (map_height >= pTextureList[i].elevation.minElevation
					&& map_height <= pTextureList[i].elevation.maxElevation)
				{
					float span = pTextureList[i].elevation.maxElevation - pTextureList[i].elevation.minElevation;
					elevationScale = map_height - pTextureList[i].elevation.minElevation;
					elevationScale *= 1.0f/span;
					elevationScale -= 0.5f;
					elevationScale *= 2.0f;
					elevationScale *= elevationScale;
					elevationScale = 1.0f-elevationScale;
				}

				if (normal.z >= pTextureList[i].elevation.minNormalZ
					&& normal.z <= pTextureList[i].elevation.maxNormalZ)
				{
					float span = pTextureList[i].elevation.maxNormalZ - pTextureList[i].elevation.minNormalZ;
					slopeScale = normal.z - pTextureList[i].elevation.minNormalZ;
					slopeScale *= 1.0f/span;
					slopeScale -= 0.5f;
					slopeScale *= 2.0f;
					slopeScale *= slopeScale;
					slopeScale = 1.0f - slopeScale;
				}

				pSamples[i].scale = pTextureList[i].elevation.strength * elevationScale * slopeScale;
				totalBlend += pSamples[i].scale;

				pTextureList[i].pImage->sampleColor(
					u*pTextureList[i].uvScale,
					v*pTextureList[i].uvScale,
					pSamples[i].color);
			}

			// balance the data (so they add up to 1.0f)
			float blendScale = 1.0f /totalBlend;

			// now compute the actual color
			float red = 0.0f;
			float green = 0.0f;
			float blue = 0.0f;
			float alpha = 0.0f;

			for (i=0; i<textureCount; ++i)
			{
				float scale = pSamples[i].scale*blendScale;

				blue	+= (pSamples[i].color & 0xff)*scale;
				green	+= ((pSamples[i].color>>8) & 0xff)*scale;
				red		+= ((pSamples[i].color>>16) & 0xff)*scale;
				alpha	+= ((pSamples[i].color>>24) & 0xff)*scale;
			}

			uint8 r = (uint8) clamp(red,0.0f, 255.0f);
			uint8 g = (uint8) clamp(green,0.0f, 255.0f);
			uint8 b = (uint8) clamp(blue,0.0f, 255.0f);
			uint8 a = (uint8) clamp(alpha,0.0f, 255.0f);

			uint32 color = (a<<24)+(r<<16)+(g<<8)+b;

			pTerrainImage->setColor(x,y,color);
		}
	}

	safe_delete_array(pSamples);


	// unlock all the images
	pTerrainImage->unlock();
	for (i=0; i<textureCount; ++i)
	{
		pTextureList[i].pImage->unlock();
	}
}



static float computeWeight(
	float value, 
	float minExtent, 
	float maxExtent)
{
	float weight = 0.0f;
	
	if (value >= minExtent
		&& value <= maxExtent)
	{

		float span = 
			maxExtent - minExtent;
		weight = 
			value - minExtent;
		
		// convert to a 0-1 range value
		// based on its distance to the midpoint
		// of the range extents
		weight *= 1.0f/span; 
		weight -= 0.5f;
		weight *= 2.0f;

		// square the result for non-linear falloff
		weight *= weight;

		// invert and bound-check the result
		weight = 1.0f-absoluteValue(weight);
		weight = clamp(weight, 0.001f, 1.0f);
	}

	return weight;
}

void cTerrain::generateBlendImage(
	cImage* pBlendImage,
	elevationData* pElevationData,
	int	elevationDataCount)
{
	bool success = false;
	int x,y,i;

	// make sure there are no more than 4 structures
	elevationDataCount = minimum(elevationDataCount, 4);
	
	// get the blend image dimensions
	int image_width = pBlendImage->width();
	int image_height = pBlendImage->height();

	// compute the step values for uv
	// coordinates across the image
	float uStep = 1.0f/(image_width-1);
	float vStep = 1.0f/(image_height-1);

	// these 4 mask values control 
	// which color component of the
	// blend image we write to
	cVector4 mask[4];
	mask[0].set(1.0f,0.0f,0.0f,0.0f);
	mask[1].set(0.0f,1.0f,0.0f,0.0f);
	mask[2].set(0.0f,0.0f,1.0f,0.0f);
	mask[3].set(0.0f,0.0f,0.0f,1.0f);

	// lock all the blend image
	pBlendImage->lock();

	// step through and generate each pixel
	for (y=0; y<image_height; ++y)
	{
		for (x=0; x<image_width; ++x)
		{
			float totalBlend = 0.0f;
			cVector4 blendFactors(
				0.0f,
				0.0f,
				0.0f,
				0.0f);

			// get the elevation and surface normal
			float u = x*uStep;
			float v = y*vStep;
			float map_height = calcMapHeight(u,v);
			cVector3 normal;
			calcMapNormal(normal, u, v);

			// examine each elevationData structure
			// a compute a weight for each one
			for (i=0; i<elevationDataCount; ++i)
			{
				// compute a weight based on elevation
				float elevationScale =
					computeWeight(
						map_height,
						pElevationData[i].minElevation,
						pElevationData[i].maxElevation);

				// compute a weight based on slope
				float slopeScale = 
					computeWeight(
						normal.z,
						pElevationData[i].minNormalZ,
						pElevationData[i].maxNormalZ);

				// combine the two with the relative
				// strength of this surface type
				float scale = 
					pElevationData[i].strength * 
					elevationScale * 
					slopeScale;

				// write the result to the proper
				// channel of the blend factor vector
				blendFactors += mask[i]*scale;

				// and remember the total weight
				totalBlend += scale;
			}

			// balance the data (so they add up to 255)
			float blendScale = 255.0f /totalBlend;

			// now compute the actual color by
			// multiplying each channel 
			// by the blend scale
			blendFactors *= blendScale;

			// clamp and convert to color values
			uint8 r = 
				(uint8) clamp(
					blendFactors.x, 
					0.0f, 
					255.0f);
			uint8 g = 
				(uint8) clamp(
					blendFactors.y, 
					0.0f, 
					255.0f);
			uint8 b = 
				(uint8) clamp(
					blendFactors.z, 
					0.0f, 
					255.0f);
			uint8 a = 
				(uint8) clamp(
					blendFactors.w, 
					0.0f, 
					255.0f);

			// pack and write a 32bit pixel value
			uint32 color = (a<<24)+(r<<16)+(g<<8)+b;
			pBlendImage->setColor(x,y,color);
		}
	}

	// unlock the image
	pBlendImage->unlock();
}


void cTerrain::renderSection(
	cTerrainSection* pSection, 
	u32Flags activationFlags,
	const cRenderEntry* pEntry)const
{
	cEffectFile* pEffectFile = 
		m_pRenderMethod->getActiveEffect();
	cSurfaceMaterial* pSurfaceMaterial = 
		m_pRenderMethod->getActiveMaterial();
	
	if (pEffectFile)
	{
		profile_scope(cTerrainSystem_renderSection);
		LPDIRECT3DDEVICE9 d3dDevice = 
			TheGameHost.d3dDevice();
		
		// do we need to activate the render method?
		if (TEST_BIT(activationFlags,
			k_activateRenderMethod))
		{
			pEffectFile->begin();
		}

		// do we need to activate the render pass?
		if (TEST_BIT(activationFlags,
			k_activateRenderMethodPass))
		{
			pEffectFile->activatePass(pEntry->renderPass);
		}

		// do we need to activate the primary vertex buffer
		if (TEST_BIT(activationFlags,
			k_activateModel))
		{
			m_pVertexGrid->activate(0,0, true);
		}

		// do we need to activate the secondary vertex buffer
		if (TEST_BIT(activationFlags,
			k_activateModelParamA))
		{
			pSection->sectorVertices()->activate(
				1,0, false);
		}

		// do we need to activate the index buffer
		if (TEST_BIT(activationFlags,
			k_activateModelParamB))
		{
			m_pTriangles->activate();
		}

		// do we need to activate the surface material
		if (TEST_BIT(activationFlags,
			k_activateSurfaceMaterial))
		{
			pEffectFile->applySurfaceMaterial(
				pSurfaceMaterial);
		}

		// apply our render settings to the method
		int sectorX = pSection->sectorX();
		int sectorY = pSection->sectorY();

		cVector4 sectorOffset(
			1.0f,
			1.0f,
			m_worldExtents.x0+(m_sectorSize.x*sectorX),
			m_worldExtents.y0+(m_sectorSize.y*sectorY));


		cVector4 uvScaleOffset(
			(float)1.0f/(m_sectorCountX+1),
			(float)1.0f/(m_sectorCountY+1),
			(float)sectorX,
			(float)sectorY);

		pEffectFile->setParameter(
			cEffectFile::k_posScaleOffset, 
			(D3DXVECTOR4*)&sectorOffset);

		pEffectFile->setParameter(
			cEffectFile::k_uvScaleOffset, 
			(D3DXVECTOR4*)&uvScaleOffset);
		
		// render!!!
		HRESULT hr = d3dDevice->DrawIndexedPrimitive(
			m_pTriangles->primitiveType(),
			0,
			0,
			m_sectorVerts*m_sectorVerts,
			0,
			m_pTriangles->primitiveCount());
	}
}

void cTerrain::submitSection(
	cTerrainSection* pSection)const
{
	cEffectFile* pEffectFile = 
		m_pRenderMethod->getActiveEffect();
	cSurfaceMaterial* pSurfaceMaterial = 
		m_pRenderMethod->getActiveMaterial();
	
	if (pEffectFile)
	{
		profile_scope(cTerrainSystem_submitSection);
		int total_passes = pEffectFile->totalPasses();

		// check the neighbor sectors for connection needs
		uint16 sX = pSection->sectorX();
		uint16 sY = pSection->sectorY();

		int index = (sY*m_sectorCountX)+sX;

		for (int iPass=0; iPass<total_passes; ++iPass)
		{
			cRenderEntry* pRenderEntry = 
				DisplayManager.openRenderQueue();
			
			pRenderEntry->hEffectFile = 
				(uint8)pEffectFile->resourceHandle();
			pRenderEntry->hSurfaceMaterial = 
				pSurfaceMaterial->resourceHandle();
			pRenderEntry->modelType = 
				cRenderEntry::k_bufferEntry;
			pRenderEntry->hModel = 
				m_pVertexGrid->resourceHandle();
			pRenderEntry->modelParamA = 
				pSection->sectorVertices()->resourceHandle();
			pRenderEntry->modelParamB = 
				m_pTriangles->resourceHandle();
			pRenderEntry->renderPass = 
				iPass;
			pRenderEntry->object = 
				(cSceneNode*)pSection;
			pRenderEntry->userData = 
				0;

			DisplayManager.closeRenderQueue(pRenderEntry);
		}
	}
}

void cTerrain::setTessellationParameters(float vScale, float vLimit)
{
	m_vScale = vScale;
	m_vLimit = vLimit;
}

float cTerrain::computeErrorMetricOfGrid(
	uint16 xVerts,	// width of grid
	uint16 yVerts,	// height of grid
	uint16 xStep,	// horz vertex count per cell
	uint16 yStep,	// vert vertex count per cell
	uint16 xOffset,	// starting index X
	uint16 yOffset)	// starting index Y
{
	float result = 0.0f;
	int total_rows = yVerts-1;
	int total_cells = xVerts-1;

	unsigned short start_vert = 
		(yOffset*m_tableWidth)+xOffset;
	unsigned short lineStep = 
		yStep*m_tableWidth;

	float invXStep = 1.0f/xStep;
	float invYStep = 1.0f/yStep;

	for (int j=0;j<total_rows;++j)
	{
		uint16 indexA = start_vert;
		uint16 indexB = start_vert+lineStep;
		float cornerA = readWorldHeight(indexA);
		float cornerB = readWorldHeight(indexB);

		for (int i=0; i<total_cells;++i)
		{
			// compute 2 new corner vertices
			uint16 indexC = indexA+xStep;
			uint16 indexD = indexB+xStep;

			// grab 2 new corner height values
			float cornerC = readWorldHeight(indexC);
			float cornerD = readWorldHeight(indexD);

			// setup the step values for 
			// both triangles of this cell
			float stepX0 = (cornerD-cornerA)*invXStep;
			float stepY0 = (cornerB-cornerA)*invYStep;
			float stepX1 = (cornerB-cornerC)*invXStep;
			float stepY1 = (cornerD-cornerC)*invYStep;

			// find the max error for all points
			// covered by the two triangles
			int subIndex = indexA;
			for (int y=0; y<yStep;++y)
			{
				for (int x=0; x<xStep;++x)
				{
					float trueHeight = 
						readWorldHeight(subIndex);
					++subIndex;

					float intepolatedHeight;

					if (y < (xStep-x))
					{
						intepolatedHeight = 
							cornerA 
							+ (stepX0*x)
							+ (stepY0*y);
					}
					else
					{
						intepolatedHeight = 
							cornerC 
							+ (stepX1*x)
							+ (stepY1*y);
					}

					float delta = absoluteValue(
						trueHeight - intepolatedHeight);

					result = maximum(
						result,delta);

				}
				subIndex = indexA+(y*m_tableWidth);
			}

			// save the corners for the next cell
			indexA = indexC;
			indexB = indexD;
			cornerA = cornerC;
			cornerB = cornerD;
		}

		start_vert += lineStep;
	}

	return result;
}

bool cTerrain::buildVertexBuffer()
{
	cString tempName;
	tempName.format("terrain_system_%i", this);

	// create the vertex buffer
	// shared by the sectors
	m_pVertexGrid = 
		DisplayManager.vertexBufferPool().
		createResource(tempName);

	cVector2 cellSize(
		m_sectorSize.x/m_sectorUnits,
		m_sectorSize.y/m_sectorUnits);

	cVector2 vert(0.0f,0.0f);
	sLocalVertex* pVerts = 
		new sLocalVertex[m_sectorVerts*m_sectorVerts];

	// fill the vertex stream with x,y positions and
	// uv coordinates. All other data (height and
	// surface normals) are stored in the vertex
	// buffers of each terrain section
	for (int y=0; y<m_sectorVerts; ++y)
	{
		vert.set(0.0f, y*cellSize.y);

		for (int x=0; x<m_sectorVerts; ++x)
		{
			pVerts[(y*m_sectorVerts)+x].xyPosition = vert;
			pVerts[(y*m_sectorVerts)+x].localUV.set(
				(float)x/(float)(m_sectorVerts-1),
				(float)y/(float)(m_sectorVerts-1));

			vert.x += cellSize.x;
		}
	}

	// now that we have built the data,
	// create one of our vertex buffer
	// resource objects with it
	bool result = SUCCEEDED(
		m_pVertexGrid->create(
		m_sectorVerts*m_sectorVerts, 
		sizeof(sLocalVertex), 
		0, pVerts));

	safe_delete_array(pVerts);

	return result;
}

bool cTerrain::setVertexDescription()
{
	// create the vertex declaration
	// and add it to the vertex
	// buffer containing our basic grid
	bool success = 
	m_pVertexGrid->setVertexDescription(
		sizeof(vertex_description)/sizeof(D3DVERTEXELEMENT9),
		vertex_description);

	return success;
}

bool cTerrain::buildIndexBuffer()
{
	cString tempName;
	tempName.format("terrain_system_%i", this);

	m_pTriangles = 
		DisplayManager.indexBufferPool().
		createResource(tempName);

	// create the index buffer which
	// all terrain sections can share
	return SUCCEEDED(
		m_pTriangles->createSingleStripGrid(
		m_sectorVerts,	// width of grid
		m_sectorVerts,	// height of grid
		1,	// horz vertex count per cell
		1,	// vert vertex count per cell
		m_sectorVerts,	// horz vertex count in vbuffer
		0));
}

void cTerrain::readUserInput()
{
	//
	// allow the user to adjust tesselation params
	//
	static float vRatioLimit = 0.03f;
	static float vErrorScale = 1.33f;

	if (GetAsyncKeyState(VK_ADD) & 0x8000) 
	{
		vRatioLimit += 0.001f;
		debug_print("vRatioLimit: %.02f\n", vRatioLimit);
	}
	else if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
	{
		vRatioLimit -= 0.001f;
		debug_print("vRatioLimit: %.02f\n", vRatioLimit);
	}

	if (GetAsyncKeyState(VK_MULTIPLY) & 0x8000) 
	{
		vErrorScale += 0.001f;
		debug_print("vErrorScale: %.02f\n", vErrorScale);
	}
	else if (GetAsyncKeyState(VK_DIVIDE) & 0x8000)
	{
		vErrorScale -= 0.001f;
		debug_print("vErrorScale: %.02f\n", vErrorScale);
	}

	if (vRatioLimit < 0.001f) vRatioLimit = 0.001f;
	if (vErrorScale < 0.001f) vErrorScale = 0.001f;

	setTessellationParameters(
		vErrorScale, vRatioLimit);
}
//***************************************************************
// end of file      ( cTerrain.cpp )

//----------------------------------------------------------
//$Log: $