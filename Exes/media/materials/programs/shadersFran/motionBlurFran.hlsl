// Textura
sampler SceneSampler	: register(s0);
sampler DepthMap			: register(s1);

// Parametros uniformes
float4x4 viewProjectionMatrix;
float4x4 inverseViewProjMatrix;

float4x4 previousViewProjMatrix;

//float4x4 previousViewMatrix;
//float4x4 previousProjectionMatrix;

// Información de entrada del vertex shader
struct VsInput {
	float4 vertex 				: POSITION;
	float2 texcoord 			: TEXCOORD0;
};

// Información de salida del vertex shader
struct VsOutput {
	float4 position 			: POSITION;
	float2 uvmain		 		: TEXCOORD0;
};

// Información de entrada del fragment shader
struct PsInput {
	float2 uvmain 			: TEXCOORD0;
};

//__________________________________________________________________

// VERTEX SHADER
VsOutput vertex_main(const VsInput IN) {
	VsOutput OUT;
	
	OUT.position = mul(viewProjectionMatrix, IN.vertex);
	OUT.uvmain = IN.texcoord;
	
	return OUT;
}

//__________________________________________________________________

// FRAGMENT SHADER
float4 fragment_main(const PsInput IN) : COLOR {
	// EXTRAER LA POSICION DEL PIXEL EN EL MUNDO

	// Get the depth buffer value at this pixel. 
	float zOverW = tex2D(DepthMap, IN.uvmain);
	// H is the viewport position at this pixel in the range -1 to 1.
	float4 H = float4(IN.uvmain.x * 2 - 1, (1 - IN.uvmain.y) * 2 - 1, zOverW, 1);
	// Transform by the view-projection inverse.
	float4 D = mul(inverseViewProjMatrix, H);
	// Divide by w to get the world position.
	float4 worldPos = D / D.w;
	
	
	// COMPUTAR LOS VECTORES DE VELOCIDAD DE CADA PIXEL CON RESPECTO AL FRAME ANTERIOR
	
	// Current viewport position
	float4 currentPos = H;
	// Use the world position, and transform by the previous view-projection matrix.
	float4 previousPos = mul(previousViewProjMatrix, worldPos);
	// Convert to nonhomogeneous points [-1,1] by dividing by w.
	previousPos /= previousPos.w;
	// Use this frame's position and last frame's to compute the pixel velocity.
	float2 velocity = (currentPos - previousPos)/2.0f;
	
	// BLURREAR SEGUN LA VELOCIDAD A LA QUE NOS MOVEMOS
	
	// Get the initial color at this pixel.
	float4 color = tex2D(SceneSampler, IN.uvmain);
	float2 texCoord = IN.uvmain + velocity;
	int numSamples = 8;
	for(int i = 1; i < numSamples; ++i, texCoord += velocity) {
		// Sample the color buffer along the velocity vector.
		float4 currentColor = tex2D(SceneSampler, texCoord);
		// Add the current color to our color sum.
		color += currentColor;
	}
	
	// Average all of the samples to get the final blur color.
	float4 finalColor = color / numSamples;
	
	return finalColor;
}
