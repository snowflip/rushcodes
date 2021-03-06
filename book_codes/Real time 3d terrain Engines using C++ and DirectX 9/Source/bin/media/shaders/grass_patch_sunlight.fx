#include "light_scattering_constants.h"
#include "patch_deform.h"

// transformations
float4x4 mViewProj: VIEWPROJECTION;
float4x4 mWorld: WORLD;

float4 vCameraPos: worldcamerapos;
float4 vCameraNearFar: viewdistances;
float4 sun_vec: sunvector = {0.578f,0.578f,0.578f,0.0f};
float4 sun_color: suncolor = {0.578f,0.578f,0.578f,0.0f};

// the cube map
texture tex0 : TEXTURE;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 vI			: COLOR0;
    float4 vE			: COLOR1;
	  float2 tex  : TEXCOORD0;
};

VS_OUTPUT VS(
    float3 Pos  : POSITION,
		float3 Norm  : NORMAL,
		float2 tex  : TEXCOORD0)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

		PatchDeformWithNormals(Pos, Norm, mWorld);

    Out.Pos = mul(float4(Pos, 1), mViewProj);  // position (view space)
    Out.tex = tex;

		float4 worldNorm = mul(float4(Norm, 0), mWorld);
		worldNorm = normalize(worldNorm);

 		// calculate eye vector and world
		// distance using a world vertex
		// and camera position.
		float3 eyeVector = vCameraPos - Pos; 
		float s = length(eyeVector);
		eyeVector = normalize(eyeVector);

		atmosphericLighting(
			eyeVector,
			sun_vec,
			worldNorm,
			sun_color,
			s,
			Out.vE,
			Out.vI);

    return Out;
}


sampler LinearSamp0 = 
sampler_state 
{
    texture = <tex0>;
    AddressU  = clamp;        
    AddressV  = clamp;
    AddressW  = clamp;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};


float4 PS(VS_OUTPUT vIN) : COLOR
{
	float4 color= tex2D(LinearSamp0, vIN.tex ); 

	color.xyz = (color*vIN.vE) + (vIN.vI);
	return color;

}

technique TVertexAndPixelShader
{
    pass P0
    {
			CULLMODE = CCW;
			ZENABLE = TRUE;
			ZWRITEENABLE = FALSE;
			ZFUNC = LESSEQUAL;

			AlphaTestEnable = true;
			AlphaRef = 16;
			AlphaFunc = Greater;

			AlphaBlendEnable = true;
			SrcBlend = destAlpha;
			DestBlend = one;
			BlendOp = add;

       // shaders
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PS();
    }  
}
