Texture2D<float4> tex : register(t0);
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

	dx = 1.0f / size.x;
	dy = 1.0f / size.y;

	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	color += tex.Sample(smp, input.uv + float2(-dx, 0.0f)) * -1.0f;
	color += tex.Sample(smp, input.uv + float2(0.0f, -dy)) * -1.0f;

	color += tex.Sample(smp, input.uv + float2(0.0f, 0.0f)) * 5.0f;

	color += tex.Sample(smp, input.uv + float2(dx, 0.0f)) * -1.0f;
	color += tex.Sample(smp, input.uv + float2(0.0f, dy)) * -1.0f;

	return float4(color.rgb, 1.0f);
}