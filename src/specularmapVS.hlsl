
cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex:TEXCOORD0;
	float3 normal:NORMAL;
	float3 tangent:TANGENT;
	float3 binormal:BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex:TEXCOORD0;
	float3 normal:NORMAL;
	float3 tangent:TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDirection:TEXCOORD1;
};

PixelInputType SpecularMapVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	input.position.w = 1.0f;
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangent, (float3x3)world);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)world);
	output.binormal = normalize(output.binormal);

	worldPosition = mul(input.position, world);

	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	output.viewDirection = normalize(output.viewDirection);

	return output;
}

