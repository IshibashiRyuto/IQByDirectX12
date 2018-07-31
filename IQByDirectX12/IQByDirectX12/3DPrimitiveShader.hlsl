Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct VSInput
{
    float3 position     : POSITION;
    float3 normal       : NORMAL;
    float2 uv           : TEXCORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCORD;
};

typedef VSOutput PSInput;

VSOutput VSMain( VSInput input )
{
    VSOutput output;
    output.position = float4(input.position, 1.0f);
    output.normal = input.normal;
    output.uv = input.uv;
	return output;
}


float4 PSMain(PSInput input) : SV_Target
{
    return tex.Sample(smp, input.uv);
 //   return float4(input.uv, 1.0f, 1.0f);
}