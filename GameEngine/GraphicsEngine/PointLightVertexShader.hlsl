// Define shader inputs
cbuffer PointLightConstantBuffer : register(b0)
{
    float4x4 projection;
    float4x4 view;
    float4x4 world;
    float3 lightPosition;
    float4 eyePosition;
    float intensity;
};

//Texture2D diffuseTexture : register(t0);
//SamplerState sampleType : register(s0);

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

// Define shader outputs
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
    float3 toEye : TEXCOORD1;
    float3 toLight : TEXCOORD2;
};

// Vertex shader
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Transform vertex position, normal, and texture coordinate to world space
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.normal = normalize(mul(input.normal, world));
    output.texcoord = float2(input.texcoord.x, 1. - input.texcoord.y);

    // Calculate vectors from vertex to eye and light positions
    output.toEye = eyePosition - input.position;
    output.toLight = lightPosition - input.position;

    return output;
}