struct PixelInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 main(PixelInput input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.tex);
}