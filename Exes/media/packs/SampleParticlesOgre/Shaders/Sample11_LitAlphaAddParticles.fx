//--------------------------------------------------------------------------------------
//
//	Global variables
//
//--------------------------------------------------------------------------------------

float3		g_LightPos[3];				// Light's direction in world space
float4		g_LightColor[3];			// Light's diffuse color
float4		g_LightAmbient;				// Light's ambient color

texture		g_SpriteDiffuse;			// Color texture for mesh

float4x4	g_mWorldViewProjection;		// World * View * Projection matrix

//--------------------------------------------------------------------------------------
//
//	Texture samplers
//
//--------------------------------------------------------------------------------------

sampler SpriteDiffuse = 
sampler_state
{
	Texture = <g_SpriteDiffuse>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//--------------------------------------------------------------------------------------
//
//	Vertex shader output structures
//
//--------------------------------------------------------------------------------------

struct VS_OUTPUT_FULL
{
	float4	Position	: POSITION;		// vertex position 
	float2	TextureUV	: TEXCOORD0;	// vertex texture coords 
	float4	Color		: TEXCOORD1;	// vertex color
	float3	Normal		: TEXCOORD2;	// vertex normal
	float3	WorldPos	: TEXCOORD3;
};

struct VS_OUTPUT_FLAT
{
	float4	Position	: POSITION;
	float4	Color		: TEXCOORD0;
};

struct VS_OUTPUT_LT
{
	float4	Position	: POSITION;
	float4	Color		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	WorldPos	: TEXCOORD2;
};

struct VS_OUTPUT_POINT
{
	float4	Position	: POSITION;
};

//--------------------------------------------------------------------------------------
//
//	Vertex shaders
//
//--------------------------------------------------------------------------------------

VS_OUTPUT_FULL		RenderSceneVS(	float3 vPos : POSITION,
									float4 vColor : TEXCOORD0,
									float2 vTexCoord : TEXCOORD1,
									float3 vNormal : TEXCOORD2)
{
	VS_OUTPUT_FULL	Output;

	Output.WorldPos = vPos;
	Output.Position = mul(float4(vPos, 1), g_mWorldViewProjection);
	Output.Normal = vNormal;
	Output.TextureUV = vTexCoord;
	Output.Color = vColor;
	return Output;
}

//--------------------------------------------------------------------------------------

VS_OUTPUT_FLAT		RenderSceneVS_Flat(	float3 vPos : POSITION,
										float4 vColor : TEXCOORD0)
{
	VS_OUTPUT_FLAT	Output;

	Output.Position = mul(float4(vPos, 1), g_mWorldViewProjection);
	Output.Color = vColor;
	return Output;
}

//--------------------------------------------------------------------------------------

VS_OUTPUT_LT		RenderSceneVS_LightingTerms(float3 vPos : POSITION,
												float4 vColor : TEXCOORD0,
												float3 vNormal : TEXCOORD1)
{
	VS_OUTPUT_LT	Output;

	Output.WorldPos = vPos;
	Output.Position = mul(float4(vPos, 1), g_mWorldViewProjection);
	Output.Normal = vNormal;
	Output.Color = vColor;
	return Output;
}

//--------------------------------------------------------------------------------------

VS_OUTPUT_POINT		RenderSceneVS_Point(float3 vPos : POSITION)
{
	VS_OUTPUT_POINT	Output;
	Output.Position = mul(float4(vPos, 1), g_mWorldViewProjection);
	return Output;
}

//--------------------------------------------------------------------------------------
//
//	Pixel shader output structures
//
//--------------------------------------------------------------------------------------

struct PS_OUTPUT
{
	float4	RGBColor : COLOR0;  // Pixel color
};

//--------------------------------------------------------------------------------------
//
//	Fragment shaders
//
//--------------------------------------------------------------------------------------

float4	ComputeDiffuseLightingCoefficient(	float absorbtion,
											float3 pN,		// pixel normal (worldspace)
											float3 pP,		// pixel pos (worldspace)
											float3 lP,		// light pos (worldspace)
											float4 lI		// light intensity
											)
{
	float3	toLight = lP - pP;
	float	id2 = 1 / dot(toLight, toLight);
	toLight = toLight * sqrt(id2);
#if	1
	float	extinction = clamp(1 - absorbtion, 0, 1);
			extinction = (1 - extinction * extinction);
	float3	lightDir = toLight;
	float	lighting = max(0, dot(toLight, pN) * 0.5 + 0.5);
			//lighting = 1 - (1 - lighting) * extinction;
#else
	float	lighting = max(0, dot(toLight, pN) * 0.5 + 0.5);
#endif

	float	iR = lighting * id2;

	return float4(iR.xxx, 1) * lI;
}

//--------------------------------------------------------------------------------------

PS_OUTPUT		RenderScenePS(VS_OUTPUT_FULL In)
{
	PS_OUTPUT	Output;

	float4	cRGBA = tex2D(SpriteDiffuse, In.TextureUV) * In.Color;
	float4	lighting = ComputeDiffuseLightingCoefficient(In.Color.a,
														 normalize(In.Normal),
														 In.WorldPos,
														 g_LightPos[0],
														 g_LightColor[0]);
	Output.RGBColor = cRGBA * lighting;

//	Output.RGBColor = float4(normalize(In.Normal) * 0.5 + 0.5, 1);

	return Output;
}

//--------------------------------------------------------------------------------------

PS_OUTPUT		RenderScenePS_Flat(VS_OUTPUT_FLAT In)
{
	PS_OUTPUT	Output;
	Output.RGBColor = In.Color;
	return Output;
}

//--------------------------------------------------------------------------------------

PS_OUTPUT		RenderScenePS_LightingTerms(VS_OUTPUT_LT In)
{
	PS_OUTPUT	Output;

#if	1
	Output.RGBColor = ComputeDiffuseLightingCoefficient(In.Color.a,
														normalize(In.Normal),
														In.WorldPos,
														g_LightPos[0],
														g_LightColor[0]);
#else
	Output.RGBColor = float4((normalize(In.Normal) * 0.5 + 0.5), 1);
#endif
	return Output;
}

//--------------------------------------------------------------------------------------

PS_OUTPUT		RenderScenePS_Point(VS_OUTPUT_POINT In)
{
	PS_OUTPUT	Output;
	Output.RGBColor = float4(1,1,1,1);
	return Output;
}

//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------

technique SpriteFull
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader  = compile ps_2_0 RenderScenePS();
	}
}

technique SpriteLightingOnly
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS_LightingTerms();
		PixelShader  = compile ps_2_0 RenderScenePS_LightingTerms();
	}
}

technique SpriteNoTex
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS_Flat();
		PixelShader  = compile ps_2_0 RenderScenePS_Flat();
	}
}

technique SpritePoint
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS_Point();
		PixelShader  = compile ps_2_0 RenderScenePS_Point();
	}
}
