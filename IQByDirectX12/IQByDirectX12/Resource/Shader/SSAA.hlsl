Texture2D<float4> tex : register(t0);
Texture2D<float> depthMap : register(t1);
SamplerState smp : register(s0);

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

    dx = 1.0f / size.x / 2.0f;
    dy = 1.0f / size.y / 2.0f;

    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    color += tex.Sample(smp, input.uv + float2(0.0f, 0.0f)) * 0.25f;
    color += tex.Sample(smp, input.uv + float2(dx, 0.0f)) * 0.25f;
    color += tex.Sample(smp, input.uv + float2(0.0f, dy)) * 0.25f;
    color += tex.Sample(smp, input.uv + float2(dx, dy)) * 0.25f;

    saturate(color);
    
    return color;
}