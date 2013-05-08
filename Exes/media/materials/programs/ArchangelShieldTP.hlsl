float4x4 ViewProjectionMatrix;
//float4x4 worldMatrix;


float tiempo;
float alpha;
sampler shieldTexture: register(s0);

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
	pos.xyz +=  In.normal * 0.1; 
	vsOut.position = mul(ViewProjectionMatrix, pos);
	vsOut.uv0 = In.uv0;
	return vsOut;
}


float4 mainPS(VsOutput psIn) : COLOR 
{
	float4 shield = tex2D(shieldTexture, psIn.uv0);
	shield.w = alpha;
	return shield;
}