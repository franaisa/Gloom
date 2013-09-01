// Textura
sampler SceneSampler	: register(s0);
sampler DepthMap			: register(s1);

// Parametros uniformes
float4x4 viewProjMatrix;
float3 lightPosition;

// Información de entrada del fragment shader
struct PsInput {
	float2 texCoord 			: TEXCOORD0;
};

#define NUM_SAMPLES 10

//__________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	float Density = 0.5f;
	float Decay = 0.95f;
	float Weight = 1.0f;
	float Exposure = 0.15f;

	// Find light pixel position
    float4 ScreenPosition = mul( viewProjMatrix, float4(lightPosition, 1.0f) );
    ScreenPosition.xyz /= ScreenPosition.w;
    ScreenPosition.x = ScreenPosition.x/2.0f+0.5f;
    ScreenPosition.y = (-ScreenPosition.y/2.0f+0.5f);
    float2 TexCoord = IN.texCoord;
    float2 DeltaTexCoord = (TexCoord - ScreenPosition.xy);
    DeltaTexCoord *= (1.0f / NUM_SAMPLES * Density);
    DeltaTexCoord = DeltaTexCoord * clamp(ScreenPosition.w * ScreenPosition.z,0,.5f);
    float3 col = tex2D(DepthMap,TexCoord);
    float IlluminationDecay = 1.0;
    float3 Sample;
    for( int i = 0; i < NUM_SAMPLES; ++i )
    {
        TexCoord -= DeltaTexCoord;
        Sample = tex2D(DepthMap, TexCoord);
        Sample *= IlluminationDecay * Weight;
        col += Sample;
        IlluminationDecay *= Decay;           
    }

	return float4(col * Exposure,1) + tex2D(SceneSampler, IN.texCoord);
	
	/*float Density = 0.5f;
	float Decay = 0.95f;
	float Weight = 1.0f;
	float Exposure = 0.15f;
	float2 texCoord = IN.texCoord;
	
	float4 ScreenLightPos = mul( viewProjMatrix, float4(lightPosition, 1.0f) );
    ScreenLightPos.xyz /= ScreenLightPos.w;
    ScreenLightPos.x = ScreenLightPos.x/2.0f+0.5f;
    ScreenLightPos.y = (-ScreenLightPos.y/2.0f+0.5f);
	
	// Calculate vector from pixel to light source in screen space.  
	float2 deltaTexCoord = (texCoord - ScreenLightPos.xy);  
	// Divide by number of samples and scale by control factor.  
	deltaTexCoord *= 1.0f / NUM_SAMPLES * Density;  
	// Store initial sample.  
	float3 color = tex2D(DepthMap, texCoord);  
	// Set up illumination decay factor.  
	float illuminationDecay = 1.0f;  
	// Evaluate summation from Equation 3 NUM_SAMPLES iterations.  
	for (int i = 0; i < NUM_SAMPLES; i++) {  
		// Step sample location along ray.  
		texCoord -= deltaTexCoord;  
		// Retrieve sample at new location.  
		float3 sample = tex2D(DepthMap, texCoord);  
		// Apply sample attenuation scale/decay factors.  
		sample *= illuminationDecay * Weight;  
		// Accumulate combined color.  
		color += sample;  
		// Update exponential decay factor.  
		illuminationDecay *= Decay;
	}  
  // Output final color with a further scale control factor.  
   return tex2D(SceneSampler, IN.texCoord) + float4( color * Exposure, 1);  */


	// Calculamos el valor de profunidad
	/*float depthVal = 1 - (tex2D(DepthMap, IN.texCoord).r);
    float4 scene = tex2D(SceneSampler, IN.texCoord);*/
    
	// Calculamos la posicion del pixel en el mundo
	/*float4 position;
    position.x = IN.texCoord.x * 2.0f - 1.0f;
    position.y = -(IN.texCoord.y * 2.0f - 1.0f);
    position.z = depthVal;
    position.w = 1.0f;
    float4 worldPos = mul(inverseViewProjMatrix, position);
    worldPos /= worldPos.w;*/
	
	// Calculamos la posicion de la luz en la pantalla
	/*float4 screenPosition = mul( viewProjMatrix, float4(lightPosition, 1.0f) );
    screenPosition.xyz /= screenPosition.w;
    screenPosition.x = screenPosition.x/2.0f+0.5f;
    screenPosition.y = (-screenPosition.y/2.0f+0.5f);*/
	
	// Si el pixel esta delante de la fuente de luz, lo pintamos de negro
    /*if(depthVal < screenPosition.z - 0.00025f)
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
	else
		return scene;*/
	
	/*float4 color;
	if(IN.texCoord.x < screenPosition.x + 0.02f && IN.texCoord.x > screenPosition.x - 0.02f && IN.texCoord.y < screenPosition.y + 0.02f && IN.texCoord.y > screenPosition.y - 0.02f && screenPosition.w > 0) {
		color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else {
		color = tex2D(DepthMap, IN.texCoord);
	}

	return color;*/
}
