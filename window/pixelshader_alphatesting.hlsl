Texture2D diffuseTex : register(t0);
SamplerState samplerLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

float4 PS(PS_INPUT input) : SV_Target0
{
    float4 albedo;
    
    albedo = diffuseTex.Sample(samplerLinear, input.TexCoords);
    
    if (albedo.a < 0.5)
    {
        discard;
    }
    
    return albedo;
}

