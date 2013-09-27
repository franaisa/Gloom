// @author Francisco Aisa García

// Registros de texturas
sampler DiffMap			: register(s0); // RGB = Difuso ; A = Especular
sampler NormalMap	: register(s1); // Bump Map
sampler SpecMap		: register(s2); // Mapa de especulares

// Parametros proporcionados por Ogre
float4x4 viewProjectionMatrix;
float4 globalAmbient;
float3 eyePosition;
float3 lightDir;
float4 lightPosition;
float4 lightColor;
float4 lightAttenuation;
float4 spotLightParams;

// Parametro customizado que se le pasa a cada luz
// Si la w vale 1.0f quiere decir que la luz ha sido marcada como static = true
// Calcular bump mapping con difuso + especular
// Si la w vale 0.0f quiere decir que la luz ha sido marcada como dinamica
// Aplicar light map + bumped diffuse
float4 customLightParams;

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

struct VsInput_AMB {
	float4 position 			: POSITION;
	float2 uv0 					: TEXCOORD0;
};

struct VsOutput_AMB {
	float4 position 			: POSITION;
	float2 uv0 					: TEXCOORD0;
};

struct PsInput_AMB {
	float2 uv0					: TEXCOORD0;
};

// ========================================================================
// 									   FUNCIONES DE CALCULO
// ========================================================================

bool isNotSpotLight(float4 spotLightParams) {
	return spotLightParams.x == 1.0f 	&& 
			   spotLightParams.y == 0.0f 	&& 
			   spotLightParams.z == 0.0f 	&& 
			   spotLightParams.w == 1.0f;
}

//________________________________________________________________________

float dualConeSpotlight(float3 P) {
	float3 V = normalize(P - lightPosition.xyz);
	float cosDirection = dot(V, normalize(lightDir));

	return smoothstep(spotLightParams.y, spotLightParams.x, cosDirection);
}

//________________________________________________________________________

float computeAttenuation(float3 lightPos, float4 att, float3 P) {
	float d = distance(P, lightPos);
	// y = constant att, z = linear att, w = quadratic att
	return 1 / (att.y + att.z * d + att.w * d * d);
}

//________________________________________________________________________

void computeLighting(float3 lightPosition, float4 lightAttenuation, float3 P, float3 N, float3 lightColor, float3 lightDirection, float3 halfAngle,
							   float Kd, float Ks, float shininess, out float3 diffuse, out float3 specular) {

	// Calculamos la atenuacion. Si el rango es 0, quiere decir que no queremos
	// atenuacion
	float attenuation;
	if(lightAttenuation.x != 0.0f)
		attenuation = saturate( computeAttenuation(lightPosition, lightAttenuation, P) );
	else
		attenuation = 1.0f;

	// Calculamos el efecto del sopt light
	float spotEffect;
	if( !isNotSpotLight(spotLightParams) )
		spotEffect = dualConeSpotlight(P);
	else
		spotEffect = 1.0f;

	// Calculamos la cantidad de luz ambiente basandonos en la ley del coseno de Lambert
	float3 L = lightDirection;
	float diffuseLight = max( dot(L, N), 0 );
	diffuse = Kd * spotEffect * attenuation * lightColor * diffuseLight;

	// Calculamos la cantidad de especular
	float3 H = halfAngle;
	float specularLight = pow( max(dot(H, N), 0), shininess );
	if(diffuseLight <= 0)
		specularLight = 0;

	specular = Ks * spotEffect * attenuation * lightColor * specularLight;
}

//________________________________________________________________________

// Esta función realiza el mismo computo que la función anterior con la diferencia de que la intensidad
// de especular indica un color completo (R, G, B) en lugar de una intensidad constante para todos los canales.
void computeLightingExtSpecular(float3 lightPosition, float4 lightAttenuation, float3 P, float3 N, float3 lightColor, float3 lightDirection, float3 halfAngle,
							   float Kd, float3 Ks, float shininess, out float3 diffuse, out float3 specular) {

	// Calculamos la atenuacion. Si el rango es 0, quiere decir que no queremos
	// atenuacion
	float attenuation;
	if(lightAttenuation.x != 0.0f)
		attenuation = saturate( computeAttenuation(lightPosition, lightAttenuation, P) );
	else
		attenuation = 1.0f;

	// Calculamos el efecto del sopt light
	float spotEffect;
	if( !isNotSpotLight(spotLightParams) )
		spotEffect = dualConeSpotlight(P);
	else
		spotEffect = 1.0f;

	// Calculamos la cantidad de luz ambiente basandonos en la ley del coseno de Lambert
	float3 L = lightDirection;
	float diffuseLight = max( dot(L, N), 0 );
	diffuse = Kd * spotEffect * attenuation * lightColor * diffuseLight;

	// Calculamos la cantidad de especular
	float3 H = halfAngle;
	float specularLight = pow( max(dot(H, N), 0), shininess );
	if(diffuseLight <= 0)
		specularLight = 0;

	specular = Ks * spotEffect * attenuation * lightColor * specularLight;
}

//________________________________________________________________________

// Las normales estan comprimidas en el espacio [0-1]. Esta función las desomprime
// al espacio [-1, 1]
float3 expand(float3 v) {
	return (v - 0.5) * 2;
}


// ========================================================================
// 											VERTEX SHADER
// ========================================================================

VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;

	// Pasamos al fragment shader la posicion proyectada, las coordenadas de textura
	// y las normales
	OUT.position = mul(viewProjectionMatrix, IN.position);
	OUT.uv0 = IN.uv0;
	OUT.objectPos = IN.position;
	OUT.normal = IN.normal;

	// Construimos la matriz 3x3 que nos permite pasar del espacio objeto al espacio
	// textura (de esta forma el reflejo funciona bien sin importar la orientación
	float3 binormal = normalize( cross(IN.normal, IN.tangent.xyz) );
	float3x3 rotation = float3x3(IN.tangent.xyz, binormal, IN.normal);

	// Si la luz es puntual w vale 1.0f. En caso de ser direccional vale 1.0f
	// Si la luz no es un spot light, spotlightparams vale (1.0f, 0.0f, 0.0f, 1.0f)
	if( lightPosition.w == 0.0f && isNotSpotLight(spotLightParams) )
		OUT.lightDirection = -lightDir;
	else
		OUT.lightDirection = (lightPosition.xyz - IN.position).xyz;

	// Obtenemos vector director de cámara y del rayo de luz
	float3 eyeDirection = (eyePosition - IN.position).xyz;
	//OUT.lightDirection = (lightPosition.xyz - IN.position).xyz;
	OUT.lightDirection = normalize(OUT.lightDirection);

	// Rotamos los vectores en base a la matriz 3x3 construida anteriormente
	eyeDirection = mul(rotation, eyeDirection);
	OUT.lightDirection = mul(rotation, OUT.lightDirection);
	OUT.halfAngle = normalize( normalize(OUT.lightDirection) + normalize(eyeDirection) );

	return OUT;
}

//________________________________________________________________________

VsOutput_AMB vertex_AMB(const VsInput_AMB IN) {
	VsOutput_AMB OUT;

	OUT.position = mul(viewProjectionMatrix, IN.position);
	OUT.uv0 = IN.uv0;

	return OUT;
}

// ========================================================================
// 									   FRAGMENT SHADER
// ========================================================================

// Fragment para calcular ambiente + difuso + especular
float4 fragment_main(const PsInput IN) : COLOR {
	// Obtenemos las normales y las descomprimimos
	float3 N = tex2D(NormalMap, IN.uv0).xyz;
	N = expand(N);
	N = normalize(N);

	// Calculamos la cantidad de luz ambiente
	float3 ambient = Ka * globalAmbient.xyz;

	// Calculamos el color del pixel en base a la luz recibida
	// Notar que la constante de especular se obtiene del canal alfa de la textura de normales
	float3 specular, diffuse;
	computeLighting(lightPosition.xyz, lightAttenuation, IN.position.xyz, N, lightColor.xyz, IN.lightDirection, IN.halfAngle, Kd, tex2D(NormalMap, IN.uv0).w, shininess, diffuse, specular);

	// Al resultado final de la iluminación del material hay que sumar
	// el color del entorno reflejado por el especular
	float3 color = (ambient + diffuse + specular) * tex2D(DiffMap, IN.uv0).xyz;

	return float4(color, 1.0f);
}

//________________________________________________________________________

// Fragment para calcular ambiente
float4 fragment_AMB(const PsInput_AMB IN) : COLOR {
	float3 color = Ka * globalAmbient.xyz * tex2D(DiffMap, IN.uv0).xyz;

	return float4(color, 1.0f);
}

//________________________________________________________________________

// Fragment para calcular difuso + especular
float4 fragment_DIFF_ALPHASPEC(const PsInput IN) : COLOR {
	// Obtenemos las normales y las descomprimimos
	float3 N = tex2D(NormalMap, IN.uv0).xyz;
	N = expand(N);
	N = normalize(N);

	// Calculamos el color del pixel en base a la luz recibida
	// Notar que la constante de especular se obtiene del canal alfa de la textura de normales
	float3 specular, diffuse;
	computeLighting(lightPosition.xyz, lightAttenuation, IN.position.xyz, N, lightColor.xyz, IN.lightDirection, IN.halfAngle, Kd, tex2D(NormalMap, IN.uv0).w, shininess, diffuse, specular);

	// Al resultado final de la iluminación del material hay que sumar
	// el color del entorno reflejado por el especular
	float3 color = (diffuse + specular) * tex2D(DiffMap, IN.uv0).xyz;

	return float4(color, 1.0f);
}

//________________________________________________________________________

// Fragment para calcular difuso + especular. El especular se obtiene de una textura especifica
// en lugar de sacarlo del canal alfa de la textura de normales.
float4 fragment_DIFF_SPEC(const PsInput IN) : COLOR {
	// Obtenemos las normales y las descomprimimos
	float3 N = tex2D(NormalMap, IN.uv0).xyz;
	N = expand(N);
	N = normalize(N);

	// Calculamos el color del pixel en base a la luz recibida
	// Notar que la constante de especular se obtiene del canal alfa de la textura de normales
	float3 specular, diffuse;
	computeLightingExtSpecular(lightPosition.xyz, lightAttenuation, IN.position.xyz, N, lightColor.xyz, IN.lightDirection, IN.halfAngle, Kd, tex2D(SpecMap, IN.uv0).xyz, shininess, diffuse, specular);

	// Al resultado final de la iluminación del material hay que sumar
	// el color del entorno reflejado por el especular
	float3 color = (diffuse + specular) * tex2D(DiffMap, IN.uv0).xyz;

	return float4(color, 1.0f);
}