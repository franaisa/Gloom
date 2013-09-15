// Texturas
sampler SceneSampler	: register(s0); // Render de la escena
sampler HeatMap			: register(s1); // Render de la escena con el filtro de heat

// Información de entrada del fragment shader
struct PsInput {
	float2 uvmain 			: TEXCOORD0;
};

//__________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	/*float4  Dis       	= tex2D(HeatMap, IN.uvmain);
    float4  D			= 1-Dis;
    float4  Scene  	= tex2D(SceneSampler, IN.uvmain + D);*/

    return tex2D(HeatMap, IN.uvmain);
}
