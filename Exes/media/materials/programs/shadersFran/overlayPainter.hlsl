// Texturas
sampler SceneSampler	: register(s0); // Render de la escena sin overlay
sampler OverlaySampler	: register(s1); // Render del overlay con glow

// Información de entrada del fragment shader
struct PsInput {
	float2 uvmain 			: TEXCOORD0;
};

//__________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	float4 sceneColor = tex2D(SceneSampler, IN.uvmain);
	float4 overlayColor = tex2D(OverlaySampler, IN.uvmain);
	
	if(overlayColor.x != 0.0f || overlayColor.y != 0.0f || overlayColor.z != 0.0f)
		return overlayColor;
	else
		return sceneColor;
}
