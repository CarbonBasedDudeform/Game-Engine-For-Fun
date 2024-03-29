cbuffer WorldViewProjection {
	matrix projection;
	matrix view;
	matrix world;
	float4 lightPosition;
	float4 eyePosition;
};

struct VertexInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.tex = float2(input.tex.x, 1. - input.tex.y);
	return output;
}