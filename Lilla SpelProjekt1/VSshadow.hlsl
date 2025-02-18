
cbuffer VertexRotationMatrix : register(b0)
{
    float4x4 world;
}

cbuffer worldViewProjectionMatrixBuffer : register(b2)
{
    float4x4 WVP;
};

cbuffer LightBuffer2
{
    float3 lightPosition;
    float padding;
};
//cbuffer viewProjectionMatrixBuffer : register(b3)
//{
//    matrix worldMatrix;
//};

//cbuffer lightProjection : register(b4)
//{
//    matrix lightProjectionMatrix;
//};


struct VertexShaderInput
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 worldPos : WORLDPOS;
};

VertexShaderOutput main(VertexShaderInput input)
{
    PixelInputType output;
    float4 worldPosition;
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    lightPosition = float3(0.0f, 20.0f, 0.0f);
    //VertexShaderOutput output;

    output.position = mul(float4(input.position, 1.0f), mul(world, WVP));

    // Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(input.position, mul(world, wvp));

    // Store the texture coordinates for the pixel shader.
    output.tex = input.uv;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)world);

    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(float4(input.position, 1.0f), worldMatrix);

    // Determine the light position based on the position of the light and the position of the vertex in the world.
    output.lightPos = lightPosition.xyz - worldPosition.xyz;

    // Normalize the light position vector.
    output.lightPos = normalize(output.lightPos);
    //   output.uv = input.uv; 
       //output.normal = mul(float4(input.normal, 0.0f), world);
       //output.worldPos = mul(float4(input.position, 1.0f), world);

    return output;
}