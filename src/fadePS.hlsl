
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer FadeBuffer
{
	float fadeAmount;
	float3 padding;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 FadePixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;

	color = shaderTexture.Sample(SampleType, input.tex) * fadeAmount;

	return color;
}
