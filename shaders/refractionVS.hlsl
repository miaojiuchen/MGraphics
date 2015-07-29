cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ClipPlaneBuffer
{
	float4 clipPlane;
};

struct VIT
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PIT
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD0;
	float3 normal:NORMAL;
	float clip : SV_ClipDistance0;
};

PIT RefractionVertexShader(VIT input)
{
	PIT output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;

	output.normal = normalize(mul(input.normal, (float3x3)world));
	
	output.clip = dot(mul(input.position, world), clipPlane);

	return output;
}