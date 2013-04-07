float4x4 ViewProjectionMatrix;
//float4x4 worldMatrix;

float tiempo;

struct VsInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv0 : TEXCOORD0;
};

struct VsOutput
{
	float2 uv0 : TEXCOORD0;
	float4 position : POSITION;
};

VsOutput vsMain(const VsInput In) {	
	VsOutput vsOut;
	float4 pos = In.position;
	float random = clamp(tiempo,1, 5);
	pos.x += In.normal.x * sin(tiempo*4) * 0.3;
	pos.y += In.normal.y * sin(tiempo*4) * 0.3;
	pos.z += In.normal.z * sin(tiempo*4) * 0.3;
		
	pos.x += (sin(pos.y+tiempo*random) + sin(pos.z + tiempo*random))*0.05;

	vsOut.position = mul(ViewProjectionMatrix, pos);
	vsOut.uv0 = In.uv0;
	//uv0.a=0.5;
	return vsOut;
}


float4 mainPS() : COLOR 
{

	return float4(0, 0.0, 0.0, 0.0);
}