/**
*   Super Sampling Anti Aliasing    (SSAA)
*/
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);


// サンプル数指定
// 出来ればシェーダパラメータで書き換えたいが非常に面倒なので後回し
#define SAMPLING_NUM 4

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv		: TEXCOORD;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    float2 size;
    float dx, dy;
    tex.GetDimensions(size.x, size.y);

    dx = 1.0f / size.x / SAMPLING_NUM;
    dy = 1.0f / size.y / SAMPLING_NUM;

    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
    for (uint x = 0; x < SAMPLING_NUM; ++x)
    {
        for (uint y = 0; y < SAMPLING_NUM; ++y)
        {
            color.rgb += tex.Sample(smp, input.uv + float2(dx * x, dy * y)) / float(SAMPLING_NUM * SAMPLING_NUM);
        }
    }

    saturate(color);
    
    return color;
}