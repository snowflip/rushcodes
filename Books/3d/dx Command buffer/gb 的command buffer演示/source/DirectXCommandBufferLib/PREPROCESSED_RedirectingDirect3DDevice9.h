
	 // EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION

// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not be copied
// or disclosed except in accordance with the terms of that agreement. Copyright
// (c) 1996-2008 Emergent Game Technologies. All Rights Reserved. Emergent Game
// Technologies, Chapel Hill, North Carolina 27517 http://www.emergent.net
#pragma once

// =====================================================================================================================
//    Redirecting Macros
// =====================================================================================================================
class RedirectingDirect3DDevice9 :
	public IDirect3DDevice9
{
/* ---------------------------------------------------------------------------------------------------------------------
 ----------------------------------------------------------------------------------------------------------------------- */
public:
	IDirect3D9			*pRealDirect3D9;
	IDirect3DDevice9	*pCurrentDevice;
	IDirect3DDevice9	*pRealDevice;
	bool				bIsReal;
	bool				bTrace;

	/* =================================================================================================================
	 =================================================================================================================== */
	RedirectingDirect3DDevice9()
	{
		pCurrentDevice = NULL;
		pRealDirect3D9 = NULL;
		pRealDevice = NULL;
		bIsReal = false;
		bTrace = false;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	~RedirectingDirect3DDevice9()
	{
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	void SetTrace(bool bval)
	{
		bTrace = bval;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	bool GetTrace()
	{
		return bTrace;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	void ReplaceCurrentDevice(IDirect3DDevice9 *pNewDevice)
	{
		pCurrentDevice = pNewDevice;
		bIsReal = false;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	void RestoreCurrentDevice()
	{
		pCurrentDevice = pRealDevice;
		bIsReal = true;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	IDirect3DDevice9 *GetCurrentDevice()
	{
		return pCurrentDevice;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	IDirect3DDevice9 *GetRealDevice()
	{
		return pRealDevice;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	bool IsCurrentDeviceReal()
	{
		return bIsReal;
	}

	/*
	 * IUnknown methods ;
	 * if !BUILDTRACE STDREDIRFUNC2( QueryInterface, REFIID, riid, void**, ppvObj);
	 * #else
	 */
	STDMETHOD_ (HRESULT, QueryInterface) (REFIID riid, void **ppvObj)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "QueryInterface", "void**", (DWORD) ppvObj);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->QueryInterface(riid, ppvObj);
	};

	// endif
	STDMETHOD_ (ULONG, AddRef) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "ULONG", "AddRef");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->AddRef();
	};
	STDMETHOD_ (ULONG, Release) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "ULONG", "Release");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->Release();
	};

	/*
	 * Implemnted IDirect3DDevice9 methods ;
	 * Note that it is assumed that input values will be valid a record time. ;
	 * Also there is no mechnism to retain returned values during playback
	 */
	STDMETHOD_ (HRESULT, DrawPrimitive) (D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "DrawPrimitive",
						"D3DPRIMITIVETYPE", (DWORD) PrimitiveType, "UINT", (DWORD) StartVertex, "UINT",
							(DWORD) PrimitiveCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
	};
	STDMETHOD_ (HRESULT, DrawIndexedPrimitive)
		(
			D3DPRIMITIVETYPE PrimitiveType,
			INT BaseVertexIndex,
			UINT MinVertexIndex,
			UINT NumVertices,
			UINT startIndex,
			UINT primCount
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT",
						"DrawIndexedPrimitive", "D3DPRIMITIVETYPE", (DWORD) PrimitiveType, "INT",
							(DWORD) BaseVertexIndex, "UINT", (DWORD) MinVertexIndex, "UINT",
								(DWORD) NumVertices, "UINT", (DWORD) startIndex, "UINT", (DWORD) primCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	};
	STDMETHOD_ (HRESULT, SetStreamSource)
		(
			UINT StreamNumber,
			IDirect3DVertexBuffer9 * pStreamData,
			UINT OffsetInBytes,
			UINT Stride
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetStreamSource",
						"UINT", (DWORD) StreamNumber, "IDirect3DVertexBuffer9*", (DWORD) pStreamData, "UINT",
							(DWORD) OffsetInBytes, "UINT", (DWORD) Stride
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
	};
	STDMETHOD_ (HRESULT, SetStreamSourceFreq) (UINT StreamNumber, UINT Divider)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetStreamSourceFreq", "UINT",
						(DWORD) StreamNumber, "UINT", (DWORD) Divider
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetStreamSourceFreq(StreamNumber, Divider);
	};
	STDMETHOD_ (HRESULT, SetVertexDeclaration) (IDirect3DVertexDeclaration9 * pDecl)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetVertexDeclaration",
						"IDirect3DVertexDeclaration9*", (DWORD) pDecl
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexDeclaration(pDecl);
	};
	STDMETHOD_ (HRESULT, SetIndices) (IDirect3DIndexBuffer9 * pIndexData)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetIndices", "IDirect3DIndexBuffer9*",
						(DWORD) pIndexData
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetIndices(pIndexData);
	};
	STDMETHOD_ (HRESULT, TestCooperativeLevel) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "HRESULT", "TestCooperativeLevel");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->TestCooperativeLevel();
	};
	STDMETHOD_ (HRESULT, EvictManagedResources) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "HRESULT", "EvictManagedResources");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->EvictManagedResources();
	};
	STDMETHOD_ (HRESULT, BeginScene) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "HRESULT", "BeginScene");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->BeginScene();
	};
	STDMETHOD_ (HRESULT, EndScene) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "HRESULT", "EndScene");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->EndScene();
	};
	STDMETHOD_ (HRESULT, BeginStateBlock) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "HRESULT", "BeginStateBlock");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->BeginStateBlock();
	};
	STDMETHOD_ (HRESULT, EndStateBlock) (IDirect3DStateBlock9 **ppSB)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "EndStateBlock", "IDirect3DStateBlock9**",
						(DWORD) ppSB
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->EndStateBlock(ppSB);
	};
	STDMETHOD_ (HRESULT, GetDirect3D) (IDirect3D9 **ppD3D9)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetDirect3D", "IDirect3D9**", (DWORD) ppD3D9);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetDirect3D(ppD3D9);
	};

	/*
	 * These methods can be recorded but without a way to retain the return value I do
	 * not see why your would use them
	 */
	STDMETHOD_ (HRESULT, SetCursorProperties) (UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetCursorProperties", "UINT",
						(DWORD) XHotSpot, "UINT", (DWORD) YHotSpot, "IDirect3DSurface9*", (DWORD) pCursorBitmap
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
	};
	STDMETHOD_ (void, SetCursorPosition) (int X, int Y, DWORD Flags)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "void", "SetCursorPosition", "int",
						(DWORD) X, "int", (DWORD) Y, "DWORD", (DWORD) Flags
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetCursorPosition(X, Y, Flags);
	};
	STDMETHOD_ (HRESULT, SetDialogBoxMode) (BOOL bEnableDialogs)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetDialogBoxMode", "BOOL", (DWORD) bEnableDialogs);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetDialogBoxMode(bEnableDialogs);
	};
	STDMETHOD_ (HRESULT, GetDeviceCaps) (D3DCAPS9 * pCaps)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetDeviceCaps", "D3DCAPS9*", (DWORD) pCaps);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetDeviceCaps(pCaps);
	};
	STDMETHOD_ (HRESULT, GetDisplayMode) (UINT iSwapChain, D3DDISPLAYMODE * pMode)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetDisplayMode", "UINT",
						(DWORD) iSwapChain, "D3DDISPLAYMODE*", (DWORD) pMode
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetDisplayMode(iSwapChain, pMode);
	};
	STDMETHOD_ (HRESULT, GetCreationParameters) (D3DDEVICE_CREATION_PARAMETERS * pParameters)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetCreationParameters",
						"D3DDEVICE_CREATION_PARAMETERS*", (DWORD) pParameters
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetCreationParameters(pParameters);
	};
	STDMETHOD_ (HRESULT, CreateAdditionalSwapChain)
		(
			D3DPRESENT_PARAMETERS * pPresentationParameters,
			IDirect3DSwapChain9 **pSwapChain
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "CreateAdditionalSwapChain",
						"D3DPRESENT_PARAMETERS*", (DWORD) pPresentationParameters, "IDirect3DSwapChain9**",
							(DWORD) pSwapChain
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
	};
	STDMETHOD_ (HRESULT, GetSwapChain) (UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetSwapChain", "UINT",
						(DWORD) iSwapChain, "IDirect3DSwapChain9**", (DWORD) pSwapChain
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetSwapChain(iSwapChain, pSwapChain);
	};
	STDMETHOD_ (HRESULT, Reset) (D3DPRESENT_PARAMETERS * pPresentationParameters)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "Reset", "D3DPRESENT_PARAMETERS*",
						(DWORD) pPresentationParameters
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->Reset(pPresentationParameters);
	};
	STDMETHOD_ (HRESULT, Present)
		(
			CONST RECT * pSourceRect,
			CONST RECT * pDestRect,
			HWND hDestWindowOverride,
			CONST RGNDATA * pDirtyRegion
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "Present",
						"CONST RECT*", (DWORD) pSourceRect, "CONST RECT*", (DWORD) pDestRect, "HWND",
							(DWORD) hDestWindowOverride, "CONST RGNDATA*", (DWORD) pDirtyRegion
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	};
	STDMETHOD_ (HRESULT, GetBackBuffer)
		(
			UINT iSwapChain,
			UINT iBackBuffer,
			D3DBACKBUFFER_TYPE Type,
			IDirect3DSurface9 **ppBackBuffer
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetBackBuffer", "UINT",
						(DWORD) iSwapChain, "UINT", (DWORD) iBackBuffer, "D3DBACKBUFFER_TYPE",
							(DWORD) Type, "IDirect3DSurface9**", (DWORD) ppBackBuffer
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
	};
	STDMETHOD_ (HRESULT, GetRasterStatus) (UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetRasterStatus", "UINT",
						(DWORD) iSwapChain, "D3DRASTER_STATUS*", (DWORD) pRasterStatus
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetRasterStatus(iSwapChain, pRasterStatus);
	};
	STDMETHOD_ (void, SetGammaRamp) (UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "void", "SetGammaRamp", "UINT",
						(DWORD) iSwapChain, "DWORD", (DWORD) Flags, "CONST D3DGAMMARAMP*", (DWORD) pRamp
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
	};
	STDMETHOD_ (void, GetGammaRamp) (UINT iSwapChain, D3DGAMMARAMP * pRamp)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "void", "GetGammaRamp", "UINT",
						(DWORD) iSwapChain, "D3DGAMMARAMP*", (DWORD) pRamp
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetGammaRamp(iSwapChain, pRamp);
	};
	STDMETHOD_ (HRESULT, CreateTexture)
		(
			UINT Width,
			UINT Height,
			UINT Levels,
			DWORD Usage,
			D3DFORMAT Format,
			D3DPOOL Pool,
			IDirect3DTexture9 **ppTexture,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699,
						"%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n",
							"HRESULT", "CreateTexture", "UINT", (DWORD) Width, "UINT", (DWORD) Height, "UINT",
								(DWORD) Levels, "DWORD", (DWORD) Usage, "D3DFORMAT", (DWORD) Format, "D3DPOOL",
									(DWORD) Pool, "IDirect3DTexture9**", (DWORD) ppTexture, "HANDLE*",
										(DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, CreateVolumeTexture)
		(
			UINT Width,
			UINT Height,
			UINT Depth,
			UINT Levels,
			DWORD Usage,
			D3DFORMAT Format,
			D3DPOOL Pool,
			IDirect3DVolumeTexture9 **ppVolumeTexture,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699,
						"%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n",
						"HRESULT",
						"CreateVolumeTexture",
						"UINT",
						(DWORD) Width, "UINT", (DWORD) Height, "UINT", (DWORD) Depth, "UINT", (DWORD) Levels, "DWORD",
							(DWORD) Usage, "D3DFORMAT", (DWORD) Format, "D3DPOOL",
								(DWORD) Pool, "IDirect3DVolumeTexture9**", (DWORD) ppVolumeTexture, "HANDLE*",
									(DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, CreateCubeTexture)
		(
			UINT EdgeLength,
			UINT Levels,
			DWORD Usage,
			D3DFORMAT Format,
			D3DPOOL Pool,
			IDirect3DCubeTexture9 **ppCubeTexture,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n",
						"HRESULT", "CreateCubeTexture", "UINT", (DWORD) EdgeLength, "UINT", (DWORD) Levels, "DWORD",
							(DWORD) Usage, "D3DFORMAT", (DWORD) Format, "D3DPOOL",
								(DWORD) Pool, "IDirect3DCubeTexture9**", (DWORD) ppCubeTexture, "HANDLE*",
									(DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, CreateVertexBuffer)
		(
			UINT Length,
			DWORD Usage,
			DWORD FVF,
			D3DPOOL Pool,
			IDirect3DVertexBuffer9 **ppVertexBuffer,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT",
						"CreateVertexBuffer", "UINT", (DWORD) Length, "DWORD", (DWORD) Usage, "DWORD",
							(DWORD) FVF, "D3DPOOL", (DWORD) Pool, "IDirect3DVertexBuffer9**",
								(DWORD) ppVertexBuffer, "HANDLE*", (DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, CreateIndexBuffer)
		(
			UINT Length,
			DWORD Usage,
			D3DFORMAT Format,
			D3DPOOL Pool,
			IDirect3DIndexBuffer9 **ppIndexBuffer,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT",
						"CreateIndexBuffer", "UINT", (DWORD) Length, "DWORD", (DWORD) Usage, "D3DFORMAT",
							(DWORD) Format, "D3DPOOL", (DWORD) Pool, "IDirect3DIndexBuffer9**",
								(DWORD) ppIndexBuffer, "HANDLE*", (DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, CreateRenderTarget)
		(
			UINT Width,
			UINT Height,
			D3DFORMAT Format,
			D3DMULTISAMPLE_TYPE MultiSample,
			DWORD MultisampleQuality,
			BOOL Lockable,
			IDirect3DSurface9 **ppSurface,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699,
						"%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n",
							"HRESULT", "CreateRenderTarget", "UINT", (DWORD) Width, "UINT", (DWORD) Height, "D3DFORMAT",
								(DWORD) Format, "D3DMULTISAMPLE_TYPE", (DWORD) MultiSample, "DWORD",
									(DWORD) MultisampleQuality, "BOOL", (DWORD) Lockable, "IDirect3DSurface9**",
										(DWORD) ppSurface, "HANDLE*", (DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, CreateDepthStencilSurface)
		(
			UINT Width,
			UINT Height,
			D3DFORMAT Format,
			D3DMULTISAMPLE_TYPE MultiSample,
			DWORD MultisampleQuality,
			BOOL Discard,
			IDirect3DSurface9 **ppSurface,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699,
						"%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n",
							"HRESULT", "CreateDepthStencilSurface", "UINT", (DWORD) Width, "UINT",
								(DWORD) Height, "D3DFORMAT", (DWORD) Format, "D3DMULTISAMPLE_TYPE",
									(DWORD) MultiSample, "DWORD", (DWORD) MultisampleQuality, "BOOL",
										(DWORD) Discard, "IDirect3DSurface9**", (DWORD) ppSurface, "HANDLE*",
											(DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, UpdateSurface)
		(
			IDirect3DSurface9 * pSourceSurface,
			CONST RECT * pSourceRect,
			IDirect3DSurface9 * pDestinationSurface,
			CONST POINT * pDestPoint
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "UpdateSurface",
						"IDirect3DSurface9*", (DWORD) pSourceSurface, "CONST RECT*",
							(DWORD) pSourceRect, "IDirect3DSurface9*", (DWORD) pDestinationSurface, "CONST POINT*",
								(DWORD) pDestPoint
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
	};
	STDMETHOD_ (HRESULT, UpdateTexture)
		(
			IDirect3DBaseTexture9 * pSourceTexture,
			IDirect3DBaseTexture9 * pDestinationTexture
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "UpdateTexture", "IDirect3DBaseTexture9*",
						(DWORD) pSourceTexture, "IDirect3DBaseTexture9*", (DWORD) pDestinationTexture
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
	};
	STDMETHOD_ (HRESULT, GetRenderTargetData) (IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetRenderTargetData", "IDirect3DSurface9*",
						(DWORD) pRenderTarget, "IDirect3DSurface9*", (DWORD) pDestSurface
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
	};
	STDMETHOD_ (HRESULT, GetFrontBufferData) (UINT iSwapChain, IDirect3DSurface9 * pDestSurface)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetFrontBufferData", "UINT",
						(DWORD) iSwapChain, "IDirect3DSurface9*", (DWORD) pDestSurface
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetFrontBufferData(iSwapChain, pDestSurface);
	};
	STDMETHOD_ (HRESULT, StretchRect)
		(
			IDirect3DSurface9 * pSourceSurface,
			CONST RECT * pSourceRect,
			IDirect3DSurface9 * pDestSurface,
			CONST RECT * pDestRect,
			D3DTEXTUREFILTERTYPE Filter
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "StretchRect",
						"IDirect3DSurface9*", (DWORD) pSourceSurface, "CONST RECT*",
							(DWORD) pSourceRect, "IDirect3DSurface9*", (DWORD) pDestSurface, "CONST RECT*",
								(DWORD) pDestRect, "D3DTEXTUREFILTERTYPE", (DWORD) Filter
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
	};
	STDMETHOD_ (HRESULT, ColorFill) (IDirect3DSurface9 * pSurface, CONST RECT * pRect, D3DCOLOR color)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "ColorFill", "IDirect3DSurface9*",
						(DWORD) pSurface, "CONST RECT*", (DWORD) pRect, "D3DCOLOR", (DWORD) color
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->ColorFill(pSurface, pRect, color);
	};
	STDMETHOD_ (HRESULT, CreateOffscreenPlainSurface)
		(
			UINT Width,
			UINT Height,
			D3DFORMAT Format,
			D3DPOOL Pool,
			IDirect3DSurface9 **ppSurface,
			HANDLE * pSharedHandle
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT",
						"CreateOffscreenPlainSurface", "UINT", (DWORD) Width, "UINT", (DWORD) Height, "D3DFORMAT",
							(DWORD) Format, "D3DPOOL", (DWORD) Pool, "IDirect3DSurface9**",
								(DWORD) ppSurface, "HANDLE*", (DWORD) pSharedHandle
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
	};
	STDMETHOD_ (HRESULT, SetRenderTarget) (DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetRenderTarget", "DWORD",
						(DWORD) RenderTargetIndex, "IDirect3DSurface9*", (DWORD) pRenderTarget
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
	};
	STDMETHOD_ (HRESULT, GetRenderTarget) (DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetRenderTarget", "DWORD",
						(DWORD) RenderTargetIndex, "IDirect3DSurface9**", (DWORD) ppRenderTarget
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
	};
	STDMETHOD_ (HRESULT, SetDepthStencilSurface) (IDirect3DSurface9 * pNewZStencil)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetDepthStencilSurface", "IDirect3DSurface9*",
						(DWORD) pNewZStencil
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetDepthStencilSurface(pNewZStencil);
	};
	STDMETHOD_ (HRESULT, GetDepthStencilSurface) (IDirect3DSurface9 **ppZStencilSurface)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetDepthStencilSurface", "IDirect3DSurface9**",
						(DWORD) ppZStencilSurface
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetDepthStencilSurface(ppZStencilSurface);
	};
	STDMETHOD_ (HRESULT, Clear)
		(
			DWORD Count,
			CONST D3DRECT * pRects,
			DWORD Flags,
			D3DCOLOR Color,
			float Z,
			DWORD Stencil
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT",
						"Clear", "DWORD", (DWORD) Count, "CONST D3DRECT*", (DWORD) pRects, "DWORD",
							(DWORD) Flags, "D3DCOLOR", (DWORD) Color, "float", (DWORD) Z, "DWORD", (DWORD) Stencil
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
	};
	STDMETHOD_ (HRESULT, SetTransform) (D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetTransform", "D3DTRANSFORMSTATETYPE",
						(DWORD) State, "CONST D3DMATRIX*", (DWORD) pMatrix
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetTransform(State, pMatrix);
	};
	STDMETHOD_ (HRESULT, GetTransform) (D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetTransform", "D3DTRANSFORMSTATETYPE",
						(DWORD) State, "D3DMATRIX*", (DWORD) pMatrix
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetTransform(State, pMatrix);
	};
	STDMETHOD_ (HRESULT, MultiplyTransform) (D3DTRANSFORMSTATETYPE type, CONST D3DMATRIX * pMatrix)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "MultiplyTransform",
						"D3DTRANSFORMSTATETYPE", (DWORD) type, "CONST D3DMATRIX*", (DWORD) pMatrix
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->MultiplyTransform(type, pMatrix);
	};
	STDMETHOD_ (HRESULT, SetViewport) (CONST D3DVIEWPORT9 * pViewport)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetViewport", "CONST D3DVIEWPORT9*",
						(DWORD) pViewport
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetViewport(pViewport);
	};
	STDMETHOD_ (HRESULT, GetViewport) (D3DVIEWPORT9 * pViewport)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetViewport", "D3DVIEWPORT9*", (DWORD) pViewport);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetViewport(pViewport);
	};
	STDMETHOD_ (HRESULT, SetMaterial) (CONST D3DMATERIAL9 * pMaterial)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetMaterial", "CONST D3DMATERIAL9*",
						(DWORD) pMaterial
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetMaterial(pMaterial);
	};
	STDMETHOD_ (HRESULT, GetMaterial) (D3DMATERIAL9 * pMaterial)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetMaterial", "D3DMATERIAL9*", (DWORD) pMaterial);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetMaterial(pMaterial);
	};
	STDMETHOD_ (HRESULT, SetLight) (DWORD Index, CONST D3DLIGHT9 * pLight)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetLight", "DWORD",
						(DWORD) Index, "CONST D3DLIGHT9*", (DWORD) pLight
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetLight(Index, pLight);
	};
	STDMETHOD_ (HRESULT, GetLight) (DWORD Index, D3DLIGHT9 * pLight)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetLight", "DWORD",
						(DWORD) Index, "D3DLIGHT9*", (DWORD) pLight
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetLight(Index, pLight);
	};
	STDMETHOD_ (HRESULT, LightEnable) (DWORD Index, BOOL Enable)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "LightEnable", "DWORD",
						(DWORD) Index, "BOOL", (DWORD) Enable
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->LightEnable(Index, Enable);
	};
	STDMETHOD_ (HRESULT, GetLightEnable) (DWORD Index, BOOL * pEnable)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetLightEnable", "DWORD",
						(DWORD) Index, "BOOL*", (DWORD) pEnable
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetLightEnable(Index, pEnable);
	};
	STDMETHOD_ (HRESULT, SetClipPlane) (DWORD Index, CONST float *pPlane)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetClipPlane", "DWORD",
						(DWORD) Index, "CONST float*", (DWORD) pPlane
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetClipPlane(Index, pPlane);
	};
	STDMETHOD_ (HRESULT, GetClipPlane) (DWORD Index, float *pPlane)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetClipPlane", "DWORD",
						(DWORD) Index, "float*", (DWORD) pPlane
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetClipPlane(Index, pPlane);
	};
	STDMETHOD_ (HRESULT, SetRenderState) (D3DRENDERSTATETYPE State, DWORD Value)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetRenderState", "D3DRENDERSTATETYPE",
						(DWORD) State, "DWORD", (DWORD) Value
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetRenderState(State, Value);
	};
	STDMETHOD_ (HRESULT, GetRenderState) (D3DRENDERSTATETYPE State, DWORD * pValue)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetRenderState", "D3DRENDERSTATETYPE",
						(DWORD) State, "DWORD*", (DWORD) pValue
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetRenderState(State, pValue);
	};
	STDMETHOD_ (HRESULT, CreateStateBlock) (D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "CreateStateBlock", "D3DSTATEBLOCKTYPE",
						(DWORD) Type, "IDirect3DStateBlock9**", (DWORD) ppSB
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateStateBlock(Type, ppSB);
	};
	STDMETHOD_ (HRESULT, SetClipStatus) (CONST D3DCLIPSTATUS9 * pClipStatus)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetClipStatus", "CONST D3DCLIPSTATUS9*",
						(DWORD) pClipStatus
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetClipStatus(pClipStatus);
	};
	STDMETHOD_ (HRESULT, GetClipStatus) (D3DCLIPSTATUS9 * pClipStatus)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetClipStatus", "D3DCLIPSTATUS9*",
						(DWORD) pClipStatus
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetClipStatus(pClipStatus);
	};
	STDMETHOD_ (HRESULT, GetTexture) (DWORD Stage, IDirect3DBaseTexture9 **ppTexture)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetTexture", "DWORD",
						(DWORD) Stage, "IDirect3DBaseTexture9**", (DWORD) ppTexture
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetTexture(Stage, ppTexture);
	};
	STDMETHOD_ (HRESULT, SetTexture) (DWORD Stage, IDirect3DBaseTexture9 * pTexture)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetTexture", "DWORD",
						(DWORD) Stage, "IDirect3DBaseTexture9*", (DWORD) pTexture
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetTexture(Stage, pTexture);
	};
	STDMETHOD_ (HRESULT, GetTextureStageState) (DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetTextureStageState", "DWORD",
						(DWORD) Stage, "D3DTEXTURESTAGESTATETYPE", (DWORD) Type, "DWORD*", (DWORD) pValue
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetTextureStageState(Stage, Type, pValue);
	};
	STDMETHOD_ (HRESULT, SetTextureStageState) (DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetTextureStageState", "DWORD",
						(DWORD) Stage, "D3DTEXTURESTAGESTATETYPE", (DWORD) Type, "DWORD", (DWORD) Value
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetTextureStageState(Stage, Type, Value);
	};
	STDMETHOD_ (HRESULT, GetSamplerState) (DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetSamplerState", "DWORD",
						(DWORD) Sampler, "D3DSAMPLERSTATETYPE", (DWORD) Type, "DWORD*", (DWORD) pValue
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetSamplerState(Sampler, Type, pValue);
	};
	STDMETHOD_ (HRESULT, SetSamplerState) (DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetSamplerState", "DWORD",
						(DWORD) Sampler, "D3DSAMPLERSTATETYPE", (DWORD) Type, "DWORD", (DWORD) Value
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetSamplerState(Sampler, Type, Value);
	};
	STDMETHOD_ (HRESULT, ValidateDevice) (DWORD * pNumPasses)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "ValidateDevice", "DWORD*", (DWORD) pNumPasses);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->ValidateDevice(pNumPasses);
	};
	STDMETHOD_ (HRESULT, SetPaletteEntries) (UINT PaletteNumber, CONST PALETTEENTRY * pEntries)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetPaletteEntries", "UINT",
						(DWORD) PaletteNumber, "CONST PALETTEENTRY*", (DWORD) pEntries
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPaletteEntries(PaletteNumber, pEntries);
	};
	STDMETHOD_ (HRESULT, GetPaletteEntries) (UINT PaletteNumber, PALETTEENTRY * pEntries)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetPaletteEntries", "UINT",
						(DWORD) PaletteNumber, "PALETTEENTRY*", (DWORD) pEntries
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetPaletteEntries(PaletteNumber, pEntries);
	};
	STDMETHOD_ (HRESULT, SetCurrentTexturePalette) (UINT PaletteNumber)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetCurrentTexturePalette", "UINT",
						(DWORD) PaletteNumber
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetCurrentTexturePalette(PaletteNumber);
	};
	STDMETHOD_ (HRESULT, GetCurrentTexturePalette) (UINT * PaletteNumber)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetCurrentTexturePalette", "UINT*",
						(DWORD) PaletteNumber
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetCurrentTexturePalette(PaletteNumber);
	};
	STDMETHOD_ (HRESULT, SetScissorRect) (CONST RECT * pRect)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetScissorRect", "CONST RECT*", (DWORD) pRect);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetScissorRect(pRect);
	};
	STDMETHOD_ (HRESULT, GetScissorRect) (RECT * pRect)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetScissorRect", "RECT*", (DWORD) pRect);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetScissorRect(pRect);
	};
	STDMETHOD_ (HRESULT, SetSoftwareVertexProcessing) (BOOL bSoftware)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetSoftwareVertexProcessing", "BOOL",
						(DWORD) bSoftware
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetSoftwareVertexProcessing(bSoftware);
	};
	STDMETHOD_ (HRESULT, SetNPatchMode) (float nSegments)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetNPatchMode", "float", (DWORD) nSegments);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetNPatchMode(nSegments);
	};
	STDMETHOD_ (HRESULT, DrawPrimitiveUP)
		(
			D3DPRIMITIVETYPE PrimitiveType,
			UINT PrimitiveCount,
			CONST void *pVertexStreamZeroData,
			UINT VertexStreamZeroStride
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "DrawPrimitiveUP",
						"D3DPRIMITIVETYPE", (DWORD) PrimitiveType, "UINT", (DWORD) PrimitiveCount, "CONST void*",
							(DWORD) pVertexStreamZeroData, "UINT", (DWORD) VertexStreamZeroStride
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	};
	STDMETHOD_ (HRESULT, DrawIndexedPrimitiveUP)
		(
			D3DPRIMITIVETYPE PrimitiveType,
			UINT MinVertexIndex,
			UINT NumVertices,
			UINT PrimitiveCount,
			CONST void *pIndexData,
			D3DFORMAT IndexDataFormat,
			CONST void *pVertexStreamZeroData,
			UINT VertexStreamZeroStride
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699,
						"%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n",
							"HRESULT", "DrawIndexedPrimitiveUP", "D3DPRIMITIVETYPE", (DWORD) PrimitiveType, "UINT",
								(DWORD) MinVertexIndex, "UINT", (DWORD) NumVertices, "UINT",
									(DWORD) PrimitiveCount, "CONST void*", (DWORD) pIndexData, "D3DFORMAT",
										(DWORD) IndexDataFormat, "CONST void*", (DWORD) pVertexStreamZeroData, "UINT",
											(DWORD) VertexStreamZeroStride
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->DrawIndexedPrimitiveUP
			(
				PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat,
					pVertexStreamZeroData, VertexStreamZeroStride
			);
	};
	STDMETHOD_ (HRESULT, ProcessVertices)
		(
			UINT SrcStartIndex,
			UINT DestIndex,
			UINT VertexCount,
			IDirect3DVertexBuffer9 * pDestBuffer,
			IDirect3DVertexDeclaration9 * pVertexDecl,
			DWORD Flags
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT",
						"ProcessVertices", "UINT", (DWORD) SrcStartIndex, "UINT", (DWORD) DestIndex, "UINT",
							(DWORD) VertexCount, "IDirect3DVertexBuffer9*",
								(DWORD) pDestBuffer, "IDirect3DVertexDeclaration9*", (DWORD) pVertexDecl, "DWORD",
									(DWORD) Flags
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
	};
	STDMETHOD_ (HRESULT, CreateVertexDeclaration)
		(
			CONST D3DVERTEXELEMENT9 * pVertexElements,
			IDirect3DVertexDeclaration9 **ppDecl
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "CreateVertexDeclaration",
						"CONST D3DVERTEXELEMENT9*", (DWORD) pVertexElements, "IDirect3DVertexDeclaration9**",
							(DWORD) ppDecl
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
	};
	STDMETHOD_ (HRESULT, GetVertexDeclaration) (IDirect3DVertexDeclaration9 **ppDecl)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetVertexDeclaration",
						"IDirect3DVertexDeclaration9**", (DWORD) ppDecl
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetVertexDeclaration(ppDecl);
	};
	STDMETHOD_ (HRESULT, SetFVF) (DWORD FVF)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetFVF", "DWORD", (DWORD) FVF);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetFVF(FVF);
	};
	STDMETHOD_ (HRESULT, GetFVF) (DWORD * pFVF)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetFVF", "DWORD*", (DWORD) pFVF);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetFVF(pFVF);
	};
	STDMETHOD_ (HRESULT, CreateVertexShader) (CONST DWORD * pFunction, IDirect3DVertexShader9 **ppShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "CreateVertexShader", "CONST DWORD*",
						(DWORD) pFunction, "IDirect3DVertexShader9**", (DWORD) ppShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateVertexShader(pFunction, ppShader);
	};
	STDMETHOD_ (HRESULT, SetVertexShader) (IDirect3DVertexShader9 * pShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetVertexShader", "IDirect3DVertexShader9*",
						(DWORD) pShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShader(pShader);
	};
	STDMETHOD_ (HRESULT, GetVertexShader) (IDirect3DVertexShader9 **ppShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetVertexShader", "IDirect3DVertexShader9**",
						(DWORD) ppShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetVertexShader(ppShader);
	};
	STDMETHOD_ (HRESULT, SetVertexShaderConstantF) (UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetVertexShaderConstantF",
						"UINT", (DWORD) StartRegister, "CONST float*", (DWORD) pConstantData, "UINT",
							(DWORD) Vector4fCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	};
	STDMETHOD_ (HRESULT, GetVertexShaderConstantF) (UINT StartRegister, float *pConstantData, UINT Vector4fCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetVertexShaderConstantF",
						"UINT", (DWORD) StartRegister, "float*", (DWORD) pConstantData, "UINT", (DWORD) Vector4fCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	};
	STDMETHOD_ (HRESULT, SetVertexShaderConstantI) (UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetVertexShaderConstantI",
						"UINT", (DWORD) StartRegister, "CONST int*", (DWORD) pConstantData, "UINT",
							(DWORD) Vector4iCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	};
	STDMETHOD_ (HRESULT, GetVertexShaderConstantI) (UINT StartRegister, int *pConstantData, UINT Vector4iCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetVertexShaderConstantI",
						"UINT", (DWORD) StartRegister, "int*", (DWORD) pConstantData, "UINT", (DWORD) Vector4iCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	};
	STDMETHOD_ (HRESULT, SetVertexShaderConstantB) (UINT StartRegister, CONST BOOL * pConstantData, UINT BoolCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetVertexShaderConstantB",
						"UINT", (DWORD) StartRegister, "CONST BOOL*", (DWORD) pConstantData, "UINT", (DWORD) BoolCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
	};
	STDMETHOD_ (HRESULT, GetVertexShaderConstantB) (UINT StartRegister, BOOL * pConstantData, UINT BoolCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetVertexShaderConstantB",
						"UINT", (DWORD) StartRegister, "BOOL*", (DWORD) pConstantData, "UINT", (DWORD) BoolCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
	};
	STDMETHOD_ (HRESULT, GetStreamSource)
		(
			UINT StreamNumber,
			IDirect3DVertexBuffer9 **ppStreamData,
			UINT * OffsetInBytes,
			UINT * pStride
		)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetStreamSource",
						"UINT", (DWORD) StreamNumber, "IDirect3DVertexBuffer9**", (DWORD) ppStreamData, "UINT*",
							(DWORD) OffsetInBytes, "UINT*", (DWORD) pStride
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
	};
	STDMETHOD_ (HRESULT, GetStreamSourceFreq) (UINT StreamNumber, UINT * Divider)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "GetStreamSourceFreq", "UINT",
						(DWORD) StreamNumber, "UINT*", (DWORD) Divider
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetStreamSourceFreq(StreamNumber, Divider);
	};
	STDMETHOD_ (HRESULT, GetIndices) (IDirect3DIndexBuffer9 **ppIndexData)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetIndices", "IDirect3DIndexBuffer9**",
						(DWORD) ppIndexData
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetIndices(ppIndexData);
	};
	STDMETHOD_ (HRESULT, CreatePixelShader) (CONST DWORD * pFunction, IDirect3DPixelShader9 **ppShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "CreatePixelShader", "CONST DWORD*",
						(DWORD) pFunction, "IDirect3DPixelShader9**", (DWORD) ppShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreatePixelShader(pFunction, ppShader);
	};
	STDMETHOD_ (HRESULT, SetPixelShader) (IDirect3DPixelShader9 * pShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetPixelShader", "IDirect3DPixelShader9*",
						(DWORD) pShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShader(pShader);
	};
	STDMETHOD_ (HRESULT, GetPixelShader) (IDirect3DPixelShader9 **ppShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "GetPixelShader", "IDirect3DPixelShader9**",
						(DWORD) ppShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetPixelShader(ppShader);
	};
	STDMETHOD_ (HRESULT, SetPixelShaderConstantF) (UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetPixelShaderConstantF", "UINT",
						(DWORD) StartRegister, "CONST float*", (DWORD) pConstantData, "UINT", (DWORD) Vector4fCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	};
	STDMETHOD_ (HRESULT, GetPixelShaderConstantF) (UINT StartRegister, float *pConstantData, UINT Vector4fCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetPixelShaderConstantF", "UINT",
						(DWORD) StartRegister, "float*", (DWORD) pConstantData, "UINT", (DWORD) Vector4fCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	};
	STDMETHOD_ (HRESULT, SetPixelShaderConstantI) (UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetPixelShaderConstantI", "UINT",
						(DWORD) StartRegister, "CONST int*", (DWORD) pConstantData, "UINT", (DWORD) Vector4iCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	};
	STDMETHOD_ (HRESULT, GetPixelShaderConstantI) (UINT StartRegister, int *pConstantData, UINT Vector4iCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetPixelShaderConstantI", "UINT",
						(DWORD) StartRegister, "int*", (DWORD) pConstantData, "UINT", (DWORD) Vector4iCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	};
	STDMETHOD_ (HRESULT, SetPixelShaderConstantB) (UINT StartRegister, CONST BOOL * pConstantData, UINT BoolCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetPixelShaderConstantB", "UINT",
						(DWORD) StartRegister, "CONST BOOL*", (DWORD) pConstantData, "UINT", (DWORD) BoolCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
	};
	STDMETHOD_ (HRESULT, GetPixelShaderConstantB) (UINT StartRegister, BOOL * pConstantData, UINT BoolCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "GetPixelShaderConstantB", "UINT",
						(DWORD) StartRegister, "BOOL*", (DWORD) pConstantData, "UINT", (DWORD) BoolCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
	};
	STDMETHOD_ (HRESULT, DrawRectPatch) (UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO * pRectPatchInfo)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "DrawRectPatch", "UINT",
						(DWORD) Handle, "CONST float*", (DWORD) pNumSegs, "CONST D3DRECTPATCH_INFO*",
							(DWORD) pRectPatchInfo
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
	};
	STDMETHOD_ (HRESULT, DrawTriPatch) (UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO * pTriPatchInfo)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "DrawTriPatch", "UINT",
						(DWORD) Handle, "CONST float*", (DWORD) pNumSegs, "CONST D3DTRIPATCH_INFO*",
							(DWORD) pTriPatchInfo
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
	};
	STDMETHOD_ (HRESULT, DeletePatch) (UINT Handle)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "DeletePatch", "UINT", (DWORD) Handle);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->DeletePatch(Handle);
	};
	STDMETHOD_ (HRESULT, CreateQuery) (D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "CreateQuery", "D3DQUERYTYPE",
						(DWORD) Type, "IDirect3DQuery9**", (DWORD) ppQuery
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->CreateQuery(Type, ppQuery);
	};

	/* IDirect3DDevice methods that cannot be recorded which throw errors */
	STDMETHOD_ (UINT, GetAvailableTextureMem) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "UINT", "GetAvailableTextureMem");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetAvailableTextureMem();
	};
	STDMETHOD_ (UINT, GetNumberOfSwapChains) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "UINT", "GetNumberOfSwapChains");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetNumberOfSwapChains();
	};
	STDMETHOD_ (BOOL, ShowCursor) (BOOL bShow)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "BOOL", "ShowCursor", "BOOL", (DWORD) bShow);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->ShowCursor(bShow);
	};
	STDMETHOD_ (BOOL, GetSoftwareVertexProcessing) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "BOOL", "GetSoftwareVertexProcessing");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetSoftwareVertexProcessing();
	};
	STDMETHOD_ (float, GetNPatchMode) ()
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( )\n", "float", "GetNPatchMode");
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->GetNPatchMode();
	};

	/* Unimplemented IDirect3DDevice9 methods */
};

