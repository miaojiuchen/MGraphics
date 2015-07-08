//globals
cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

//typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangend : TANGENT;
	float4 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex :TEXCOORD0;
	float3 normal:NORMAL;
	float3 tangent : TANGEND;
	float3 binormal : BINORMAL;
};

PixelInputType BumpMapVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangend, (float3x3)world);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)world);
	output.binormal = normalize(output.binormal);

	return output;
}
