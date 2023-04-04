Texture2D diffuseTexture : register(t0);
SamplerState sampleType : register(s0);

cbuffer PointLightConstantBuffer : register(b0)
{
    float4x4 projection;
    float4x4 view;
    float4x4 world;
    float3 lightPosition;
    float4 eyePosition;
    float intensity;
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

// Pixel shader
float4 main(VS_OUTPUT input) : SV_TARGET
{
    float3 ambient = float3(0.1f, 0.1f, 0.1f);
    float3 specular = float3(1.0f, 1.0f, 1.0f);

    float shininess = 32.0f;
    // Sample texture color
    float4 textureColor = diffuseTexture.Sample(sampleType, input.texcoord);

    // Calculate normal vector
    float3 normal = input.normal;

    // Calculate vectors to eye and light positions
    float3 toEye = normalize(input.toEye);
    float3 toLight = normalize(input.toLight);

    // Calculate the attenuation factor based on the distance and the attenuation coefficients
    float attenuation = 1.0f / (intensity + (intensity * toLight) + (intensity * toLight * toLight));

    // Calculate ambient lighting
    float3 ambientLight = ambient * textureColor.rgb;

    // Calculate diffuse lighting
    float NdotL = max(0.0f, dot(normal, toLight));
    float3 diffuseLight = textureColor.rgb * NdotL;

    // Calculate specular lighting
    float3 reflected = reflect(-toLight, normal);
    float3 specularLight = pow(max(0.0f, dot(toEye, reflected)), shininess) * specular * textureColor.a * attenuation;

    // Combine lighting and return final color
    return float4(ambientLight + diffuseLight + specularLight, textureColor.a);
}