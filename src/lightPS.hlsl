////////////////////////////////////
//Filename:light.ps
////////////////////////////////////

//globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

//typedefs
struct PixelInputType
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD0;
	float3 normal:NORMAL;
	float3 viewDirection : TEXCOORD1;
};

//pixel shader
float4 LightPixelShader(PixelInputType input):SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;

	//sample the pixel color from the texture using the sampler at this texture coordinate location
	textureColor = shaderTexture.Sample(SampleType,input.tex);

	//set the default output color to be the ambient light value for all pixels
	color = ambientColor;

	//initialize the specular color
	specular = float4(0.0f,0.0f,0.0f,0.0f);

	//invert the light direction for calculations
	lightDir = -lightDirection;

	//calculate the intensity of light on this pixel
	lightIntensity = saturate(dot(input.normal,lightDir));

	if(lightIntensity > 0.0f)
	{
		//determine the final diffuse color based on the diffuse color and the amount of the light intensity
		color += (diffuseColor * lightIntensity);

		//saturate the ambient and diffuse color
		color = saturate(color);

		//calculate the refection vector based on the light intendity, normal vector, and light direction
		reflection = normalize(2 * lightIntensity *input.normal - lightDir);

		//determine the amount of specular light based on the reflection vector, viewing direction, and specular power
		specular = pow(saturate(dot(reflection,input.viewDirection)),specularPower);
	}

	//multiply the texture pixel and the final diffuse color to get the final pixel color result
	//color = color * textureColor;

	//add the specular component last to the output color
	color = saturate(color + specular);

	return color;
}

