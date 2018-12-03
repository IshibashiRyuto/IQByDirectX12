SamplerState smp : register(s0);
Texture2D<float> shadowMap: register(t0);

cbuffer mat : register(b0)
{
    float4x4 wvp; // wvp行列
    float4x4 world; // world行列
    float4x4 view; // view行列
    float4x4 projection; // projection行列
    float4 eyePos; // 視点
    float4 eyeDir; // 視線ベクトル
    float4 upper; // 上方向ベクトル
    float4 right; // 右方向ベクトル
}

cbuffer light : register(b1)
{
    float4x4 lightWvp; // wvp行列
    float4x4 lightWorld; // world行列
    float4x4 lightView; // view行列
    float4x4 lightProjection; // projection行列
    float4 lightEyePos; // 視点
    float4 lightEyeDir; // 視線ベクトル
    float4 lightUpper; // 上方向ベクトル
    float4 lightRight; // 右方向ベクトル
}

cbuffer material : register(b2)
{
    float4 diffuseColor;
    float3 specularColor;
    float specularity;
    float3 ambientColor;
}

struct VSInput
{
    float3 position     : POSITION;
    float3 normal       : NORMAL;
    float2 uv           : TEXCOORD;
    float4x4 modelMatrix : INSTANCE_MATRIX;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 origPosition : POSITION1;
    float3 lightDepth : POSITION2;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
};

typedef VSOutput PSInput;

VSOutput VSMain( VSInput input )
{
    VSOutput output;
    
	output.position = mul(wvp, mul(input.modelMatrix, float4(input.position, 1.0f)));
    output.origPosition = mul(world, mul(input.modelMatrix, float4(input.position, 1.0f)));
    
	input.modelMatrix._14_24_34 = float3(0.0f, 0.0f, 0.0f);
    output.normal = normalize(mul(world, mul(input.modelMatrix, float4(input.normal, 1.0f))).xyz);
    output.lightDepth = mul(lightWvp, mul(input.modelMatrix, float4(input.position, 1.0f)));
	
	output.uv = input.uv;

	return output;
}


float4 PSMain(PSInput input) : SV_Target
{
    float3 light = lightEyeDir.xyz;
    float brightness = saturate(dot(input.normal, -light));

    float2 shadowMapUV = (float2(1, 1) + input.lightDepth.xy * float2(1, -1)) * 0.5;
    if (shadowMapUV.x > 0 && shadowMapUV.x < 1 && shadowMapUV.y > 0 && shadowMapUV.y < 1)
    {
        if (shadowMap.Sample(smp, shadowMapUV) + 0.00005 < input.lightDepth.z)
        {
            return float4(ambientColor, diffuseColor.a);
        }
    }

    // 視線ベクトル
    float3 vray = normalize(input.origPosition - eyePos.xyz);
    
    float spec = pow(saturate(dot(reflect(light, input.normal), -vray)), specularity);

    float3 color = saturate(diffuseColor.xyz * brightness + ambientColor + spec * specularColor);

    return float4(color, diffuseColor.a);
}

