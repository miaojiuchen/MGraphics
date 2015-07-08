Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
};

struct PIT
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD0;
	float3 normal:NORMAL;
	float clip : SV_ClipDistance0;
};

float4 RefractionPixelShader(PIT input):SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	color = ambientColor;
	lightDir = -lightDirection;
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		color += diffuseColor * lightIntensity;
	}

	color = saturate(color);
	color *= textureColor;

	return color;
}