Texture2D<float4> tex : register(t0);
Texture2D<float4> lumaTex : register(t1);

SamplerState smp : register(s0);


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float2 DownSamplingPos(float2 pos, float2 downSampleRate)
{
    int2 _pos = pos * downSampleRate;
    return float2(_pos) / downSampleRate;
}

float GaussianDist(float x, float y, float rho)
{
    return exp(-(x * x + y * y) / (2.0f * rho * rho));

}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);
    //float2 size;
    //lumaTex.GetDimensions(size.x, size.y);
    float totalWeight = 0;
    //float downScale = 1.0;
    //for (int samplingCount = 0; samplingCount < 8; ++samplingCount, downScale *=2.0)
    //{
    //    float2 downSampleRate = size / float(downScale);
    //    float2 offset = float2(1, 1) / downSampleRate;
    //    for (int x = -1; x <= 1; ++x)
    //    {
    //        for (int y = -1; y <= 1; ++y)
    //        {
    //            float gaussWeight = GaussianDist(x, y, 10.0);
    //            color += gaussWeight * lumaTex.Sample(smp, DownSamplingPos(input.uv + offset * float2(x, y), downSampleRate));
    //            totalWeight += gaussWeight;
    //        }
    //    }
    //}
    
    totalWeight = 0;
    color = float4(0, 0, 0, 0);
    // mipmapCalc
    for (int mipLevel = 0; mipLevel < 4; ++mipLevel)
    {
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                float gaussWeight = GaussianDist(x, y, 10.0);
                color += gaussWeight * lumaTex.SampleLevel(smp, input.uv, mipLevel, int2(x, y));
                totalWeight += gaussWeight;
            }
        }
    }
    
    color.rgb /= totalWeight;
    color.a = 1;
    color.rgb += tex.Sample(smp, input.uv).rgb;
    saturate(color);

    return color;
}