struct SKYMAP_VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

cbuffer CONSTANT_BUFFER : register(b0) {
	matrix mat;
}

SKYMAP_VS_OUTPUT main( float3 pos : POSITION )
{
	SKYMAP_VS_OUTPUT output;
	output.pos = mul(mat, float4(pos, 1.0)).xyww;
	output.pos.z -= 0.0001;
	output.texCoord = pos;

	return output;
}