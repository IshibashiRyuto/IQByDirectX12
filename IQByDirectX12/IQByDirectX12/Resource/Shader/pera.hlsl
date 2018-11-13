Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct VSInput
{
	float3 position : POSITION;
	float2 uv		: TEXCOORD;
};


struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv		: TEXCOORD;
};

typedef VSOutput PSInput;

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.position = float4(input.position, 1.0f);
	output.uv = input.uv;

	return output;
}

float4 PSMain(PSInput input) : SV_Target
{
	float2 size;
	tex.GetDimensions(size.x, size.y);

	float du = 1.0f / size.x / 2.0f;
	float dv = 1.0f / size.y / 2.0f;

	float4 color = float4(0.0f,0.0f,0.0f,0.0f);
	color += tex.Sample(smp, input.uv) * 0.25f;
	color += tex.Sample(smp, input.uv + float2(du, 0)) * 0.25f;
	color += tex.Sample(smp, input.uv + float2(0, dv)) * 0.25f;
	color += tex.Sample(smp, input.uv + float2(du, dv)) * 0.25f;

	return color;
}
