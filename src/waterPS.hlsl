

SamplerState SampleType;
Texture2D reflectionTexture;
Texture2D refractionTexture;
Texture2D normalTexture;

cbuffer WaterBuffer
{
	float waterTranslation;
	float reflectRefractScale;
	float2 padding;
};

struct PIT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectionPos : TEXCOORD1;
	float4 refractionPos : TEXCOORD2;
};

float4 WaterPixelShader(PIT input):SV_TARGET
{
	float2 reflectionTexcoord;
	float2 refractionTexcoord;
	float4 normalMap;
	float3 normal;
	float4 reflectionColor;
	float4 refractionColor;
	float4 color;

	input.tex.y += waterTranslation;

	reflectionTexcoord.x = input.reflectionPos.x / input.reflectionPos.w / 2.0f + 0.5f;
	reflectionTexcoord.y = -input.reflectionPos.y / input.reflectionPos.w / 2.0f + 0.5f;

	refractionTexcoord.x = input.refractionPos.x / input.refractionPos.w / 2.0f + 0.5f;
	refractionTexcoord.y = -input.refractionPos.y / input.refractionPos.w / 2.0f + 0.5f;

	normalMap = normalTexture.Sample(SampleType, input.tex);

	normal = (normalMap.xyz * 2.0f) - 1.0f;

	reflectionTexcoord = reflectionTexcoord + (normal.xy * reflectRefractScale);
	refractionTexcoord = refractionTexcoord + (normal.xy * reflectRefractScale);

	reflectionColor = reflectionTexture.Sample(SampleType, reflectionTexcoord);
	refractionColor = refractionTexture.Sample(SampleType, refractionTexcoord);

	color = lerp(reflectionColor, refractionColor, 0.5f);

	return color;
}