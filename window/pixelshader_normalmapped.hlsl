Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
SamplerState samplerLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

float4 PS(PS_INPUT input) : SV_Target0{
    float3 lightDir = normalize(float3(1.0, 1.0, 1.0));
    float3 lightColour = float3(1.0, 1.0, 1.0);
    float PI = 3.1415926;

    float4 albedo = diffuseTex.Sample(samplerLinear, input.TexCoords);
    if (albedo.a < 0.5) discard;

    float3 normal = normalize(input.Normal);
    float3 tangent = normalize(input.Tangent);
    float3 binormal = normalize(cross(input.Normal, tangent));
    float3x3 TBN = float3x3(tangent, binormal, normal);

    float3 mapNormal = normalTex.Sample(samplerLinear, input.TexCoords).xyz;
    mapNormal = mapNormal * 2.0 - 1.0;
    float3 localLightDir = normalize(mul(lightDir, transpose(TBN)));

    float3 diffuse = (albedo.rgb / PI) * lightColour * max(dot(mapNormal, localLightDir), 0.0);
    return float4(diffuse, albedo.a);
}