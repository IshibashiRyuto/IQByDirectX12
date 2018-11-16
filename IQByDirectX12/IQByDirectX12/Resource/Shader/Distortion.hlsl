Texture2D<float4> srcTex : register(t0);
Texture2D<float> depthMap : register(t1);
SamplerState smp : register(s0);

Texture2D<float4> distortionMap : register(t1);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 PSMain(PSInput input) : SV_Target
{
    float2 uv = input.uv;
    if(uv.x + uv.y < 1)
    {
        float b = depthMap.Sample(smp, input.uv);
        return float4(b, b, b, 1);

    }
    return srcTex.Sample(smp, input.uv);
}