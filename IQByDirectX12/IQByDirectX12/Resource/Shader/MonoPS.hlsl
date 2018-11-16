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
	float3 color = tex.Sample(smp, input.uv).rgb;
	float b = dot(color, float3(0.1f, 0.3f, 0.6f));
	return float4(b, b, b, 1.0f);
}