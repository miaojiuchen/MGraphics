

cbuffer PerFrameBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer FogBuffer
{
	float fogStart;
	float fogEnd;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};