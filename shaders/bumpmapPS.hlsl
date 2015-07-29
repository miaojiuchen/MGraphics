Texture2D shaderTextures[2];
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
};

//typedefs
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

float4 BumpMapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	textureColor = shaderTextures[0].Sample(SampleType, input.tex);

	float4 bumpMap;
	bumpMap = shaderTextures[1].Sample(SampleType, input.tex);
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	float3 bumpNormal;
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);

	float3 lightDir;
	lightDir = -lightDirection;

	float lightIntensity;
	lightIntensity = saturate(dot(bumpNormal, lightDir));

	float4 color;
	color = saturate(diffuseColor * lightIntensity);
	color = color *textureColor;

	return color;
}