cbuffer World : register(b0)
{
	matrix model;
};

cbuffer Light : register(b1)
{
    matrix modelViewProjection;
};

struct VertexShaderInput
{
    float3 position :   POSITION;
    float2 uv :         UV;
    float3 normal :     NORMAL;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float4 pos = float4(input.position, 1.0f);

    // Transform the vertex position into projected space.
	pos = mul(pos, mul(model, modelViewProjection));
    output.position = pos;

    return output;
}
