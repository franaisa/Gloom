// Textura
sampler SceneSampler	: register(s0);
sampler DepthMap			: register(s1);

// Parametros uniformes
float4x4 viewProjMatrix;
float4x4 inverseViewProjMatrix;
float4x4 previousViewProjMatrix;
float3 lightPosition;

// Información de entrada del fragment shader
struct PsInput {
	float2 texCoord 			: TEXCOORD0;
};

//__________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
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
	float4 screenPosition = mul( previousViewProjMatrix, float4(lightPosition, 1.0f) );
    screenPosition.xyz /= screenPosition.w;
    screenPosition.x = screenPosition.x/2.0f+0.5f;
    screenPosition.y = (-screenPosition.y/2.0f+0.5f);
	
	// Si el pixel esta delante de la fuente de luz, lo pintamos de negro
    /*if(depthVal < screenPosition.z - 0.00025f)
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
	else
		return scene;*/
	
	float4 color;
	if(IN.texCoord.x < screenPosition.x + 0.02f && IN.texCoord.x > screenPosition.x - 0.02f && IN.texCoord.y < screenPosition.y + 0.02f && IN.texCoord.y > screenPosition.y - 0.02f && screenPosition.w > 0) {
		color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else {
		color = tex2D(DepthMap, IN.texCoord);
	}
	
	// Calculamos la posicion de la luz en la pantalla
	screenPosition = mul( viewProjMatrix, float4(lightPosition, 1.0f) );
    screenPosition.xyz /= screenPosition.w;
    screenPosition.x = screenPosition.x/2.0f+0.5f;
    screenPosition.y = (-screenPosition.y/2.0f+0.5f);
	if(IN.texCoord.x < screenPosition.x + 0.02f && IN.texCoord.x > screenPosition.x - 0.02f && IN.texCoord.y < screenPosition.y + 0.02f && IN.texCoord.y > screenPosition.y - 0.02f && screenPosition.w > 0) {
		color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
	return color;
}
