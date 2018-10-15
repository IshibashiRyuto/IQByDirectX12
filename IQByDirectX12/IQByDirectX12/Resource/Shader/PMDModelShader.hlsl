Texture2D<float4> surfaceTexture : register(t0);
Texture2D<float4> addSphereTexture : register(t1);
Texture2D<float4> mulSphereTexture : register(t2);
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
	float3 light = normalize(float3(1.0f, -1.0f, 1.0f));
	float3 lightSpecularColor = float3(1.0f, 1.0f, 1.0f);
	float3 lightDiffuseColor = float3(1.0f, 1.0f, 1.0f);
	float3 lightAmbientColor = float3(1.0f, 1.0f, 1.0f);

	float3 eyePosition = float3(0.0f, 20.0f, -15.0f);

	float3 vray = normalize(input.origPosition - eyePosition);
	float3 up = float3 (0, 1, 0);
	
	float brightness = saturate( dot(input.normal, -light) );
    
	float attenuation = 1.0f;

	/// アンビエントカラー計算
	float3 modelAmbientColor = ambientColor * attenuation;
	modelAmbientColor = modelAmbientColor * lightAmbientColor;

	// ディフューズカラー計算
	float3 modelDiffuseColor = diffuseColor.rgb * brightness;
	modelDiffuseColor = modelDiffuseColor * lightDiffuseColor;

	// スペキュラ計算
	float3 modelSpecularColor = specularColor * attenuation;
	float spec = saturate(pow(dot(reflect(light, input.normal), -vray), specularity));
	modelSpecularColor = modelSpecularColor * spec;


	// スフィアマップ計算
	float3 reflectVray = normalize( reflect(vray, input.normal) );

	float3 vrayAxisX = cross(up, vray);
	float3 vrayAxisY = cross( vray, vrayAxisX);
	float2 sphereUV = float2(dot(vrayAxisX, input.normal), dot(vrayAxisY, input.normal));  //reflectVray.xy * float2(0.5f, -0.5) + float2(0.5, 0.5);
	sphereUV = sphereUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	float4 texColor = surfaceTexture.Sample(smp, input.uv);// *mulSphereTexture.Sample(smp, sphereUV) + addSphereTexture.Sample(smp, sphereUV);
	
	float3 modelColor = modelDiffuseColor + modelAmbientColor;
	modelColor = modelColor * texColor.xyz;
	modelColor = modelColor * mulSphereTexture.Sample(smp, sphereUV) +addSphereTexture.Sample(smp, sphereUV);
	modelColor = saturate( modelColor + modelSpecularColor);


	return float4 (modelColor, diffuseColor.a);
}