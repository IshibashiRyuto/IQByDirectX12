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
    float4x4 modelMatrix : INSTANCE_MATRIX;
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
    output.position = mul(wvp, mul(input.modelMatrix, float4(input.position, 1.0f)));
    input.modelMatrix._14_24_34 = float3(0.0f, 0.0f, 0.0f);
    output.normal = normalize(mul(input.modelMatrix, float4(input.normal, 1.0f)).xyz);
	//output.normal = input.normal;

	output.uv = input.uv;
	return output;
}


float4 PSMain(PSInput input) : SV_Target
{
    float3 light = normalize(float3(0.0f, 1.0f, -1.0f));
    float brightness = dot(input.normal, light) * 0.8f + 0.2f;
    
    
    return float4(brightness, brightness, brightness, 1.0f);
    //return tex.Sample(smp, input.uv);
}