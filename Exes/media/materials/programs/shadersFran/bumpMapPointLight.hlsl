// @author Francisco Aisa García

// Registros de texturas
sampler DiffMap			: register(s0); // RGB = Difuso ; A = Especular
sampler NormalMap	: register(s1); // Bump Map

// Parametros proporcionados por Ogre
float4x4 viewProjectionMatrix;
float4 globalAmbient;
float3 eyePosition;
float4 lightPosition;
float4 lightColor;
float4 lightAttenuation;

// Parametros uniformes
// Constante para modular el ambiente
float Ka;
// Constante para modular el difuso
float Kd;
// Potencia de brillo del especular
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
	float3 normal				: TEXCOORD1;
	float3 lightDirection	: TEXCOORD2;
	float3 halfAngle			: TEXCOORD3;
};

// Información de entrada del fragment shader
struct PsInput {
	float2 uv0 					: TEXCOORD0;
	float3 normal				: TEXCOORD1;
	float3 lightDirection	: TEXCOORD2;
	float3 halfAngle			: TEXCOORD3;
};

//________________________________________________________________________

// VERTEX SHADER
VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;

	// Pasamos al fragment shader la posicion proyectada, las coordenadas de textura
	// y las normales
	OUT.position = mul(viewProjectionMatrix, IN.position);
	OUT.uv0 = IN.uv0;
	OUT.normal = IN.normal;
	
	// Construimos la matriz 3x3 que nos permite pasar del espacio objeto al espacio
	// textura (de esta forma el reflejo funciona bien sin importar la orientación
	float3 binormal = normalize( cross(IN.normal, IN.tangent.xyz) );
	float3x3 rotation = float3x3(IN.tangent.xyz, binormal, IN.normal);
	
	// Obtenemos vector director de cámara y del rayo de luz
	float3 eyeDirection = (eyePosition - IN.position).xyz;
	OUT.lightDirection = (lightPosition.xyz - IN.position).xyz;
	OUT.lightDirection = normalize(OUT.lightDirection);
	
	// Rotamos los vectores en base a la matriz 3x3 construida anteriormente
	eyeDirection = mul(rotation, eyeDirection);
	OUT.lightDirection = mul(rotation, OUT.lightDirection);
	OUT.halfAngle = normalize( normalize(OUT.lightDirection) + normalize(eyeDirection) );

	return OUT;
}

//________________________________________________________________________

// Las normales estan comprimidas en el espacio [0-1]. Esta función las desomprime
// al espacio [-1, 1]
float3 expand(float3 v) {
	return (v - 0.5) * 2;
}

float4 computeLighting(float3 N, float3 globalAmbient, float3 lightColor, float3 lightDirection, float3 halfAngle,
							  float Ka, float Kd, float Ks, float shininess) {
	
	// Calculamos la cantidad de luz ambiente
	float3 ambient = Ka * globalAmbient;
	
	// Calculamos la cantidad de luz ambiente basandonos en la ley del coseno de Lambert
	float3 L = lightDirection;
	float diffuseLight = max( dot(L, N), 0 );
	float3 diffuse = Kd * lightColor * diffuseLight;
	
	// Calculamos la cantidad de especular
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
	// Obtenemos las normales y las descomprimimos
	float3 N = tex2D(NormalMap, IN.uv0).xyz;
	N = expand(N);
	N = normalize(N);
	
	// Calculamos el color del pixel en base a la luz recibida
	// Notar que la constante de especular se obtiene del canal alfa de la textura de difuso
	float4 color = computeLighting(N, globalAmbient, lightColor.xyz, IN.lightDirection, IN.halfAngle, Ka, Kd, tex2D(DiffMap, IN.uv0).w, shininess);
	
	return color * float4( tex2D(DiffMap, IN.uv0).xyz, 1.0f );
}
