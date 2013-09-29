float4x4 ViewProjectionMatrix;
//float4x4 worldMatrix;

float tiempo;
float alpha;
float4 color;

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

VsOutput mainVS(const VsInput In) {

	VsOutput vsOut;
	float4 pos = In.position;
	pos.xyz +=  abs(0.5*(sin(tiempo*3))) * In.normal; 
	vsOut.position = mul(ViewProjectionMatrix, pos);
	vsOut.uv0 = In.uv0;
	return vsOut;
}


float4 mainPS() : COLOR 
{
	return float4(color.rgb, alpha*abs(sin(tiempo*3))+0.05);
}