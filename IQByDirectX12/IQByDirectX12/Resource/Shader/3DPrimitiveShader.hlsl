SamplerState smp : register(s0);
Texture2D<float> shadowMap: register(t0);

cbuffer mat : register(b0)
{
    float4x4 wvp; // wvp�s��
    float4x4 world; // world�s��
    float4x4 view; // view�s��
    float4x4 projection; // projection�s��
    float4 eyePos; // ���_
    float4 eyeDir; // �����x�N�g��
    float4 upper; // ������x�N�g��
    float4 right; // �E�����x�N�g��
}

cbuffer light : register(b1)
{
    float4x4 lightWvp; // wvp�s��
    float4x4 lightWorld; // world�s��
    float4x4 lightView; // view�s��
    float4x4 lightProjection; // projection�s��
    float4 lightEyePos; // ���_
    float4 lightEyeDir; // �����x�N�g��
    float4 lightUpper; // ������x�N�g��
    float4 lightRight; // �E�����x�N�g��
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

    // �����x�N�g��
    float3 vray = normalize(input.origPosition - eyePos.xyz);
    
    float spec = pow(saturate(dot(reflect(light, input.normal), -vray)), specularity);

    float3 color = saturate(diffuseColor.xyz * brightness + ambientColor + spec * specularColor);

    return float4(color, diffuseColor.a);
}

