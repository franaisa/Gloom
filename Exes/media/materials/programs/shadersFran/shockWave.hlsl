// Textura
sampler SceneSampler	: register(s0);

// Parametros uniformes
float4x4 viewProjectionMatrix;
float waveLength;
float wavePos;
float waveSize;
float waveCurvature;
float waveIntensity;
float waveU;
float waveV;

// Información de entrada del vertex shader
struct VsInput {
	float4 vertex 				: POSITION;
	float2 texcoord 			: TEXCOORD0;
};

// Información de salida del vertex shader
struct VsOutput {
	float4 position 			: POSITION;
	float2 uvmain		 		: TEXCOORD0;
};

// Información de entrada del fragment shader
struct PsInput {
	float2 uvmain 			: TEXCOORD0;
};

//__________________________________________________________________

// VERTEX SHADER
VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;
	
	OUT.position = mul(viewProjectionMatrix, IN.vertex);
	OUT.uvmain = IN.texcoord;
	
	return OUT;
}

//__________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	float2 waveCenter;
	waveCenter.x = waveU;
	waveCenter.y = waveV;
	
	float dist = distance(IN.uvmain, waveCenter);
	float waveDecreaseFactor = 1 - dist / waveLength;
	if(dist <= wavePos + waveSize * waveDecreaseFactor && dist >= wavePos - waveSize * waveDecreaseFactor) {
		float ecart = (dist - wavePos);
		float powEcart = 1.0 - pow( abs(ecart * waveCurvature), waveIntensity );
		float ecartTime = ecart * powEcart;
		float2 diff = normalize(IN.uvmain - waveCenter);
		float2 newTexCoord = IN.uvmain + (diff * ecartTime);
	
		return tex2D(SceneSampler, newTexCoord);
	}
	else {
		return tex2D(SceneSampler, IN.uvmain);
	}
}