// =====================================================================================================================
//    ID3DXEffectStateManager: This is a user implemented interface that can be used to manage device state changes made
//    by an Effect. ;
//    typedef interface ID3DXEffectStateManager ID3DXEffectStateManager;
//    typedef interface ID3DXEffectStateManager *LPD3DXEFFECTSTATEMANAGER;
// =====================================================================================================================
class RedirectingEffectStateManager :
	public ID3DXEffectStateManager
{
/* ---------------------------------------------------------------------------------------------------------------------
 ----------------------------------------------------------------------------------------------------------------------- */
public:

	// =================================================================================================================
	//    The user must correctly implement QueryInterface, AddRef, and Release.
	// =================================================================================================================
	RedirectingEffectStateManager(IDirect3DDevice9 *pDevice)
	{
		pCurrentDevice = pDevice;
		dwRefCount = 1;
		bTrace = false;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	~RedirectingEffectStateManager()
	{
		Release();
	}

	IDirect3DDevice9	*pCurrentDevice;
	bool				bTrace;
	DWORD				dwRefCount;

	/* =================================================================================================================
	 =================================================================================================================== */
	void SetTrace(bool bval)
	{
		bTrace = bval;
	}

	// =================================================================================================================
	//    IUnknown
	// =================================================================================================================
	HRESULT __stdcall QueryInterface(REFIID iid, LPVOID *ppv)
	{
		return pCurrentDevice->QueryInterface(iid, ppv);
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	ULONG __stdcall AddRef()
	{
		dwRefCount++;

		ULONG	ret = max(0, dwRefCount);
		return ret;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	ULONG __stdcall Release()
	{
		dwRefCount--;

		ULONG	ret = max(0, dwRefCount);
		return ret;
	}

	// The following methods are called by the Effect when it wants to make the
	// corresponding device call. Note that: 1. Users manage the state and are
	// therefore responsible for making the the corresponding device calls themselves
	// inside their callbacks. 2. Effects pay attention to the return values of the
	// callbacks, and so users must pay attention to what they return in their
	// callbacks.
	STDMETHOD_ (HRESULT, SetTransform) (D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetTransform", "D3DTRANSFORMSTATETYPE",
						(DWORD) State, "CONST D3DMATRIX*", (DWORD) pMatrix
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetTransform(State, pMatrix);
	};
	STDMETHOD_ (HRESULT, SetMaterial) (CONST D3DMATERIAL9 * pMaterial)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetMaterial", "CONST D3DMATERIAL9 *",
						(DWORD) pMaterial
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetMaterial(pMaterial);
	};
	STDMETHOD_ (HRESULT, SetLight) (DWORD Index, CONST D3DLIGHT9 * pLight)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetLight", "DWORD",
						(DWORD) Index, "CONST D3DLIGHT9*", (DWORD) pLight
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetLight(Index, pLight);
	};
	STDMETHOD_ (HRESULT, LightEnable) (DWORD Index, BOOL Enable)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "LightEnable", "DWORD",
						(DWORD) Index, "BOOL", (DWORD) Enable
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->LightEnable(Index, Enable);
	};
	STDMETHOD_ (HRESULT, SetRenderState) (D3DRENDERSTATETYPE State, DWORD Value)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetRenderState", "D3DRENDERSTATETYPE",
						(DWORD) State, "DWORD", (DWORD) Value
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetRenderState(State, Value);
	};
	STDMETHOD_ (HRESULT, SetTexture) (DWORD Stage, LPDIRECT3DBASETEXTURE9 pTexture)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x)\n", "HRESULT", "SetTexture", "DWORD",
						(DWORD) Stage, "LPDIRECT3DBASETEXTURE9", (DWORD) pTexture
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetTexture(Stage, pTexture);
	};
	STDMETHOD_ (HRESULT, SetTextureStageState) (DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetTextureStageState", "DWORD",
						(DWORD) Stage, "D3DTEXTURESTAGESTATETYPE", (DWORD) Type, "DWORD", (DWORD) Value
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetTextureStageState(Stage, Type, Value);
	};
	STDMETHOD_ (HRESULT, SetSamplerState) (DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetSamplerState", "DWORD",
						(DWORD) Sampler, "D3DSAMPLERSTATETYPE", (DWORD) Type, "DWORD", (DWORD) Value
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetSamplerState(Sampler, Type, Value);
	};
	STDMETHOD_ (HRESULT, SetNPatchMode) (FLOAT NumSegments)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetNPatchMode", "FLOAT", (DWORD) NumSegments);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetNPatchMode(NumSegments);
	};
	STDMETHOD_ (HRESULT, SetFVF) (DWORD FVF)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s(strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetFVF", "DWORD", (DWORD) FVF);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetFVF(FVF);
	};
	STDMETHOD_ (HRESULT, SetVertexShader) (LPDIRECT3DVERTEXSHADER9 pShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetVertexShader", "LPDIRECT3DVERTEXSHADER9",
						(DWORD) pShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShader(pShader);
	};
	STDMETHOD_ (HRESULT, SetVertexShaderConstantF) (UINT RegisterIndex, CONST FLOAT * pConstantData, UINT RegisterCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetVertexShaderConstantF",
						"UINT", (DWORD) RegisterIndex, "CONST FLOAT*", (DWORD) pConstantData, "UINT",
							(DWORD) RegisterCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
	};
	STDMETHOD_ (HRESULT, SetVertexShaderConstantI) (UINT RegisterIndex, CONST INT * pConstantData, UINT RegisterCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetVertexShaderConstantI",
						"UINT", (DWORD) RegisterIndex, "CONST INT*", (DWORD) pConstantData, "UINT",
							(DWORD) RegisterCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
	};
	STDMETHOD_ (HRESULT, SetVertexShaderConstantB) (UINT RegisterIndex, CONST BOOL * pConstantData, UINT RegisterCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetVertexShaderConstantB",
						"UINT", (DWORD) RegisterIndex, "CONST BOOL*", (DWORD) pConstantData, "UINT",
							(DWORD) RegisterCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetVertexShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
	};
	STDMETHOD_ (HRESULT, SetPixelShader) (LPDIRECT3DPIXELSHADER9 pShader)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x)\n", "HRESULT", "SetPixelShader", "LPDIRECT3DPIXELSHADER9",
						(DWORD) pShader
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShader(pShader);
	};
	STDMETHOD_ (HRESULT, SetPixelShaderConstantF) (UINT RegisterIndex, CONST FLOAT * pConstantData, UINT RegisterCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetPixelShaderConstantF", "UINT",
						(DWORD) RegisterIndex, "CONST FLOAT*", (DWORD) pConstantData, "UINT", (DWORD) RegisterCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
	};
	STDMETHOD_ (HRESULT, SetPixelShaderConstantI) (UINT RegisterIndex, CONST INT * pConstantData, UINT RegisterCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetPixelShaderConstantI", "UINT",
						(DWORD) RegisterIndex, "CONST INT*", (DWORD) pConstantData, "UINT", (DWORD) RegisterCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
	};
	STDMETHOD_ (HRESULT, SetPixelShaderConstantB) (UINT RegisterIndex, CONST BOOL * pConstantData, UINT RegisterCount)
	{
		if(bTrace)
		{
			char	strTrace[700];
			sprintf_s
				(
					strTrace, 699, "%s %s ( %s  0x%x, %s  0x%x, %s  0x%x)\n", "HRESULT", "SetPixelShaderConstantB", "UINT",
						(DWORD) RegisterIndex, "CONST BOOL*", (DWORD) pConstantData, "UINT", (DWORD) RegisterCount
				);
			OutputDebugStringA(strTrace);
		}
		return pCurrentDevice->SetPixelShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
	};
};
