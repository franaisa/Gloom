// Textura que define los colores que puede adquirir el objeto
sampler GradeSampler	: register(s0);

// Constantes para necesarias para el calculo del ruido de perlin
#define BSIZE 32
#define FULLSIZE 66
#define NOISEFRAC 0.03125

// Parametros uniformes
// Reloj, usado para modificar la forma y el color del objeto
float Timer;
// Matriz de modelado vista y proyeccion
float4x4 WvpXf;
// Parametros de configuracion del ruido
float Displacement;
float Sharpness;
float ColorSharpness;
float Speed;
float TurbDensity;
float ColorRange;
float4x4 NoiseXf;


// Funciones para calcular el ruido de perlin (version 3d, 2d y 1d)

float3 s_curve(float3 t) { return t*t*( float3(3,3,3) - float3(2,2,2)*t); }
float2 s_curve(float2 t) { return t*t*( float2(3,3) - float2(2,2)*t); }
float  s_curve(float  t) { return t*t*(3.0-2.0*t); }

// Version 3D
float noise(float3 v, const uniform float4 pg[FULLSIZE]) {
    v = v + (10000.0f).xxx;   // Para evitar numeros negativos

    float3 i = frac(v * NOISEFRAC) * BSIZE;   // Indice entre 0 y BSIZE-1
    float3 f = frac(v);           	 						// Fraccion de la posicion

    // Tabla de consultas para permutaciones
    float2 p;
    p.x = pg[ i[0]     ].w;
    p.y = pg[ i[0] + 1 ].w;
    p = p + i[1];

    float4 b;
    b.x = pg[ p[0] ].w;
    b.y = pg[ p[1] ].w;
    b.z = pg[ p[0] + 1 ].w;
    b.w = pg[ p[1] + 1 ].w;
    b = b + i[2];

	// Calculamos los productos vectoriales entre gradientes y vectores
    float4 r;
    r[0] = dot( pg[ b[0] ].xyz, f );
    r[1] = dot( pg[ b[1] ].xyz, f - float3(1.0f, 0.0f, 0.0f) );
    r[2] = dot( pg[ b[2] ].xyz, f - float3(0.0f, 1.0f, 0.0f) );
    r[3] = dot( pg[ b[3] ].xyz, f - float3(1.0f, 1.0f, 0.0f) );

    float4 r1;
    r1[0] = dot( pg[ b[0] + 1 ].xyz, f - float3(0.0f, 0.0f, 1.0f) );
    r1[1] = dot( pg[ b[1] + 1 ].xyz, f - float3(1.0f, 0.0f, 1.0f) );
    r1[2] = dot( pg[ b[2] + 1 ].xyz, f - float3(0.0f, 1.0f, 1.0f) );
    r1[3] = dot( pg[ b[3] + 1 ].xyz, f - float3(1.0f, 1.0f, 1.0f) );

    // Interpolamos
    f = s_curve(f);
    r = lerp( r, r1, f[2] );
    r = lerp( r.xyyy, r.zwww, f[1] );
    return lerp( r.x, r.y, f[0] );
}

// Version 2D
float noise(float2 v, const uniform float4 pg[FULLSIZE]) {
    v = v + (10000.0f).xx;	// Para evitar numeros negativos

    float2 i = frac(v * NOISEFRAC) * BSIZE;   // Indice entre 0 y BSIZE-1
    float2 f = frac(v);            							// Fraccion de la posicion

    // Tabla de consultas para permutaciones
    float2 p;
    p[0] = pg[ i[0]   ].w;
    p[1] = pg[ i[0]+1 ].w;
    p = p + i[1];

    // Calculamos los productos vectoriales entre gradientes y vectores
    float4 r;
    r[0] = dot( pg[ p[0] ].xy,   f);
    r[1] = dot( pg[ p[1] ].xy,   f - float2(1.0f, 0.0f) );
    r[2] = dot( pg[ p[0]+1 ].xy, f - float2(0.0f, 1.0f) );
    r[3] = dot( pg[ p[1]+1 ].xy, f - float2(1.0f, 1.0f) );

    // Interpolamos
    f = s_curve(f);
    r = lerp( r.xyyy, r.zwww, f[1] );
    return lerp( r.x, r.y, f[0] );
}

// Version 1D
float noise(float v, const uniform float4 pg[FULLSIZE]) {
    v = v + 10000.0f;	// Para evitar numeros negativos

    float i = frac(v * NOISEFRAC) * BSIZE;  	// Indice entre 0 y BSIZE-1
    float f = frac(v);            							// Fraccion de la posicion

    // Calculamos los productos vectoriales entre gradientes y vectores
    float2 r;
    r[0] = pg[i].x * f;
    r[1] = pg[i + 1].x * (f - 1.0f);

    // Interpolamos
    f = s_curve(f);
    return lerp( r[0], r[1], f);
}

// Datos de entrada para el vertex shader
struct appData {
    float4 Position     	: POSITION;
    float4 Normal       	: NORMAL;
    float4 TexCoord0    : TEXCOORD0;
};

