Texture2D<float4> surfaceTexture : register(t0);
Texture2D<float4> addSphereTexture : register(t1);
Texture2D<float4> mulSphereTexture : register(t2);
Texture2D<float4> toonTexture : register(t3);
SamplerState smp : register(s0);

cbuffer mat:register(b0)
{
	float4x4 wvp;			// wvp�s��
	float4x4 world;			// world�s��
	float4x4 view;			// view�s��
	float4x4 projection;	// projection�s��
    float4 eyePos;          // ���_
    float4 eyeDir;          // �����x�N�g��
    float4 upper;           // ������x�N�g��
    float4 right;           // �E�����x�N�g��
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

cbuffer boneMatrix : register(b2)
{
    float4x4 boneMatrix[512];
}

cbuffer material : register(b3)
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
	min16uint2	boneno	: BONENO;
	min16uint	weight	: WEIGHT;
	float4x4 modelMatrix : INSTANCE_MATRIX;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 origPosition	: POSITION1;
	float3 normal   : NORMAL;
    float3 color : COLOR;
	float2 uv       : TEXCOORD;
};

typedef VSOutput PSInput;

VSOutput VSMain(VSInput input)
{
	VSOutput output;

	float w = input.weight / 100.f;
	uint idx1 = input.boneno.x;
	uint idx2 = input.boneno.y;

	float4x4 localModelMatrix = boneMatrix[idx1] * w + boneMatrix[idx2] * (1.0f - w);

	output.position = mul(wvp, mul(input.modelMatrix, mul(localModelMatrix, float4(input.position, 1.0f))));
	output.origPosition = mul(world, mul(input.modelMatrix,mul(localModelMatrix, float4(input.position, 1.0f))));

	output.normal = normalize(mul(world,mul(input.modelMatrix, mul(localModelMatrix,float4(input.normal, 0.0f)))).xyz);

	output.uv = input.uv;

	return output;
}


float4 PSMain(PSInput input) : SV_Target
{
    //float3 light = float3(-0.41, -0.82, 0.41);
    float3 light = lightEyeDir.xyz;
    float3 lightSpecularColor = float3(0.6f, 0.6f, 0.6f);
	float3 lightDiffuseColor = float3(0.6f, 0.6f, 0.6f);
	float3 lightAmbientColor = float3(1.0f, 1.0f, 1.0f);
    

	float3 vray = normalize(input.origPosition - eyePos.xyz);
	
	float brightness =  dot(input.normal, -light);
    
	float attenuation = 1.0f;

	/// �A���r�G���g�J���[�v�Z
	float3 modelAmbientColor = ambientColor * attenuation;
	modelAmbientColor = modelAmbientColor * lightAmbientColor;
    // �f�B�t���[�Y�J���[�v�Z
    float normalRad = acos(brightness);
    float3 modelDiffuseColor = diffuseColor.rgb * lightDiffuseColor;

	// �X�y�L�����v�Z
	float3 modelSpecularColor = specularColor * attenuation * lightSpecularColor;
    float spec = pow(saturate(dot(reflect(light, input.normal), -vray)), (100 - specularity)/10.0f);
	modelSpecularColor = modelSpecularColor * spec;

    
	// �X�t�B�A�}�b�v�v�Z
	
	float3 vrayAxisX = cross(upper.xyz, vray);
	float3 vrayAxisY = cross( vray, vrayAxisX);
	float2 sphereUV = float2(dot(vrayAxisX, input.normal), dot(vrayAxisY, input.normal));
	
	sphereUV = sphereUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);


	float4 texColor = surfaceTexture.Sample(smp, input.uv);
	
    float4 modelColor = saturate(float4(modelDiffuseColor + modelAmbientColor, diffuseColor.a));
    
    modelColor = modelColor * texColor * toonTexture.Sample(smp, float2(0.0f, saturate(normalRad / (3.141592f))));
	modelColor = modelColor * mulSphereTexture.Sample(smp, sphereUV) + addSphereTexture.Sample(smp, sphereUV);

	modelColor = saturate(modelColor + float4(modelSpecularColor, 0.0f));

    return modelColor;
}