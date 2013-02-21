float4x4 ViewProjectionMatrix;

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

	float4 pos = float4(In.position.xyz,0);
	pos.y += (sin(pos.x+tiempo) + sin(pos.z+tiempo));
    vsOut.position = mul(pos,ViewProjectionMatrix);
	//vsOut.position = In.position;
    vsOut.uv0 = In.uv0;
	//vsOut.color = float4(1.0f, 1.0f, 1.0f, 0.0f);

	return vsOut;
}


float4 mainPS() : COLOR 
{
	return float4(0, 0.0, 0.0, 0.0);
}
