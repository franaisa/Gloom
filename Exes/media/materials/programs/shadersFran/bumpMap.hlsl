// Registros de texturas
sampler DiffMap0		: register(s0);
sampler NormalMap	: register(s1);

// Parametros de Ogre
float4x4 ViewProjectionMatrix;
float4x4 inverseViewProjMatrix;
float4 globalAmbient;
float3 eyePosition;

// Parametros para el fragment shader (desde codigo)
float3 lightColor;
float3 lightPosition;
float lightKc;
float lightKl;
float lightKq;
float3 Ke;
float3 Ka;
float3 Kd;
float3 Ks;
float shininess;

// Información de entrada del vertex shader
struct VsInput {
	float4 position 			: POSITION;
	float3 normal 			: NORMAL;
	float2 uv0 					: TEXCOORD0;
	float4 tangent			: TANGENT0;
};

// Información de salida del vertex shader
struct VsOutput {
	float4 position 			: POSITION;
	float2 uv0 					: TEXCOORD0;
	float4 objectPos			: TEXCOORD1;
	float3 normal				: TEXCOORD2;
	float3 lightDirection	: TEXCOORD3;
	float3 halfAngle			: TEXCOORD4;
};

// Información de entrada del fragment shader
struct PsInput {
	float2 uv0 					: TEXCOORD0;
	float4 position			: TEXCOORD1;
	float3 normal				: TEXCOORD2;
	float3 lightDirection	: TEXCOORD3;
	float3 halfAngle			: TEXCOORD4;
};

//________________________________________________________________________

// VERTEX SHADER
VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;

	OUT.position = mul(ViewProjectionMatrix, IN.position);
	OUT.uv0 = IN.uv0;
	OUT.objectPos = IN.position;
	OUT.normal = IN.normal;
	
	float3 binormal = normalize( cross(IN.normal, IN.tangent) );
	float3x3 rotation = float3x3(IN.tangent.xyz, binormal, IN.normal);
	
	float3 eyeDirection = (eyePosition - IN.position).xyz;
	OUT.lightDirection = (lightPosition - IN.position).xyz;
	OUT.lightDirection = normalize(OUT.lightDirection);
	
	eyeDirection = mul(rotation, eyeDirection);
	OUT.lightDirection = mul(rotation, OUT.lightDirection);
	OUT.halfAngle = normalize( normalize(OUT.lightDirection) + normalize(eyeDirection) );

	return OUT;
}

//________________________________________________________________________

float computeAttenuation(float3 P) {
	float d = distance(P, lightPosition);
	return 1 / (lightKc + lightKl * d + lightKq * d * d);
}

void computeLighting(float3 P, float3 N, out float3 diffuseResult, out float3 specularResult) {
	float attenuation = computeAttenuation(P);
	attenuation = 1.0f;
	
	// Compute the diffuse lighting
	float3 L = normalize(lightPosition - P);
	float  diffuseLight = max(dot(N, L), 0);
	diffuseResult = attenuation * lightColor * diffuseLight;

	// Compute the specular lighting
	float3 V = normalize(eyePosition - P);
	float3 H = normalize(L + V);
	float specularLight = pow(max(dot(N, H), 0), shininess);
	if (diffuseLight <= 0)
		specularLight = 0;
	
	specularResult = attenuation * lightColor * specularLight;
}

float4 compute_light(float3 N, float3 globalAmbient, float3 lightColor, float3 lightDirection, float3 halfAngle,
							  float3 Ke, float3 Ka, float3 Kd, float3 Ks, float shininess) {
							  
	//Compute emissive term
	float3 emissive = Ke;
	
	// Compute ambient term
	float3 ambient = Ka * globalAmbient;
	
	// Compute the diffuse term
	float3 L = lightDirection;
	float diffuseLight = max( dot(L, N), 0 );
	float3 diffuse = Kd * lightColor * diffuseLight;
	
	// Compute the specular term
	float3 H = halfAngle;
	float specularLight = pow( max(dot(H, N), 0), shininess );
	if(diffuseLight <= 0)
		specularLight = 0;
		
	float3 specular = Ks * lightColor * specularLight;
	
	float3 color = emissive + ambient + diffuse + specular;
	return float4(color, 1.0f);
}

//________________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	float3 N = tex2D(NormalMap, IN.uv0);
	N = normalize(N);
	
	float4 color = compute_light(N, globalAmbient, lightColor, IN.lightDirection, IN.halfAngle, Ke, Ka, Kd, Ks, shininess);
	
	return color * tex2D(DiffMap0, IN.uv0);
	
	/*float3 P = IN.position.xyz;
	float3 N = normalize(IN.normal);

	// Compute emissive and ambient terms
	float3 emissive = Ke;
	float3 ambient = Ka * globalAmbient;

	// Compute the diffuse and specular terms
	float3 diffuseLight;
	float3 specularLight;
	float3 diffuseSum  = 0;
	float3 specularSum = 0;

	// Depende del numero de luces que afecte al material
	// hay que calcularlo desde la logica
	for (int i = 0; i < 1; ++i) {
    	computeLighting(P, N, diffuseLight, specularLight);
    	diffuseSum += diffuseLight;
		specularSum += specularLight;
	}

    // Modulate diffuse and specular by material color
	float3 diffuse  = Kd * diffuseSum;
  	float3 specular = Ks * specularSum;

	float3 color = emissive + ambient + diffuse + specular;

	return float4(color, 1.0f) *  tex2D(DiffMap0, IN.uv0);*/
	//return tex2D(NormalMap, IN.uv0);
}
