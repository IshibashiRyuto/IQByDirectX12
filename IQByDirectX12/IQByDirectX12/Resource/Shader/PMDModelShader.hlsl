Texture2D<float4> tex : register(t0);
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
	float3 diffuseColor;
	float alpha;
	float specularity;
	float3 specularColor;
	float3 ambientColor;
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
	float2 uv       : TEXCORD;
};

typedef VSOutput PSInput;

VSOutput VSMain(VSInput input)
{
	VSOutput output;

	output.position = mul(wvp, mul(input.modelMatrix, float4(input.position, 1.0f)));
	output.origPosition = mul(world, mul(input.modelMatrix, float4(input.position, 1.0f)));

	input.modelMatrix._14_24_34 = float3(0.0f, 0.0f, 0.0f);
	output.normal = normalize(mul(input.modelMatrix, float4(input.normal, 1.0f)).xyz);

	output.uv = input.uv;

	return output;
}


float4 PSMain(PSInput input) : SV_Target
{
	float3 light = normalize(float3(0.0f, 1.0f, -1.0f));
	float3 lightSpecularColor = float3(1.0f, 1.0f, 1.0f);
	float3 lightDiffuseColor = float3(1.0f, 1.0f, 1.0f);
	float3 lightAmbientColor = float3(1.0f, 1.0f, 1.0f);

	float3 eyePosition = float3(0.0f, 30.0f, -15.0f);

	float3 vray = normalize(input.origPosition - eyePosition);
	
	float brightness = dot(input.normal, light);
    
	float attenuation = 1.0f;

	/// アンビエントカラー計算
	float3 modelAmbientColor = ambientColor * attenuation;
	modelAmbientColor.x = modelAmbientColor.x * lightAmbientColor.x;
	modelAmbientColor.y = modelAmbientColor.y * lightAmbientColor.y;
	modelAmbientColor.z = modelAmbientColor.z * lightAmbientColor.z;

	// ディフューズカラー計算
	float3 modelDiffuseColor = diffuseColor * brightness;
	modelDiffuseColor.x = modelDiffuseColor.x * lightDiffuseColor.x;
	modelDiffuseColor.y = modelDiffuseColor.y * lightDiffuseColor.y;
	modelDiffuseColor.z = modelDiffuseColor.z * lightDiffuseColor.z;

	// スペキュラ計算
	float3 modelSpecularColor = specularColor * attenuation;
	float spec = saturate(pow(dot(reflect(-light, input.normal), -vray), specularity));
	modelSpecularColor = modelSpecularColor * spec;

	float4 modelColor = float4(modelAmbientColor, 1.0f) + float4(modelDiffuseColor, alpha) + float4(modelSpecularColor, 1.0f) ;

	return modelColor;
}