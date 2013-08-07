sampler SceneSampler	: register(s0);
sampler HeatMap			: register(s1);
sampler HeatTexture		: register(s2);

float4x4 viewProjectionMatrix;
float4x4 worldViewMatrix;
float3 cameraPosition;
float4 clock;
float strength;
float transparency;

// Información de entrada del vertex shader
struct VsInput {
	float4 vertex 				: POSITION;
	float3 normal				: NORMAL;
	float2 texcoord 			: TEXCOORD0;
};

// Información de salida del vertex shader
struct VsOutput {
	float4 position 			: POSITION;
	float4 screenPos 		: TEXCOORD0;
	float2 uvmain 			: TEXCOORD1;
	float distortion			: TEXCOORD3;
};

// Información de entrada del fragment shader
struct PsInput {
	float4 position 			: POSITION;
	float4 screenPos 		: TEXCOORD0;
	float2 uvmain 			: TEXCOORD1;
	float distortion			: TEXCOORD2;
};

VsOutput VS_Heat(const VsInput IN) {
	VsOutput OUT;
	
	OUT.position = mul(viewProjectionMatrix, IN.vertex);
	OUT.uvmain = IN.texcoord;
	float3 vertexToCameraDir = cameraPosition - IN.vertex.xyz;
	float viewAngle = dot( normalize(vertexToCameraDir), IN.normal );
	OUT.distortion = viewAngle * viewAngle;	// square viewAngle to make the effect fall off stronger
	float depth = -mul( worldViewMatrix, IN.vertex ).z;	// compute vertex depth
	OUT.distortion /= 1+depth;		// scale effect with vertex depth
	OUT.distortion *= strength;	// multiply with user controlled strength
	OUT.screenPos = OUT.position;   // pass the position to the pixel shader
	
	return OUT;
}

float4 PS_Heat(const PsInput IN) : COLOR {
	// compute the texture coordinates
    float2 screenPos = IN.screenPos.xy / IN.screenPos.w;   // screenpos ranges from -1 to 1
    screenPos.x = (screenPos.x + 1) * 0.5;   // I need 0 to 1
    screenPos.y = (screenPos.y + 1) * 0.5;   // I need 0 to 1
	 
	// get two offset values by looking up the noise texture shifted in different directions
    half4 offsetColor1 = tex2D(HeatTexture, IN.uvmain + clock.xz);
    half4 offsetColor2 = tex2D(HeatTexture, IN.uvmain - clock.yx);
	
	screenPos.x += ((offsetColor1.r + offsetColor2.r) - 1) * IN.distortion;
    screenPos.y += ((offsetColor1.g + offsetColor2.g) - 1) * IN.distortion;
	
	half4 col = tex2D( HeatMap, screenPos );
    col.a = IN.distortion/transparency;
	return col;
}
