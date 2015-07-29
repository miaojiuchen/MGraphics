
cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ReflectionBuffer
{
	matrix reflectionMatrix;
};


struct VertexInputType
{
	float4 position:POSITION; 
	float2 tex:TEXCOORD0;
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float2 tex:TEXCOORD0;
	float4 reflectionPosition:TEXCOORD1;
};

PixelInputType ReflectionVertexShader(VertexInputType input)
{
	PixelInputType output;
	matrix reflectProjectWorld;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;

	reflectProjectWorld = mul(reflectionMatrix, proj);
	reflectProjectWorld = mul(world, reflectProjectWorld);

	output.reflectionPosition = mul(input.position, reflectProjectWorld);

	return output;
}