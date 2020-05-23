struct vs_out {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer CONSTANT_BUFFER : register(b0) {
	matrix mat;
}

vs_out main( float3 pos : POSITION, float2 tex : TEXCOORD)
{
	vs_out output;
	output.pos = mul(mat, float4(pos, 1.0));
	output.tex = tex;

	return output;
}