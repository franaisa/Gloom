// @author Francisco Aisa García

// Registros de texturas
sampler DiffMap 		: register(s0);
sampler NormalMap	: register(s1);

// Parametros proporcionados por Ogre
float4x4 viewProjectionMatrix;
float4x4 worldMatrix;

float4 globalAmbient;
float3 eyePosition;
float4 lightPosition;
float3 lightDirection;
float4 lightColor;
float4 lightAttenuation;

// Parametros para el fragment shader

// Propiedades del spotlight
float lightCosInnerCone;
float lightCosOuterCone;

// Propiedades del material
float Ka; // constante ambiente
float Kd; // constante difuso
float shininess; // brillo

// Información de entrada del vertex shader
struct VsInput {
	float4 position 	: POSITION;
	float3 normal 	: NORMAL;
	float2 tex 			: TEXCOORD0;
	float4 tangent	: TANGENT0;
	float4 binormal	: BINORMAL0;
};

// Información de salida del vertex shader
struct VsOutput {
	float4 position 	: POSITION;
	float2 tex 			: TEXCOORD0;
	float4 objectPos	: TEXCOORD1;
	float3 normal		: TEXCOORD2;
	float4 tangent	: TEXCOORD3;
	float4 binormal	: TEXCOORD4;
	float3 viewDir		: TEXCOORD5;
};

// Información de entrada del fragment shader
struct PsInput {
	float2 tex 			: TEXCOORD0;
	float4 position	: TEXCOORD1;
	float3 normal		: TEXCOORD2;
	float4 tangent	: TEXCOORD3;
	float4 binormal	: TEXCOORD4;
	float3 viewDir		: TEXCOORD5;
};

//________________________________________________________________________

// VERTEX SHADER
VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;

	OUT.position = mul(viewProjectionMatrix, IN.position);
	OUT.tex = IN.tex;
	
	OUT.normal = mul(IN.normal, (float3x3)worldMatrix);
	OUT.normal = normalize(OUT.normal);
	
	OUT.tangent = mul(IN.tangent, worldMatrix);
	OUT.tangent = normalize(OUT.tangent);
	
	OUT.binormal = mul(IN.binormal, worldMatrix);
	OUT.binormal = normalize(OUT.binormal);
	
	OUT.objectPos = IN.position;
	
	float4 worldPosition = mul(IN.position, worldMatrix);
	
	OUT.viewDir = eyePosition.xyz - worldPosition.xyz;
	
	OUT.viewDir = normalize(OUT.viewDir);

	return OUT;
}

//________________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	float4 textureColor = float4( tex2D(DiffMap, IN.tex).xyz, 1.0f );
	float4 bumpMap = tex2D(NormalMap, IN.tex);

	bumpMap = (bumpMap * 2.0f) - 1.0f;
	
	float3 bumpNormal = IN.normal + bumpMap.x * IN.tangent.xyz + bumpMap.y * IN.binormal.xyz;
	bumpNormal = normalize(bumpNormal);
	
	float3 lightDir = -lightDirection;
	
	// CALCULAMOS EL DIFUSO
	float diffuseLight = saturate( dot(bumpNormal, lightDir) );
	float3 diffuseTerm = Kd * lightColor.xyz * diffuseLight;
	
	// CALCULAMOS EL AMBIENTE
	float3 ambientTerm = Ka * globalAmbient.xyz;
		
	// CALCULAMOS EL ESPECULAR
	float specularIntensity = tex2D(DiffMap, IN.tex).w;
	
	float3 reflection = normalize( 2 * diffuseLight * bumpNormal - lightDir );
	
	float specularLight = pow( saturate( dot(reflection, IN.viewDir) ), shininess );
	float3 specularTerm =  specularIntensity * lightColor.xyz * specularLight;
	
	float3 color = saturate( ambientTerm + diffuseTerm + specularTerm );

	return float4(color, 1.0f) * textureColor;
}
