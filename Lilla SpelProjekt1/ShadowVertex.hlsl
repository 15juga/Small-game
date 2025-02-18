struct VertexShaderInput
{
	float3 position : POSITION
	float2 uv : UV
	float3 normal : NORMAL
	float3 tan : TANGENT
	float3 bitTan : BITTANGENT
};

struct VertexOut 
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 TangentW : TANGENT;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD0;
	float4 ProjTex : TEXCOORD1;
}

VertexOut VS(VertexIn vertexInput)
{
	VertexOut vOut;

	//TODO

	//tranform to lights projective space
	vOut.ProjTex = mul(float4(vertexInput.posL, 1.0f), )

}
float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}