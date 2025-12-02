struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

cbuffer bufferName{
float time;
float2 lights[4];
};

float dist(float2 screenspace_pos, float2 light) {
	return (length(screenspace_pos - light) / (50.0 * abs(cos(time))));
}


float4 PS(PS_INPUT input) : SV_Target0 {
    return float4(abs(normalize(input.Normal)) * 0.9f, 1.0);
}
