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
	pos.y += (sin(pos.x+tiempo) + sin(pos.z + tiempo));
	vsOut.position = mul(ViewProjectionMatrix, pos);
	vsOut.uv0 = In.uv0;
/*

	//float4 pos = mul(In.position,worldMatrix);
	float4 pos = mul(worldMatrix, In.position);
	//pos = mul(pos, worldMatrix);
	pos.y += (sin(pos.x+tiempo) + sin(pos.z+tiempo));
    vsOut.position = mul(ViewProjectionMatrix, pos);
	//vsOut.position = In.position;
    vsOut.uv0 = In.uv0;
	//vsOut.color = float4(1.0f, 1.0f, 1.0f, 0.0f);
*/
	return vsOut;
}


float4 mainPS() : COLOR 
{
	return float4(0, 0.0, 0.0, 0.0);
}
