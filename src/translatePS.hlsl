

Texture2D shaderTexture;
SamplerState SampleType;

cbuffer TranslationBuffer
{
	float textureTranslation;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 TranslatePixelShader(PixelInputType input) : SV_TARGET
{
	input.tex.x += textureTranslation;
	input.tex.y += textureTranslation;

	return shaderTexture.Sample(SampleType, input.tex);
}