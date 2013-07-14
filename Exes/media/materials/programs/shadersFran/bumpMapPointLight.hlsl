// @author Francisco Aisa García

// Registros de texturas
sampler DiffMap			: register(s0);
sampler NormalMap	: register(s1);

// Parametros de Ogre
float4x4 viewProjectionMatrix;
float4 globalAmbient;
float3 eyePosition;
float4 lightPosition;
float4 lightColor;
float4 lightAttenuation;

// Parametros para el fragment shader
float Ka;
float Kd;
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

	OUT.position = mul(viewProjectionMatrix, IN.position);
	OUT.uv0 = IN.uv0;
	OUT.objectPos = IN.position;
	OUT.normal = IN.normal;
	
	float3 binormal = normalize( cross(IN.normal, IN.tangent.xyz) );
	float3x3 rotation = float3x3(IN.tangent.xyz, binormal, IN.normal);
	
	float3 eyeDirection = (eyePosition - IN.position).xyz;
	OUT.lightDirection = (lightPosition.xyz - IN.position).xyz;
	OUT.lightDirection = normalize(OUT.lightDirection);
	
	eyeDirection = mul(rotation, eyeDirection);
	OUT.lightDirection = mul(rotation, OUT.lightDirection);
	OUT.halfAngle = normalize( normalize(OUT.lightDirection) + normalize(eyeDirection) );

	return OUT;
}

//________________________________________________________________________

float3 expand(float3 v) {
	return (v - 0.5) * 2;
}

float4 compute_light(float3 N, float3 globalAmbient, float3 lightColor, float3 lightDirection, float3 halfAngle,
							  float Ka, float Kd, float Ks, float shininess) {
	
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
	
	float3 color = ambient + diffuse + specular;
	return float4(color, 1.0f);
}

//________________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	float3 N = tex2D(NormalMap, IN.uv0).xyz;
	N = expand(N);
	N = normalize(N);
	
	float4 color = compute_light(N, globalAmbient, lightColor.xyz, IN.lightDirection, IN.halfAngle, Ka, Kd, tex2D(DiffMap, IN.uv0).w, shininess);
	
	return color * float4( tex2D(DiffMap, IN.uv0).xyz, 1.0f );
}
