Texture2D<float4> surfaceTexture : register(t0);
Texture2D<float4> addSphereTexture : register(t1);
Texture2D<float4> mulSphereTexture : register(t2);
Texture2D<float4> toonTexture : register(t3);
SamplerState smp : register(s0);

cbuffer mat:register(b0)
{
	float4x4 wvp;			// wvp行列
	float4x4 world;			// world行列
	float4x4 view;			// view行列
	float4x4 projection;	// projection行列	
}

cbuffer material : register(b1)
{
	float4 diffuseColor;
	float specularity;
	float3 specularColor;
	float3 ambientColor;
	uint isUseTexture;
	uint sphereFlag;
}

struct VSInput
{
	float3 position     : POSITION0;
	float3 normal       : NORMAL;
	float2 uv           : TEXCOORD;
	float4x4 modelMatrix : INSTANCE_MATRIX;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 origPosition	: POSITION1;
	float3 normal   : NORMAL;
	float2 uv       : TEXCOORD;
};

typedef VSOutput PSInput;

VSOutput VSMain(VSInput input)
{
	VSOutput output;

	output.position = mul(wvp, mul(input.modelMatrix, float4(input.position, 1.0f)));
	output.origPosition = mul(world, mul(input.modelMatrix, float4(input.position, 1.0f)));

	output.normal = normalize(mul(world,mul(input.modelMatrix, float4(input.normal, 0.0f))).xyz);

	output.uv = input.uv;


	return output;
}


float4 PSMain(PSInput input) : SV_Target
{
	float3 light = normalize(float3(-0.41f, -0.82f, 0.41f));
	float3 lightSpecularColor = float3(0.6f, 0.6f, 0.6f);
	float3 lightDiffuseColor = float3(0.6f, 0.6f, 0.6f);
	float3 lightAmbientColor = float3(1.0f, 1.0f, 1.0f);

	float3 eyePosition = float3(0.0f, 15.0f, -50.0f);

	float3 vray = normalize(input.origPosition - eyePosition);
	float3 up = float3 (0, 1, 0);
	
	float brightness = saturate( dot(input.normal, -light) );
    
	float attenuation = 1.0f;

	/// アンビエントカラー計算
	float3 modelAmbientColor = ambientColor * attenuation;
	modelAmbientColor = modelAmbientColor * lightAmbientColor;
    // ディフューズカラー計算
    float normalRad = acos(brightness);
    float3 modelDiffuseColor = diffuseColor.rgb * lightDiffuseColor;

	// スペキュラ計算
	float3 modelSpecularColor = specularColor * attenuation * lightSpecularColor;
    float spec = pow(saturate(dot(reflect(light, input.normal), -vray)), (100 - specularity)/10.0f);
	modelSpecularColor = modelSpecularColor * spec;

    
	// スフィアマップ計算
	
	float3 vrayAxisX = cross(up, vray);
	float3 vrayAxisY = cross( vray, vrayAxisX);
	float2 sphereUV = float2(dot(vrayAxisX, input.normal), dot(vrayAxisY, input.normal));
	
	sphereUV = sphereUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);


	float4 texColor = surfaceTexture.Sample(smp, input.uv);
	
    float4 modelColor = saturate(float4(modelDiffuseColor + modelAmbientColor, diffuseColor.a));
    
	modelColor = modelColor * mulSphereTexture.Sample(smp, sphereUV) +addSphereTexture.Sample(smp, sphereUV);
    modelColor = saturate(modelColor + float4(modelSpecularColor, 0.0f));
    modelColor = modelColor * texColor * toonTexture.Sample(smp, float2(0.0f, saturate(normalRad / (3.141592f))));

    
    return modelColor;
}