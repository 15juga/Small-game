
cbuffer VertexRotationMatrix : register(b0)
{
    float4x4 world;
}

cbuffer worldViewProjectionMatrixBuffer : register(b1)
{
    float4x4 WVP;
};

cbuffer viewProjectionMatrixBuffer : register(b2)
{
    matrix view;
    matrix projection;
};


cbuffer lightProjection : register(b3)
{
    matrix lightView;
    matrix lightProjection;
    
    float4 lPos;
    float4 eyePos; // main camera position
};

struct VertexShaderInput
{
    float3 position :   POSITION;
    float2 uv :         UV;
    float3 normal :     NORMAL;
};

struct VertexShaderOutput
{
	float4 position :       SV_POSITION;
	float2 uv :             UV;
	float3 normal :         NORMAL;
	float4 lightSpacePos :  LIGHT_POSITION;
	float3 lightRay :       LIGHT_RAY;
	float3 view :           VIEW;
	float4 worldPos :       WORLDPOS;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
	float4 pos = float4(input.position, 1.0f);

    // Transform the vertex position into projected space.
    float4 modelPos = mul(pos, mul(world, WVP));
	output.position = modelPos;

    // Transform the vertex position into projected space from the POV of the light.
    float4 lightPosition = mul(pos, world);
	lightPosition = mul(lightPosition, lightView);
	lightPosition = mul(lightPosition, lightProjection);
	output.lightSpacePos = lightPosition;
    
    // create a vector from the light to vertex position
	float3 lightRay = lPos.xyz - modelPos.xyz;
	output.lightRay = lightRay;

    // set view to be a ray from the camera to vertex position
	output.view = eyePos.xyz - modelPos.xyz;

    // Transform the vertex normal into world space.
	float4 norm = float4(input.normal, 0.0f);
	norm = normalize(mul(norm, world));
	output.normal = norm.xyz;

    output.uv = input.uv; 
	output.worldPos = mul(float4(input.position, 1.0f), world); 
    
    return output;
} 