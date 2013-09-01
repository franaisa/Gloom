// @author Francisco Aisa García

float4x4 worldViewProj;

struct VsInput {
	float4 position 			: POSITION;
};

struct VsOutput {
	float4 position 			: POSITION;
	float4 color				: COLOR;
};

struct PsInput {
	float4 color				: COLOR;
};


// ========================================================================
// 											VERTEX SHADER
// ========================================================================

VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;

	OUT.position = mul(worldViewProj, IN.position);
	OUT.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	return OUT;
}

// ========================================================================
// 									   FRAGMENT SHADER
// ========================================================================

float4 fragment_main(const PsInput IN) : COLOR {	
	return IN.color;
}

