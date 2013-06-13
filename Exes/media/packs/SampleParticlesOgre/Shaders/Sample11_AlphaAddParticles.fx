//--------------------------------------------------------------------------------------
//
//	Global variables
//
//--------------------------------------------------------------------------------------

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

struct VS_OUTPUT
{
	float4	Position	: POSITION;		// vertex position 
	float2	TextureUV	: TEXCOORD0;	// vertex texture coords 
	float4	Color		: TEXCOORD1;
};

//--------------------------------------------------------------------------------------

struct VS_OUTPUT_POINT
{
	float4	Position	: POSITION;
};

//--------------------------------------------------------------------------------------
//
//	Vertex shaders
//
//--------------------------------------------------------------------------------------

VS_OUTPUT		RenderSceneVS(	float3 vPos : POSITION, 
								float4 vColor : TEXCOORD0,
								float2 vTexCoord : TEXCOORD1,
								uniform bool bTexture)
{
	VS_OUTPUT	Output;

	// Transform the position from object space to homogeneous projection space
	Output.Position = mul(float4(vPos, 1), g_mWorldViewProjection);

	// Just copy the texture coordinate through
	if (bTexture)
		Output.TextureUV = vTexCoord;
	else
		Output.TextureUV = 0;

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

PS_OUTPUT RenderScenePS( VS_OUTPUT In,
						 uniform bool bTexture )
{ 
	PS_OUTPUT Output;

	float4	color = In.Color;

	// Lookup mesh texture and modulate it with diffuse
	if (bTexture)
		Output.RGBColor = tex2D(SpriteDiffuse, In.TextureUV) * color;
	else
		Output.RGBColor = color;

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
		VertexShader = compile vs_2_0 RenderSceneVS( true );
		PixelShader  = compile ps_2_0 RenderScenePS( true );
	}
}

technique SpriteLightingOnly
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS( false );
		PixelShader  = compile ps_2_0 RenderScenePS( false );
	}
}

technique SpriteNoTex
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS( false );
		PixelShader  = compile ps_2_0 RenderScenePS( false );
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
