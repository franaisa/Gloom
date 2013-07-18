// @author Francisco Aisa García

// Registros de texturas
sampler DiffMap			: register(s0);

// Parametros de Ogre
float4x4 viewProjectionMatrix;
float4 globalAmbient;
float3 eyePosition;

float4 lightPosition;
float4 lightColor;
float4 lightAttenuation;

// Parametros para el fragment shader
float Ka; // constante de ambiente
float Kd; // constante de difuso
float shininess; // constante de shininess

// Información de entrada del vertex shader
struct VsInput {
	float4 position 	: POSITION;
	float3 normal 	: NORMAL;
	float2 uv0 			: TEXCOORD0;
};

// Información de salida del vertex shader
struct VsOutput {
	float4 position 	: POSITION;
	float2 uv0 			: TEXCOORD0;
	float4 objectPos	: TEXCOORD1;
	float3 normal		: TEXCOORD2;
};

// Información de entrada del fragment shader
struct PsInput {
	float2 uv0 			: TEXCOORD0;
	float4 position	: TEXCOORD1;
	float3 normal		: TEXCOORD2;
};

//________________________________________________________________________

// VERTEX SHADER
VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;

	OUT.position = mul(viewProjectionMatrix, IN.position);
	OUT.uv0 = IN.uv0;
	
	OUT.objectPos = IN.position;
	OUT.normal = IN.normal;

	return OUT;
}

//________________________________________________________________________

float computeAttenuation(float4 lightPos, float4 att, float3 P) {
	float d = distance(P, lightPos.xyz);
	// y = constant att, z = linear att, w = quadratic att
	return 1 / (att.y + att.z * d + att.w * d * d);
}

void computeLighting(float4 lightPos, float4 lightCol, float3 P, float3 N, float3 eyePos, float shin, float4 att, out float3 diffuseResult, out float3 specularResult) {
	float attenuation = computeAttenuation(lightPos, att, P);
	if(attenuation > 1.0f)
		attenuation = 1.0f;
	
	// Compute the diffuse lighting
	float3 L = normalize(lightPos.xyz - P);
	float  diffuseLight = max(dot(N, L), 0);
	diffuseResult = attenuation * lightCol.xyz * diffuseLight;

	// Compute the specular lighting
	float3 V = normalize(eyePos - P);
	float3 H = normalize(L + V);
	float specularLight = pow(max(dot(N, H), 0), shin);
	if (diffuseLight <= 0)
		specularLight = 0;
	
	specularResult = attenuation * lightCol.xyz * specularLight;
}

//________________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	float3 P = IN.position.xyz;
	float3 N = normalize(IN.normal);

	// Compute ambient term
	float3 ambient = Ka * globalAmbient;

	// Compute the diffuse and specular terms
	float3 diffuseLight;
	float3 specularLight;
	float3 diffuseSum  = 0;
	float3 specularSum = 0;

	// Ejecutar tantas veces como luces haya
	//for (int i = 0; i < 1; ++i) {
		computeLighting(lightPosition, lightColor, P, N, eyePosition, shininess, lightAttenuation, diffuseLight, specularLight);
		diffuseSum += diffuseLight;
		specularSum += specularLight;
	//}
	
    // Kd allows us to modulate diffuse. Sometimes is too much if just based on the diffuse texture
	float3 diffuse = Kd * diffuseSum;
  	// Specular constant is read from channel alpha
	float3 specular = tex2D(DiffMap, IN.uv0).w * specularSum;

	float3 color = ambient + diffuse + specular;

	return float4(color, 1.0f) * float4( tex2D(DiffMap, IN.uv0).xyz, 1.0f );
}
