struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.position = input.position;
	output.tex = input.tex;
	return output;
}