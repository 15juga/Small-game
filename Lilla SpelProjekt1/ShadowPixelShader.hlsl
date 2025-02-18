struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

struct outputColor
{
	float4 color;
};

outputColor main(PixelInputType input) : SV_TARGET
{
	outputColor colour;

	float depthValue;
	float4 color;


	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	depthValue = input.depthPosition.z / input.depthPosition.w;

	color = float4(depthValue, depthValue, depthValue, 1.0f);
	colour.color = color;
	return colour;
}