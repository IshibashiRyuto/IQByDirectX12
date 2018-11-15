Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv		: TEXCOORD;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    float2 uv = input.uv;

    uv = (uv - float2(0.5f, 0.5f)) * 2;

    float2 fishEyeUV;
    fishEyeUV = float2(sin(uv.x * (3.141592f / 2.0f)), sin(uv.y * (3.141592f / 2.0f)));

    fishEyeUV = fishEyeUV * float2(0.5, 0.5) + float2(0.5f, 0.5f);

	float3 color = tex.Sample(smp, fishEyeUV).rgb;
	float b = dot(color, float3(0.1f, 0.3f, 0.6f));
	return float4(b, b, b, 1.0f);
}