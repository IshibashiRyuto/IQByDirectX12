Texture2D<float4> srcTex : register(t0);
SamplerState smp : register(s0);

Texture2D<float4> distortionMap : register(t1);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 PSMain(PSInput input) : SV_Target
{
    return float4(1, 1, 1, 1);

}