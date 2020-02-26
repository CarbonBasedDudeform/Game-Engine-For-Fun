struct PixelInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 main(PixelInput input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
	//return shaderTexture.Sample(SampleType, input.tex);
}