// @author Francisco Aisa García

// Registros de texturas
sampler DiffMap					: register(s0); // RGB = Difuso ; A = Especular
sampler BumpedLightmap	: register(s1); // Bumped Lightmap
sampler NormalMap			: register(s2); // Bump Map
sampler SpecMap				: register(s3); // Mapa de especulares

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
	float2 uv1					: TEXCOORD1;
};

struct VsOutput_AMB {
	float4 position 			: POSITION;
	float2 uv0 					: TEXCOORD0;
	float2 uv1					: TEXCOORD1;
};

struct PsInput_AMB {
	float2 uv0					: TEXCOORD0;
	float2 uv1					: TEXCOORD1;
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

float3 computeSpecular(float3 Ks, float attenuation, float spotEffect, float3 lightColor, float3 normal, float3 halfAngle, float shininess) {
	float specularLight = pow( max(dot(halfAngle, normal), 0), shininess );
	return Ks * spotEffect * attenuation * lightColor * specularLight;
}

//________________________________________________________________________

float3 computeDiffuse(float3 Kd, float attenuation, float spotEffect, float3 lightColor, float3 normal, float3 lightDirection, out float diffuseTerm) {
	diffuseTerm = max( dot(lightDirection, normal), 0 );
	return Kd * spotEffect * attenuation * lightColor * diffuseTerm;
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
	OUT.uv1 = IN.uv1;
	
	return OUT;
}

// ========================================================================
// 									   FRAGMENT SHADER
// ========================================================================

// Fragment para calcular ambiente
float4 fragment_AMB(const PsInput_AMB IN) : COLOR {
	float3 color = Ka * tex2D(DiffMap, IN.uv0).xyz * tex2D(BumpedLightmap, IN.uv1).xyz;
	
	return float4(color, 1.0f);
}

//________________________________________________________________________

// Fragment para calcular difuso + especular. El especular se saca de una textura especifica.
float4 fragment_DIFF_ALPHASPEC(const PsInput IN) : COLOR {
	// Obtenemos las normales y las descomprimimos al rango [-1, 1]
	float3 normal = tex2D(NormalMap, IN.uv0).xyz;
	normal = expand(normal);
	normal = normalize(normal);
	
	// Calculamos la atenuacion. Si el rango es 0, quiere decir que no queremos
	// atenuacion. Ojito con esto porque ogre automaticamente apaga las luces fuera
	// de rango si no estan dentro del frustrum de camara.
	float attenuation;
	if(lightAttenuation.x != 0.0f)
		attenuation = saturate( computeAttenuation(lightPosition, lightAttenuation, IN.position.xyz) );
	else
		attenuation = 1.0f;
	
	// Calculamos el efecto del sopt light
	float spotEffect;
	if( !isNotSpotLight(spotLightParams) )
		spotEffect = dualConeSpotlight(IN.position.xyz);
	else
		spotEffect = 1.0f;
	
	// Calculamos el color del pixel en base a la luz recibida
	float3 diffuse, specular;
	float diffuseTerm;
	// Si se trata de una luz estatica solo realizamos el calculo de especular
	// ya que el calculo de difusos lo obtenemos directamente del lightmapping
	if( customLightParams.w == 1.0f ) {
		diffuse = float3(0.0f, 0.0f, 0.0f);
		// Sacamos el especular del canal alfa del mapa de normales.
		specular = computeSpecular(tex2D(NormalMap, IN.uv0).www, attenuation, spotEffect, lightColor.xyz, normal, IN.halfAngle, shininess);
	}
	// Si se trata de una luz dinamica calculamos la luz difusa y especular
	else {
		diffuse = computeDiffuse(Kd, attenuation, spotEffect, lightColor.xyz, normal, IN.lightDirection, diffuseTerm);
	
		// Si existe luz difusa puede haber luz especular. En este caso sacamos el especular del canal alfa del mapa de normales.
		if(diffuseTerm > 0.0f)
			specular = computeSpecular(tex2D(NormalMap, IN.uv0).www, attenuation, spotEffect, lightColor.xyz, normal, IN.halfAngle, shininess);
		else
			specular = float3(0.0f, 0.0f, 0.0f);
	}
	
	// El color resultante es difuso + especular por difuso
	float3 color = (diffuse + specular) * tex2D(DiffMap, IN.uv0).xyz;
	
	return float4(color, 1.0f);
}

//________________________________________________________________________

// Fragment para calcular difuso + especular. El especular se obtiene de una textura especifica
// en lugar de sacarlo del canal alfa de la textura de normales.
float4 fragment_DIFF_SPEC(const PsInput IN) : COLOR {
	// Obtenemos las normales y las descomprimimos al rango [-1, 1]
	float3 normal = tex2D(NormalMap, IN.uv0).xyz;
	normal = expand(normal);
	normal = normalize(normal);
	
	// Calculamos la atenuacion. Si el rango es 0, quiere decir que no queremos
	// atenuacion. Ojito con esto porque ogre automaticamente apaga las luces fuera
	// de rango si no estan dentro del frustrum de camara.
	float attenuation;
	if(lightAttenuation.x != 0.0f)
		attenuation = saturate( computeAttenuation(lightPosition, lightAttenuation, IN.position.xyz) );
	else
		attenuation = 1.0f;
	
	// Calculamos el efecto del sopt light
	float spotEffect;
	if( !isNotSpotLight(spotLightParams) )
		spotEffect = dualConeSpotlight(IN.position.xyz);
	else
		spotEffect = 1.0f;
	
	// Calculamos el color del pixel en base a la luz recibida
	float3 diffuse, specular;
	float diffuseTerm;
	// Si se trata de una luz estatica solo realizamos el calculo de especular
	// ya que el calculo de difusos lo obtenemos directamente del lightmapping
	if( customLightParams.w == 1.0f ) {
		diffuse = float3(0.0f, 0.0f, 0.0f);
		specular = computeSpecular(tex2D(SpecMap, IN.uv0).xyz, attenuation, spotEffect, lightColor.xyz, normal, IN.halfAngle, shininess);
	}
	// Si se trata de una luz dinamica calculamos la luz difusa y especular
	else {
		diffuse = computeDiffuse(Kd, attenuation, spotEffect, lightColor.xyz, normal, IN.lightDirection, diffuseTerm);
	
		// Si existe luz difusa puede haber luz especular.
		if(diffuseTerm > 0.0f)
			specular = computeSpecular(tex2D(SpecMap, IN.uv0).xyz, attenuation, spotEffect, lightColor.xyz, normal, IN.halfAngle, shininess);
		else
			specular = float3(0.0f, 0.0f, 0.0f);
	}
	
	// El color resultante es difuso + especular por difuso
	float3 color = (diffuse + specular) * tex2D(DiffMap, IN.uv0).xyz;
	
	return float4(color, 1.0f);
}
