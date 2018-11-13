Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv		: TEXCOORD;
};

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 color = tex.Sample(smp, input.uv);
	uint4 i_color = color * 8;
	color = i_color;
	color = color / 8;
	return float4(color.rgb, 1.0f);
}