// Datos de salida del vertex shader y de entrada al pixel shader
struct vbombVertexData {
    float4 HPosition		: POSITION;
    float4 Color0			: COLOR0;
};

// VERTEX SHADER
vbombVertexData mainVS(appData IN) {
	// Tabla de consulta usada por las funciones de ruido
	const float4 NTab[FULLSIZE] = {
		-0.569811,0.432591,-0.698699,0,
		0.78118,0.163006,0.60265,1,
		0.436394,-0.297978,0.848982,2,
		0.843762,-0.185742,-0.503554,3,
		0.663712,-0.68443,-0.301731,4,
		0.616757,0.768825,0.168875,5,
		0.457153,-0.884439,-0.093694,6,
		-0.956955,0.110962,-0.268189,7,
		0.115821,0.77523,0.620971,8,
		-0.716028,-0.477247,-0.50945,9,
		0.819593,-0.123834,0.559404,10,
		-0.522782,-0.586534,0.618609,11,
		-0.792328,-0.577495,-0.196765,12,
		-0.674422,0.0572986,0.736119,13,
		-0.224769,-0.764775,-0.60382,14,
		0.492662,-0.71614,0.494396,15,
		0.470993,-0.645816,0.600905,16,
		-0.19049,0.321113,0.927685,17,
		0.0122118,0.946426,-0.32269,18,
		0.577419,0.408182,0.707089,19,
		-0.0945428,0.341843,-0.934989,20,
		0.788332,-0.60845,-0.0912217,21,
		-0.346889,0.894997,-0.280445,22,
		-0.165907,-0.649857,0.741728,23,
		0.791885,0.124138,0.597919,24,
		-0.625952,0.73148,0.270409,25,
		-0.556306,0.580363,0.594729,26,
		0.673523,0.719805,0.168069,27,
		-0.420334,0.894265,0.153656,28,
		-0.141622,-0.279389,0.949676,29,
		-0.803343,0.458278,0.380291,30,
		0.49355,-0.402088,0.77119,31,
		-0.569811,0.432591,-0.698699,0,
		0.78118,0.163006,0.60265,1,
		0.436394,-0.297978,0.848982,2,
		0.843762,-0.185742,-0.503554,3,
		0.663712,-0.68443,-0.301731,4,
		0.616757,0.768825,0.168875,5,
		0.457153,-0.884439,-0.093694,6,
		-0.956955,0.110962,-0.268189,7,
		0.115821,0.77523,0.620971,8,
		-0.716028,-0.477247,-0.50945,9,
		0.819593,-0.123834,0.559404,10,
		-0.522782,-0.586534,0.618609,11,
		-0.792328,-0.577495,-0.196765,12,
		-0.674422,0.0572986,0.736119,13,
		-0.224769,-0.764775,-0.60382,14,
		0.492662,-0.71614,0.494396,15,
		0.470993,-0.645816,0.600905,16,
		-0.19049,0.321113,0.927685,17,
		0.0122118,0.946426,-0.32269,18,
		0.577419,0.408182,0.707089,19,
		-0.0945428,0.341843,-0.934989,20,
		0.788332,-0.60845,-0.0912217,21,
		-0.346889,0.894997,-0.280445,22,
		-0.165907,-0.649857,0.741728,23,
		0.791885,0.124138,0.597919,24,
		-0.625952,0.73148,0.270409,25,
		-0.556306,0.580363,0.594729,26,
		0.673523,0.719805,0.168069,27,
		-0.420334,0.894265,0.153656,28,
		-0.141622,-0.279389,0.949676,29,
		-0.803343,0.458278,0.380291,30,
		0.49355,-0.402088,0.77119,31,
		-0.569811,0.432591,-0.698699,0,
		0.78118,0.163006,0.60265,1
	};

	// Aqui comienza el shader
    vbombVertexData OUT;
    float4 noisePos = TurbDensity * mul( NoiseXf, IN.Position + (Speed*Timer) );
    float i = ( noise(noisePos.xyz, NTab) + 1.0f ) * 0.5f;

    float cr = 1.0 - ( 0.5 + ColorRange * (i - 0.5) );
    cr = pow(cr, ColorSharpness);
    OUT.Color0 = float4( (cr).xxx, 1.0f );
    
	// Desplazamiento sobre la normal
    float ni = pow( abs(i), Sharpness);
    i -=  0.5;
	
    float4 Nn = float4( normalize(IN.Position).xyz, 0 );
    float4 NewPos = IN.Position - ( Nn * (ni - 0.5) * Displacement );
	
    OUT.HPosition = mul(WvpXf, NewPos);
    return OUT;
}

// FRAGMENT SHADER
float4 hotPS(vbombVertexData IN) : COLOR {
	float2 nuv = float2(IN.Color0.x, 0.5);
	float4 nc = tex2D(GradeSampler, nuv);
	return nc;
}