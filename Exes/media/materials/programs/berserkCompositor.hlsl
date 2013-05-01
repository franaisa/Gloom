
sampler RT : register(s0);
struct VsInput
{
	float2 uv0 : TEXCOORD0;
	float radius;
	float darkness;
	
};

struct VsOutput
{
	float4 color;
};

VsOutput vsMain(const VsInput In) {

	VsOutput vsOut;
	
	
	float4 color=tex2D(RT,In.uv0);
	float2 inTex = In.uv0 - 0.5;
	float vignette  = 1 - dot(inTex, inTex);
	vsOut.color.rgb *= saturate(pow(vignette, radius) + darkness);
	
	return vsOut;
}
