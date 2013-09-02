// Textura
sampler SceneSampler	: register(s0); // Render de la escena actual
sampler occlusionMap	: register(s1); // Mapa de oclusion

// Matriz de vista-proyeccion
float4x4 viewProjMatrix;
// Posicion de la luz que genera los rayos
float3 lightPosition;

// Información de entrada del fragment shader
struct PsInput {
	float2 texCoord 			: TEXCOORD0;
};

// Numero de muestras tomadas
#define NUM_SAMPLES 10
// Parametros de los godrays
float Density;
float Decay;
float Weight;
float Exposure;
	
//__________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	// Primero situamos la luz en el espacio de la pantalla
    float4 ScreenPosition = mul( viewProjMatrix, float4(lightPosition, 1.0f) );
    ScreenPosition.xyz /= ScreenPosition.w;
    ScreenPosition.x = saturate( ScreenPosition.x/2.0f+0.5f );
    ScreenPosition.y = saturate( (-ScreenPosition.y/2.0f+0.5f) );
	
	// Una vez calculada la posicion de la luz hacemos los calculos expuestos en el
	// GPU Gems 3
    float2 TexCoord = IN.texCoord;
    float2 DeltaTexCoord = (TexCoord - ScreenPosition.xy);
    DeltaTexCoord *= (1.0f / NUM_SAMPLES * Density);
    DeltaTexCoord = DeltaTexCoord * clamp(ScreenPosition.w * ScreenPosition.z,0,.5f);
    float3 col = tex2D(occlusionMap,TexCoord);
    float IlluminationDecay = 1.0;
    float3 Sample;
    for( int i = 0; i < NUM_SAMPLES; ++i ) {
        TexCoord -= DeltaTexCoord;
        Sample = tex2D(occlusionMap, TexCoord);
        Sample *= IlluminationDecay * Weight;
        col += Sample;
        IlluminationDecay *= Decay;
    }

	return float4(col * Exposure,1) + tex2D(SceneSampler, IN.texCoord);
}
