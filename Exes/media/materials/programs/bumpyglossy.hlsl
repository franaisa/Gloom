//http://www.eriksdigitalportfolio.com/2012/bumpy-glossy-shader-in-ogre3d/
void bumpyGlossy_VS(
  float3 Position  : POSITION,
  float4 UV    : TEXCOORD0,
  float4 Normal  : NORMAL,
  float4 Tangent  : TANGENT0,
  float4 Binormal  : BINORMAL0,

  out float4 vsOutHPosition    : POSITION,
  out float4 vsOutTexCoord    : TEXCOORD0,
  out float3 vsOutWorldNormal    : TEXCOORD1,
  out float3 vsOutWorldPos    : TEXCOORD2,
  out float3 vsOutWorldEyeVec    : TEXCOORD3,
  out float3 vsOutWorldTangent  : TEXCOORD4,
  out float3 vsOutWorldBinorm    : TEXCOORD5,
  out float3 vsOutLightVec1    : TEXCOORD6,

  uniform float4x4 WorldITXf,
  uniform float4x4 WorldXf,
  uniform float4x4 ViewIXf,
  uniform float4x4 WvpXf,
  uniform float4    lightPos
  )
{
  vsOutWorldNormal = mul(WorldITXf, Normal).xyz;
  vsOutWorldTangent = mul(WorldITXf, Tangent).xyz;
  vsOutWorldBinorm = mul(WorldITXf, Binormal).xyz;
  float4 Po = float4(Position.xyz,1.0);
  float3 Pw = mul(WorldXf, Po).xyz;
  vsOutWorldPos = Pw;
  vsOutLightVec1 = lightPos.xyz - Pw;
  vsOutTexCoord = UV;
  vsOutWorldEyeVec = ViewIXf[3].xyz - Pw;
  vsOutHPosition = mul(WvpXf, Po);
}

void bumpyGlossy_FS(
  float4 vsOutHPosition    : POSITION,
  float4 vsOutTexCoord    : TEXCOORD0,
  float3 vsOutWorldNormal    : TEXCOORD1,
  float3 vsOutWorldPos    : TEXCOORD2,
  float3 vsOutWorldEyeVec    : TEXCOORD3,
  float3 vsOutWorldTangent  : TEXCOORD4,
  float3 vsOutWorldBinorm    : TEXCOORD5,
  float3 vsOutLightVec1    : TEXCOORD6,

  out float4 fsOutColor  : COLOR,

  uniform float4   AmbiColor,
  uniform float4    SpecularColor,
  uniform float    LightIntensity,
  uniform float4    DiffuseColor,
  uniform float   SpecExpon,
  uniform float   Kd,
  uniform float   Ks,
  uniform float   Kr,
  uniform float   Bumpy,

  uniform sampler2D   colorSampler   : register(s0),
  uniform sampler2D   normalSampler   : register(s1),
  uniform sampler2D   glossSampler   : register(s2),
  uniform samplerCUBE envSampler     : register(s3)
  )
{
  float3 map = tex2D(colorSampler,vsOutTexCoord.xy).rgb;
  float3 bumps = Bumpy * (tex2D(normalSampler,vsOutTexCoord.xy).xyz-(0.5).xxx);
  float gloss = Ks * tex2D(glossSampler,vsOutTexCoord.xy).x;
  float3 Nn = normalize(vsOutWorldNormal);
  float3 Tn = normalize(vsOutWorldTangent);
  float3 Bn = normalize(vsOutWorldBinorm);
  float3 Nb = Nn + (bumps.x * Tn + bumps.y * Bn);
  Nb = normalize(Nb);
  float3 Vn = normalize(vsOutWorldEyeVec);

  float falloff = LightIntensity/dot(vsOutLightVec1,vsOutLightVec1);
  float3 Ln = normalize(vsOutLightVec1);
  float3 Hn = normalize(Vn + Ln);
  float hdn = dot(Hn,Nb);
  float ldn = dot(Ln,Nb);
  float4 litV = lit(ldn,hdn,SpecExpon);
  float3 incident = falloff * litV.y * SpecularColor.rgb;
  float3 diffContrib = incident;
  float3 specContrib = litV.z * gloss * incident;

  float3 reflVect = reflect(Vn,Nb);
  float3 reflColor = Kr * texCUBE(envSampler,float4(reflVect, 1)).rgb;

  float3 result = (DiffuseColor.rgb*map) * (Kd*diffContrib + AmbiColor.rgb + specContrib + reflColor);
  fsOutColor = float4(result.rgb,1.0);
}