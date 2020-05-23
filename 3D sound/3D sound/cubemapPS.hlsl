struct SKYMAP_VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

TextureCube skymap;
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 main(SKYMAP_VS_OUTPUT input) : SV_TARGET
{
	return skymap.Sample(samLinear, input.texCoord);
}