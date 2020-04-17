struct vs_out {
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

vs_out main( float3 pos : POSITION, float4 color : COLOR)
{
	vs_out output;
	output.pos = float4(pos, 1.0);
	output.col = color;

	return output;
}