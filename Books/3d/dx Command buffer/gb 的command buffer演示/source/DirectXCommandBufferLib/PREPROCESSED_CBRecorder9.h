
	 // EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION

// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not be copied
// or disclosed except in accordance with the terms of that agreement. Copyright
// (c) 1996-2008 Emergent Game Technologies. All Rights Reserved. Emergent Game
// Technologies, Chapel Hill, North Carolina 27517 http://www.emergent.net
#pragma once

// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION This software is supplied
// under the terms of a license agreement or nondisclosure agreement with Emergent
// Game Technologies and may not be copied or disclosed except in accordance with
// the terms of that agreement. Copyright (c) 1996-2008 Emergent Game
// Technologies. All Rights Reserved. Emergent Game Technologies, Chapel Hill,
// North Carolina 27517 http://www.emergent.net
#pragma once

// Recording Macros Generated from MacroGenerator_Record.xls, do not modify here
// v ;
// Generated from MacroGenerator_Record.xls, do not modify here ^ required for
// setting constants. ;
// Playback Macros Generated from MacroGenerator_Playback.xls, do not modify here
// v ;
// Generated from MacroGenerator_Playback.xls, do not modify here ^ this is used
// primarily to read from memory buffer and play shader constant set calls. ;
// Get and Put macros
typedef UINT * LPUINT;
typedef D3DCAPS9	*LPD3DCAPS9;
#pragma warning(disable : 4311)
class CBRecorder9 :
	public IDirect3DDevice9
{
/* ---------------------------------------------------------------------------------------------------------------------
 ----------------------------------------------------------------------------------------------------------------------- */
public:

	/* =================================================================================================================
	 =================================================================================================================== */
	CBRecorder9() :
	m_pCB(NULL)
	{
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	void SetCommandBuffer(CBMemoryBuffer *pCB)
	{
		m_pCB = pCB;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	HRESULT BeginCommandBuffer()
	{
		m_pCB->ResetNext();
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	HRESULT EndCommandBuffer()
	{
		if(m_pCB->CheckAvailableMemorySize(1))
		{
			m_pCB->PutDWORD(CBD3D_COMMANDS::NULLCALL);
			return D3D_OK;
		}

		return E_OUTOFMEMORY;
	}

	/* IUnknown methods */
	STDMETHOD (QueryInterface) (REFIID riid, void **ppvObj)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual ULONG STDMETHODCALLTYPE AddRef()
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::AddRef);
		return 1;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual ULONG STDMETHODCALLTYPE Release()
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::Release);
		return 1;
	}

	/* =================================================================================================================
	    Implemnted IDirect3DDevice9 methods ;
	    Note that it is assumed that input values will be valid a record time. ;
	    Also there is no mechnism to retain returned values during playback
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE DrawPrimitive
	(
		D3DPRIMITIVETYPE	PrimitiveType,
		UINT				StartVertex,
		UINT				PrimitiveCount
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::DrawPrimitive);
		m_pCB->PutDWORD(PrimitiveType);
		m_pCB->PutDWORD(StartVertex);
		m_pCB->PutDWORD(PrimitiveCount);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive
	(
		D3DPRIMITIVETYPE	PrimitiveType,
		INT					BaseVertexIndex,
		UINT				MinVertexIndex,
		UINT				NumVertices,
		UINT				startIndex,
		UINT				primCount
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::DrawIndexedPrimitive);
		m_pCB->PutDWORD(PrimitiveType);
		m_pCB->PutDWORD(BaseVertexIndex);
		m_pCB->PutDWORD(MinVertexIndex);
		m_pCB->PutDWORD(NumVertices);
		m_pCB->PutDWORD(startIndex);
		m_pCB->PutDWORD(primCount);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetStreamSource
	(
		UINT					StreamNumber,
		IDirect3DVertexBuffer9	*pStreamData,
		UINT					OffsetInBytes,
		UINT					Stride
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetStreamSource);
		m_pCB->PutDWORD(StreamNumber);
		m_pCB->PutDWORD(pStreamData);
		m_pCB->PutDWORD(OffsetInBytes);
		m_pCB->PutDWORD(Stride);
		return D3D_OK;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT StreamNumber, UINT Divider)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetStreamSourceFreq);
		m_pCB->PutDWORD(StreamNumber);
		m_pCB->PutDWORD(Divider);
		return D3D_OK;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9 *pDecl)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetVertexDeclaration);
		m_pCB->PutDWORD(pDecl);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9 *pIndexData)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetIndices);
		m_pCB->PutDWORD(pIndexData);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel()
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::TestCooperativeLevel);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE EvictManagedResources()
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::EvictManagedResources);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE BeginScene()
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::BeginScene);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE EndScene()
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::EndScene);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE BeginStateBlock()
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::BeginStateBlock);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetCursorProperties
	(
		UINT				XHotSpot,
		UINT				YHotSpot,
		IDirect3DSurface9	*pCursorBitmap
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetCursorProperties);
		m_pCB->PutDWORD(XHotSpot);
		m_pCB->PutDWORD(YHotSpot);
		m_pCB->PutDWORD(pCursorBitmap);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetDialogBoxMode(BOOL bEnableDialogs)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetDialogBoxMode);
		m_pCB->PutDWORD(bEnableDialogs);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS *pPresentationParameters)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::Reset);
		m_pCB->PutDWORD(pPresentationParameters);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE Present
	(
		CONST RECT		*pSourceRect,
		CONST RECT		*pDestRect,
		HWND			hDestWindowOverride,
		CONST RGNDATA	*pDirtyRegion
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::Present);
		m_pCB->PutDWORD(pSourceRect);
		m_pCB->PutDWORD(pDestRect);
		m_pCB->PutDWORD(hDestWindowOverride);
		m_pCB->PutDWORD(pDirtyRegion);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE UpdateSurface
	(
		IDirect3DSurface9	*pSourceSurface,
		CONST RECT			*pSourceRect,
		IDirect3DSurface9	*pDestinationSurface,
		CONST POINT			*pDestPoint
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::UpdateSurface);
		m_pCB->PutDWORD(pSourceSurface);
		m_pCB->PutDWORD(pSourceRect);
		m_pCB->PutDWORD(pDestinationSurface);
		m_pCB->PutDWORD(pDestPoint);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE UpdateTexture
	(
		IDirect3DBaseTexture9	*pSourceTexture,
		IDirect3DBaseTexture9	*pDestinationTexture
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::UpdateTexture);
		m_pCB->PutDWORD(pSourceTexture);
		m_pCB->PutDWORD(pDestinationTexture);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE StretchRect
	(
		IDirect3DSurface9		*pSourceSurface,
		CONST RECT				*pSourceRect,
		IDirect3DSurface9		*pDestSurface,
		CONST RECT				*pDestRect,
		D3DTEXTUREFILTERTYPE	Filter
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::StretchRect);
		m_pCB->PutDWORD(pSourceSurface);
		m_pCB->PutDWORD(pSourceRect);
		m_pCB->PutDWORD(pDestSurface);
		m_pCB->PutDWORD(pDestRect);
		m_pCB->PutDWORD(Filter);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE ColorFill(IDirect3DSurface9 *pSurface, CONST RECT *pRect, D3DCOLOR color)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::ColorFill);
		m_pCB->PutDWORD(pSurface);
		m_pCB->PutDWORD(pRect);
		m_pCB->PutDWORD(color);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 *pRenderTarget)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetRenderTarget);
		m_pCB->PutDWORD(RenderTargetIndex);
		m_pCB->PutDWORD(pRenderTarget);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9 *pNewZStencil)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetDepthStencilSurface);
		m_pCB->PutDWORD(pNewZStencil);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE Clear
	(
		DWORD			Count,
		CONST D3DRECT	*pRects,
		DWORD			Flags,
		D3DCOLOR		Color,
		float			Z,
		DWORD			Stencil
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::Clear);
		m_pCB->PutDWORD(Count);
		m_pCB->PutDWORD(pRects);
		m_pCB->PutDWORD(Flags);
		m_pCB->PutDWORD(Color);
		m_pCB->PutDWORD(Z);
		m_pCB->PutDWORD(Stencil);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetTransform);
		m_pCB->PutDWORD(State);
		m_pCB->PutDWORD(pMatrix);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE type, CONST D3DMATRIX *pMatrix)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::MultiplyTransform);
		m_pCB->PutDWORD(type);
		m_pCB->PutDWORD(pMatrix);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetViewport(CONST D3DVIEWPORT9 *pViewport)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetViewport);
		m_pCB->PutDWORD(pViewport);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetMaterial(CONST D3DMATERIAL9 *pMaterial)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetMaterial);
		m_pCB->PutDWORD(pMaterial);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetLight(DWORD Index, CONST D3DLIGHT9 *pLight)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetLight);
		m_pCB->PutDWORD(Index);
		m_pCB->PutDWORD(pLight);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::LightEnable);
		m_pCB->PutDWORD(Index);
		m_pCB->PutDWORD(Enable);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD Index, CONST float *pPlane)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetClipPlane);
		m_pCB->PutDWORD(Index);
		m_pCB->PutDWORD(pPlane);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetRenderState);
		m_pCB->PutDWORD(State);
		m_pCB->PutDWORD(Value);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9 **ppSB)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::EndStateBlock);
		m_pCB->PutDWORD(ppSB);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetClipStatus);
		m_pCB->PutDWORD(pClipStatus);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, IDirect3DBaseTexture9 *pTexture)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetTexture);
		m_pCB->PutDWORD(Stage);
		m_pCB->PutDWORD(pTexture);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetTextureStageState);
		m_pCB->PutDWORD(Stage);
		m_pCB->PutDWORD(Type);
		m_pCB->PutDWORD(Value);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetSamplerState);
		m_pCB->PutDWORD(Sampler);
		m_pCB->PutDWORD(Type);
		m_pCB->PutDWORD(Value);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD *pNumPasses)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::ValidateDevice);
		m_pCB->PutDWORD(pNumPasses);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetPaletteEntries);
		m_pCB->PutDWORD(PaletteNumber);
		m_pCB->PutDWORD(pEntries);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT PaletteNumber)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetCurrentTexturePalette);
		m_pCB->PutDWORD(PaletteNumber);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetScissorRect(CONST RECT *pRect)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetScissorRect);
		m_pCB->PutDWORD(pRect);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL bSoftware)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetSoftwareVertexProcessing);
		m_pCB->PutDWORD(bSoftware);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetNPatchMode(float nSegments)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetNPatchMode);
		m_pCB->PutDWORD(nSegments);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE DrawPrimitiveUP
	(
		D3DPRIMITIVETYPE	PrimitiveType,
		UINT				PrimitiveCount,
		CONST void			*pVertexStreamZeroData,
		UINT				VertexStreamZeroStride
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::DrawPrimitiveUP);
		m_pCB->PutDWORD(PrimitiveType);
		m_pCB->PutDWORD(PrimitiveCount);
		m_pCB->PutDWORD(pVertexStreamZeroData);
		m_pCB->PutDWORD(VertexStreamZeroStride);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP
	(
		D3DPRIMITIVETYPE	PrimitiveType,
		UINT				MinVertexIndex,
		UINT				NumVertices,
		UINT				PrimitiveCount,
		CONST void			*pIndexData,
		D3DFORMAT			IndexDataFormat,
		CONST void			*pVertexStreamZeroData,
		UINT				VertexStreamZeroStride
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::DrawIndexedPrimitiveUP);
		m_pCB->PutDWORD(PrimitiveType);
		m_pCB->PutDWORD(MinVertexIndex);
		m_pCB->PutDWORD(NumVertices);
		m_pCB->PutDWORD(PrimitiveCount);
		m_pCB->PutDWORD(pIndexData);
		m_pCB->PutDWORD(IndexDataFormat);
		m_pCB->PutDWORD(pVertexStreamZeroData);
		m_pCB->PutDWORD(VertexStreamZeroStride);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE ProcessVertices
	(
		UINT						SrcStartIndex,
		UINT						DestIndex,
		UINT						VertexCount,
		IDirect3DVertexBuffer9		*pDestBuffer,
		IDirect3DVertexDeclaration9 *pVertexDecl,
		DWORD						Flags
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::ProcessVertices);
		m_pCB->PutDWORD(SrcStartIndex);
		m_pCB->PutDWORD(DestIndex);
		m_pCB->PutDWORD(VertexCount);
		m_pCB->PutDWORD(pDestBuffer);
		m_pCB->PutDWORD(pVertexDecl);
		m_pCB->PutDWORD(Flags);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetFVF(DWORD FVF)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetFVF);
		m_pCB->PutDWORD(FVF);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9 *pShader)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetVertexShader);
		m_pCB->PutDWORD(pShader);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9 *pShader)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetPixelShader);
		m_pCB->PutDWORD(pShader);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE DrawRectPatch
	(
		UINT					Handle,
		CONST float				*pNumSegs,
		CONST D3DRECTPATCH_INFO *pRectPatchInfo
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::DrawRectPatch);
		m_pCB->PutDWORD(Handle);
		m_pCB->PutDWORD(pNumSegs);
		m_pCB->PutDWORD(pRectPatchInfo);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE DrawTriPatch
	(
		UINT					Handle,
		CONST float				*pNumSegs,
		CONST D3DTRIPATCH_INFO	*pTriPatchInfo
	)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::DrawTriPatch);
		m_pCB->PutDWORD(Handle);
		m_pCB->PutDWORD(pNumSegs);
		m_pCB->PutDWORD(pTriPatchInfo);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE DeletePatch(UINT Handle)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::DeletePatch);
		m_pCB->PutDWORD(Handle);
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual void STDMETHODCALLTYPE SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP *pRamp)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetGammaRamp);
		m_pCB->PutDWORD(iSwapChain);
		m_pCB->PutDWORD(Flags);
		m_pCB->PutDWORD(pRamp);
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual void STDMETHODCALLTYPE SetCursorPosition(int X, int Y, DWORD Flags)
	{ ;
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetCursorPosition);
		m_pCB->PutDWORD(X);
		m_pCB->PutDWORD(Y);
		m_pCB->PutDWORD(Flags);
	}

	/* IDirect3DDevice methods that cannot be recorded which throw errors */
	STDMETHOD_ (BOOL, ShowCursor) (BOOL bShow)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return FALSE;
	};
	STDMETHOD_ (UINT, GetAvailableTextureMem) (THIS)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return 0;
	};
	STDMETHOD_ (UINT, GetNumberOfSwapChains) (THIS)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return 0;
	};
	STDMETHOD_ (BOOL, GetSoftwareVertexProcessing) (THIS)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return FALSE;
	};
	STDMETHOD_ (float, GetNPatchMode) (THIS)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return 0.0f;
	};
	STDMETHOD_ (void, GetGammaRamp) (UINT iSwapChain, D3DGAMMARAMP * pRamp)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};
	STDMETHOD (GetSwapChain) (UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetDeviceCaps) (LPD3DCAPS9 pCaps)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetDisplayMode) (UINT iSwapChain, D3DDISPLAYMODE * pMode)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetCreationParameters) (D3DDEVICE_CREATION_PARAMETERS * pParameters)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetBackBuffer)
		(
			UINT iSwapChain,
			UINT iBackBuffer,
			D3DBACKBUFFER_TYPE Type,
			IDirect3DSurface9 **ppBackBuffer
		)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetRasterStatus) (UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetPaletteEntries) (UINT PaletteNumber, PALETTEENTRY * pEntries)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetRenderTargetData) (IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetFrontBufferData) (UINT iSwapChain, IDirect3DSurface9 * pDestSurface)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetRenderTarget) (DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetDepthStencilSurface) (IDirect3DSurface9 **ppZStencilSurface)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetTransform) (D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetViewport) (D3DVIEWPORT9 * pViewport)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetMaterial) (D3DMATERIAL9 * pMaterial)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetLightEnable) (DWORD Index, BOOL * pEnable)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetLight) (DWORD Index, D3DLIGHT9 * pLight)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetClipPlane) (DWORD Index, float *pPlane)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetRenderState) (D3DRENDERSTATETYPE State, DWORD * pValue)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetClipStatus) (D3DCLIPSTATUS9 * pClipStatus)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetTexture) (DWORD Stage, IDirect3DBaseTexture9 **ppTexture)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetTextureStageState) (DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetSamplerState) (DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetCurrentTexturePalette) (UINT * PaletteNumber)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetScissorRect) (RECT * pRect)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetVertexDeclaration) (IDirect3DVertexDeclaration9 **ppDecl)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetFVF) (DWORD * pFVF)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetPixelShader) (IDirect3DPixelShader9 **ppShader)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetVertexShader) (IDirect3DVertexShader9 **ppShader)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetStreamSource)
		(
			UINT StreamNumber,
			IDirect3DVertexBuffer9 **ppStreamData,
			LPUINT OffsetInBytes,
			LPUINT pStride
		)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetStreamSourceFreq) (UINT StreamNumber, LPUINT Divider)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetIndices) (IDirect3DIndexBuffer9 **ppIndexData)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateQuery) (D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetDirect3D) (IDirect3D9 **ppD3D9)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetVertexShaderConstantF) (UINT StartRegister, float *pConstantData, UINT Vector4fCount)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetVertexShaderConstantI) (UINT StartRegister, int *pConstantData, UINT Vector4iCount)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetVertexShaderConstantB) (UINT StartRegister, BOOL * pConstantData, UINT BoolCount)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetPixelShaderConstantF) (UINT StartRegister, float *pConstantData, UINT Vector4fCount)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetPixelShaderConstantI) (UINT StartRegister, int *pConstantData, UINT Vector4iCount)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (GetPixelShaderConstantB) (UINT StartRegister, BOOL * pConstantData, UINT BoolCount)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateAdditionalSwapChain)
		(
			D3DPRESENT_PARAMETERS * pPresentationParameters,
			IDirect3DSwapChain9 **pSwapChain
		)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateTexture)
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
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateVolumeTexture)
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
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateCubeTexture)
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
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateVertexBuffer)
		(
			UINT Length,
			DWORD Usage,
			DWORD FVF,
			D3DPOOL Pool,
			IDirect3DVertexBuffer9 **ppVertexBuffer,
			HANDLE * pSharedHandle
		)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateIndexBuffer)
		(
			UINT Length,
			DWORD Usage,
			D3DFORMAT Format,
			D3DPOOL Pool,
			IDirect3DIndexBuffer9 **ppIndexBuffer,
			HANDLE * pSharedHandle
		)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateRenderTarget)
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
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateDepthStencilSurface)
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
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateOffscreenPlainSurface)
		(
			UINT Width,
			UINT Height,
			D3DFORMAT Format,
			D3DPOOL Pool,
			IDirect3DSurface9 **ppSurface,
			HANDLE * pSharedHandle
		)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateStateBlock) (D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateVertexDeclaration)
		(
			CONST D3DVERTEXELEMENT9 * pVertexElements,
			IDirect3DVertexDeclaration9 **ppDecl
		)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreateVertexShader) (CONST DWORD * pFunction, IDirect3DVertexShader9 **ppShader)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};
	STDMETHOD (CreatePixelShader) (CONST DWORD * pFunction, IDirect3DPixelShader9 **ppShader)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return D3DERR_INVALIDCALL;
	};

	/* =================================================================================================================
	    IDirect3DDevice9 methods that write variable sized objects
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF
	(
		UINT		StartRegister,
		CONST float *pConstantData,
		UINT		Vector4fCount
	)
	{
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetVertexShaderConstantF);
		m_pCB->PutDWORD(StartRegister);
		m_pCB->PutDWORD(Vector4fCount);
		m_pCB->DoPutMem((DWORD *) pConstantData, Vector4fCount * 4 * sizeof(float));
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI
	(
		UINT		StartRegister,
		CONST int	*pConstantData,
		UINT		Vector4iCount
	)
	{
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetVertexShaderConstantI);
		m_pCB->PutDWORD(StartRegister);
		m_pCB->PutDWORD(Vector4iCount);
		m_pCB->DoPutMem((DWORD *) pConstantData, Vector4iCount * 4 * sizeof(int));
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB
	(
		UINT		StartRegister,
		CONST BOOL	*pConstantData,
		UINT		BoolCount
	)
	{
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetVertexShaderConstantB);
		m_pCB->PutDWORD(StartRegister);
		m_pCB->PutDWORD(BoolCount);
		m_pCB->DoPutMem((DWORD *) pConstantData, BoolCount * 4 * sizeof(BOOL));
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF
	(
		UINT		StartRegister,
		CONST float *pConstantData,
		UINT		Vector4fCount
	)
	{
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetPixelShaderConstantF);
		m_pCB->PutDWORD(StartRegister);
		m_pCB->PutDWORD(Vector4fCount);
		m_pCB->DoPutMem((DWORD *) pConstantData, Vector4fCount * 4 * sizeof(float));
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI
	(
		UINT		StartRegister,
		CONST int	*pConstantData,
		UINT		Vector4iCount
	)
	{
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetPixelShaderConstantI);
		m_pCB->PutDWORD(StartRegister);
		m_pCB->PutDWORD(Vector4iCount);
		m_pCB->DoPutMem((DWORD *) pConstantData, Vector4iCount * 4 * sizeof(int));
		return D3D_OK;
	}

	/* =================================================================================================================
	 =================================================================================================================== */
	virtual HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB
	(
		UINT		StartRegister,
		CONST BOOL	*pConstantData,
		UINT		BoolCount
	)
	{
		m_pCB->PutDWORD(CBD3D_COMMANDS::SetPixelShaderConstantB);
		m_pCB->PutDWORD(StartRegister);
		m_pCB->PutDWORD(BoolCount);
		m_pCB->DoPutMem((DWORD *) pConstantData, BoolCount * 4 * sizeof(BOOL));
		return D3D_OK;
	}

/* ---------------------------------------------------------------------------------------------------------------------
 ----------------------------------------------------------------------------------------------------------------------- */
protected:
	CBMemoryBuffer	*m_pCB;
};
