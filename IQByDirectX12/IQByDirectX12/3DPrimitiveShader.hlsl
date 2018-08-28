Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer mat:register(b0)
{
	float4x4 wvp;	/// wvpçsóÒ
}

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
    output.position = mul(wvp, float4(input.position, 1.0f));
	output.normal = input.normal;
    output.uv = input.uv;
	return output;
}


float4 PSMain(PSInput input) : SV_Target
{
    float3 light = normalize(float3(-1.0f, 1.0f, -1.0f));
    float brightness = dot(input.normal, light);
    
    return float4(brightness, brightness, brightness, 1.0f);
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
    //return tex.Sample(smp, input.uv);
}