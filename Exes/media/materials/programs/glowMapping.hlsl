// @author Francisco Aisa García

// Registros de texturas
sampler DiffMap		: register(s0); // RGB = Difuso ; A = Especular
sampler GlowMap		: register(s1); // Bump Map

// Parametros proporcionados por Ogre
float4x4 viewProjectionMatrix;

// Entrada al vertex
struct VsInput {
	float4 position 			: POSITION;
	float2 uv0 					: TEXCOORD0;
};

// Salida del vertex
struct VsOutput {
	float4 position 			: POSITION;
	float2 uv0 					: TEXCOORD0;
};

// Entrada al fragment
struct PsInput {
	float2 uv0					: TEXCOORD0;
};

// ========================================================================
// 											VERTEX SHADER
// ========================================================================

VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;

	OUT.position = mul(viewProjectionMatrix, IN.position);
	OUT.uv0 = IN.uv0;
	
	return OUT;
}

// ========================================================================
// 									   FRAGMENT SHADER
// ========================================================================

float4 fragment_main(const PsInput IN) : COLOR {
	return tex2D(DiffMap, IN.uv0) * tex2D(GlowMap, IN.uv0);
}
