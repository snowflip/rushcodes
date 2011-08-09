
	 // EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION

// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not be copied
// or disclosed except in accordance with the terms of that agreement. Copyright
// (c) 1996-2008 Emergent Game Technologies. All Rights Reserved. Emergent Game
// Technologies, Chapel Hill, North Carolina 27517 http://www.emergent.net ;
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
class				CBPlayer9
{
/* ---------------------------------------------------------------------------------------------------------------------
 ----------------------------------------------------------------------------------------------------------------------- */
public:
	CBPlayer9();
	void			Playback(IDirect3DDevice9 *pDevice, CBMemoryBuffer *pCB);
	static DWORD	PlaybackNumberArgs[215];
	void			DoNULLCall();
	void			DoQueryInterface();
	void			DoAddRef();
	void			DoRelease();

	/*
	 * Implemnted IDirect3DDevice9 methods ;
	 * Note that it is assumed that input values will be valid a record time. ;
	 * Also there is no mechnism to retain returned values during playback
	 */
	void			DoDrawPrimitive();
	void			DoDrawIndexedPrimitive();
	void			DoSetStreamSource();
	void			DoSetStreamSourceFreq();
	void			DoSetVertexDeclaration();
	void			DoSetIndices();
	void			DoTestCooperativeLevel();
	void			DoEvictManagedResources();
	void			DoBeginScene();
	void			DoEndScene();
	void			DoBeginStateBlock();
	void			DoSetCursorProperties();
	void			DoSetCursorPosition();
	void			DoSetDialogBoxMode();
	void			DoReset();
	void			DoPresent();
	void			DoSetGammaRamp();
	void			DoUpdateSurface();
	void			DoUpdateTexture();
	void			DoStretchRect();
	void			DoColorFill();
	void			DoSetRenderTarget();
	void			DoSetDepthStencilSurface();
	void			DoClear();
	void			DoSetTransform();
	void			DoMultiplyTransform();
	void			DoSetViewport();
	void			DoSetMaterial();
	void			DoLightEnable();
	void			DoSetClipPlane();
	void			DoSetRenderState();
	void			DoEndStateBlock();
	void			DoSetClipStatus();
	void			DoSetTexture();
	void			DoSetTextureStageState();
	void			DoSetSamplerState();
	void			DoValidateDevice();
	void			DoSetPaletteEntries();
	void			DoSetCurrentTexturePalette();
	void			DoSetScissorRect();
	void			DoSetSoftwareVertexProcessing();
	void			DoSetNPatchMode();
	void			DoDrawPrimitiveUP();
	void			DoDrawIndexedPrimitiveUP();
	void			DoProcessVertices();
	void			DoSetFVF();
	void			DoSetVertexShader();
	void			DoSetPixelShader();
	void			DoDrawRectPatch();
	void			DoDrawTriPatch();
	void			DoDeletePatch();
	void			DoSetLight();
	void			DoSetVertexShaderConstantF();
	void			DoSetVertexShaderConstantI();
	void			DoSetVertexShaderConstantB();
	void			DoSetPixelShaderConstantF();
	void			DoSetPixelShaderConstantI();
	void			DoSetPixelShaderConstantB();

	/* =================================================================================================================
	    IDirect3DDevice methods that cannot be recorded which throw errors ;
	    create methods should not be recorded.
	 =================================================================================================================== */
	void DoCreateOffscreenPlainSurface()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateStateBlock()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateAdditionalSwapChain()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateVertexDeclaration()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateVertexShader()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateQuery()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateTexture()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateVolumeTexture()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateCubeTexture()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateVertexBuffer()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateIndexBuffer()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateRenderTarget()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreateDepthStencilSurface()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	// =================================================================================================================
	//    get methods should not be recorded.
	// =================================================================================================================
	void DoGetTextureStageState()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetSwapChain()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetRenderTargetData()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetGammaRamp()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetFrontBufferData()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetBackBuffer()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetRasterStatus()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetRenderTarget()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetDeviceCaps()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetDisplayMode()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetCreationParameters()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetDepthStencilSurface()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetTransform()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetViewport()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetMaterial()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetLight()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetLightEnable()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetClipPlane()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetRenderState()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetClipStatus()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetTexture()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetSamplerState()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetPaletteEntries()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetCurrentTexturePalette()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetScissorRect()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetVertexDeclaration()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetFVF()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetVertexShader()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetStreamSource()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetStreamSourceFreq()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetIndices()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetDirect3D()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetVertexShaderConstantF()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetVertexShaderConstantI()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetVertexShaderConstantB()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetPixelShaderConstantF()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetPixelShaderConstantI()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetPixelShaderConstantB()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoGetPixelShader()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* =================================================================================================================
	 =================================================================================================================== */
	void DoCreatePixelShader()
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return;
	};

	/* IDirect3DDevice methods that cannot be recorded which throw errors */
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
	STDMETHOD_ (BOOL, ShowCursor) (BOOL bShow)
	{
		OutputDebugStringA("CBD3D: "__FUNCTION__ " not implemented via command buffers.\n");
		__debugbreak();;
		return FALSE;
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
	CBMemoryBuffer		*m_pCB;
	IDirect3DDevice9	*m_pDevice;
};
typedef void (CBPlayer9:: *PlayBFuncPTR) ();
