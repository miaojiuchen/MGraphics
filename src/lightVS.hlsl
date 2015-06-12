////////////////////////////////////
//Filename:light.vs
////////////////////////////////////

//globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

//typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCORRD1;
};

//vertex shader
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	//change the position vector to be 4 units for proper matrix calculations
	input.position.w = 1.0f;

	//calculate the position of the vertex against the world, view, and projection matrices
	output.position = mul(input.position,worldMatrix);
	output.position = mul(output.position,viewMatrix);
	output.position = mul(output.position,projectionMatrix);

	//store the texture coordinates for the pixel shader
	output.tex = input.tex;

	//calculate the nornal vector against the world matrix only
	output.normal = mul(input.normal,(float3x3)worldMatrix);

	//normalize the normal vector
	output.normal = normalize(output.normal);

	//calculate the position of the vertex in the world
	worldPosition = mul(input.position,worldMatrix);

	//determine the viewing direction based on the position of the camera and the position of the vertex in the world
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	//normalize the viewing direction vector
	output.viewDirection = normalize(output.viewDirection);

	return output;
}