Texture2D<float4> tex : register(t0);
Texture2D<float> dep : register(t1);

SamplerState smp : register(s0);

//cbuffer world : register(b0)
//{
//    float2 windowSize;
//}

struct VSInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3x3 instMat : INSTANCE_MATRIX;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

typedef VSOutput PSInput;

float2 ConvertPosFromWindowCoordinate(float2 pos)
{
    float2 windowSize = float2(1280, 720);
    float2 ret;

    ret = pos;
    ret.x = ret.x / (windowSize.x);
    ret.y = ret.y / (windowSize.y);
  
    return ret;
}

VSOutput VSMain(VSInput input)
{
    float3 pos = float3(input.position.xy, 1.0f);

    pos = mul(input.instMat, pos);

    VSOutput output;

    output.position = input.position;
    output.uv = input.uv;

    return output;
}

float4 PSMain(PSInput input) :SV_Target
{
    if(input.uv.x + input.uv.y < 1.0f)
    {
        return float4(input.uv, 1, 1);
    }
    return float4(0.0f, 0.0f, 0.0f, 0.0f);

}