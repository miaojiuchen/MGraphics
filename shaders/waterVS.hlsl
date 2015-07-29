
cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ReflectionBuffer
{
	matrix reflection;
};

struct VIT
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PIT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectionPos : TEXCOORD1;
	float4 refractionPos : TEXCOORD2;
};

PIT WaterVertexShader(VIT input)
{
	PIT output;
	matrix reflectProjWorld;
	matrix viewProjWorld;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;

	reflectProjWorld = mul(reflection, proj);
	reflectProjWorld = mul(world, reflectProjWorld);

	output.reflectionPos = mul(input.position, reflectProjWorld);

	viewProjWorld = mul(view, proj);
	viewProjWorld = mul(world, viewProjWorld);

	output.refractionPos = mul(input.position, viewProjWorld);

	return output;
}