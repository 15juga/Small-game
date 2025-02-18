
struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW :POSITION;
	float3 TangentW : TANGENT;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD0;
	float4 ProjTex : TEXCOORD1;
};

VertexOut VS(VertexIn vin) {
	VertexOut vout; 
	
	// Transform to light′s projective space.
	vout.ProjTex = mul(float4(vIn.posL, 1.0f),
		gLightWorldViewProjTexture);
	
	return vout;

}
float4 PS(VertexOut pin) : SV_Target
{
	// Complete projection by doing division by w.
	pin.ProjTex.xyz /= pin.ProjTex.w;
	// Depth in NDC space.
	float depth = pin.ProjTex.z;
	// Sample the texture using the projective tex-coords.
	float4 c = gTextureMap.Sample(sampler, pin.ProjTex.xy);
	
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}