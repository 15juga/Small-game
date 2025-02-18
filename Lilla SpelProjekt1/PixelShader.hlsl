Texture2D testTexture : TEXTURE : register(t0);
Texture2D shadowMapTexture : register(t3); 

SamplerState testSampler : SAMPLER : register(s0);
SamplerState clampSampler : register(s1);

struct PixelShaderOutput
{
	float4 lightPassOutput : SV_Target0;
};

cbuffer LightCalc : register(b0)
{
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
	float4 lightPos;
	float4 camPos;
	float4 lightDirection;
    

	float lightRange;
	float lightStrenght;
	float att0;
	float att1;
	float att2;
    float pad1;
    float pad2;
    float pad3;
};

cbuffer Light : register(b2)//not used
{
	matrix lightMatrix;
};

struct PixelShaderInput
{
	float4 position :		SV_POSITION;
	float2 uv :				UV;
	float3 normal :			NORMAL;
	float4 lightSpacePos :	LIGHT_POSITION;
	float3 lightRay :		LIGHT_RAY;
	float3 view :			VIEW;
	float4 worldPos :		WORLDPOS;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float lightIntensity;
	float depthValue;
	float4 finalOutput;
	float2 shadowTexCoords;
	
	float bias = 0.001f;
	float4 albedo = testTexture.Sample(testSampler, input.uv);
	float4 output = lightAmbient;
	
    //Compute texture coordinates for the current point's location on the shadow map.
	shadowTexCoords.x = (input.lightSpacePos.x / input.lightSpacePos.w * 0.5f) + 0.5f;
	shadowTexCoords.y =  -(input.lightSpacePos.y / input.lightSpacePos.w * 0.5f) + 0.5f;
	float pixelDepth = input.lightSpacePos.z / input.lightSpacePos.w;
		
    // Check if the pixel texture coordinate is in the view frustum of the 
    // light before doing any shadow work.
	// This will always be true because our light covers the whole map
	if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
		(saturate(shadowTexCoords.y) == shadowTexCoords.y))
	{
		// sample the depth from the shadow texture
		depthValue = shadowMapTexture.Sample(clampSampler, shadowTexCoords).r;

		// offset the pixels depth with a small bias
		pixelDepth = (pixelDepth - bias);
		
		if (pixelDepth < depthValue)
		{
			// this if statement should be removed once every object has correct normal values
			if (input.normal.x != 0 || 
				input.normal.y != 0 || 
				input.normal.z != 0)
			{
				lightIntensity = saturate(dot(input.normal, camPos.xyz));
			}
			else
				lightIntensity = 0.5f;
			
			if (lightIntensity > 0.0f)
			{
				// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
				output += (lightDiffuse * lightIntensity);

				// Saturate the final light color.
				output = saturate(output);
			}
		}
	}

	// multiply with the albedo, which is really just the pixel color taken from a texture
	output = output * albedo;
	return  output;
}