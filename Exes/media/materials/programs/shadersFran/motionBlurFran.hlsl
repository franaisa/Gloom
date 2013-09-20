// Texturas
sampler SceneSampler	: register(s0); // Render de la escena
sampler DepthMap			: register(s1); // Render del Z-Buffer

#define NUM_SAMPLES 18

// Parametros uniformes
// Inversa de la matriz de vista-proyeccion del frame actual
float4x4 inverseViewProjMatrix;
// Matriz de vista-proyeccion del frame anterior. La pasamos corregida
// desde codigo para que solo tenga en cuenta la rotacion del frame anterior
// y no la posicion. De esta manera conseguimos que el blur solo afecta a la
// rotacion de camara
float4x4 previousViewProjMatrix;
// Tiempo transcurrido entre el frame anterior y el actual
float deltaTime;
// Factor de blur, controla cuanto se difumina la imagen
float blurriness;

// Información de entrada del fragment shader
struct PsInput {
	float2 uvmain 			: TEXCOORD0;
};

//__________________________________________________________________

// FRAGMENT SHADER - Implementacion basada en la propuesta en GPU Gems 3
float4 fragment_main(const PsInput IN) : COLOR {
   // En primer lugar, calculamos la posicion del pixel en el mundo
   
   // Sacamos el nivel de profunidad del pixel
	float zOverW = tex2D(DepthMap, IN.uvmain).x;
   // H es la posicion del viewport en este pixel en el rango [-1, 1]
	float4 H = float4(IN.uvmain.x * 2 - 1, (1 - IN.uvmain.y) * 2 - 1, zOverW, 1);
   // Transformamos usando la inversa de la matriz vista-proyeccion actual
	float4 D = mul(inverseViewProjMatrix, H);
   // Dividimos por w para obtener la posicion del pixel en el mundo
	float4 worldPos = D / D.w;
	
   // En segundo lugar, obtenemos el vector velocidad del pixel teniendo en cuanta
   // la posicion que el pixel tenia en el frame anterior
	
	// Posicion actual del viewport
	float4 currentPos = H;
   // Situamos el pixel en el frame anterior usando la posicion del mundo calculada
   // previamente y la matriz de vista-proyeccion del frame anterior
	float4 previousPos = mul(previousViewProjMatrix, worldPos);
   // Convertimos a coordenadas no homogeneas [-1, 1] diviendo por w.
	previousPos /= previousPos.w;
   // Usamos la posicion del frame actual y la del frame anterior para calcular
   // el vector de velocidad teniendo en cuenta el tiempo transcurrido entre frames
	float2 velocity = ((currentPos - previousPos) / deltaTime).xy;	
	// Escalamos la velocidad para controlar la cantidad de blur generado
	velocity *= blurriness;
	
	// En tercer lugar, difuminamos la escena en funcion del vector velocidad
   // calculado previamente
	
   // Sacamos el color inicial del pixel
	float4 color = tex2D(SceneSampler, IN.uvmain);
	float2 texCoord = IN.uvmain + velocity;
	for(int i = 1; i < NUM_SAMPLES; ++i, texCoord += velocity) {
      // Sampleamos el buffer de color a lo largo del vector velocidad
		float4 currentColor = tex2D(SceneSampler, texCoord);
      // Sumamos el color actual a la suma total
		color += currentColor;
	}
	
   // Calculamos la media de todas las muestras para obtener el color final de blur
	float4 finalColor = color / NUM_SAMPLES;
	
   // Devolvemos el color calculado
	return finalColor;
}
